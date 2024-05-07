#ifndef SV_H_
#define SV_H_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

typedef struct StringView
{
	size_t len;
	const char *data;
} StringView;

char *read_file_cstr(char *filename);

StringView sv_from_cstr(char *cstr);
StringView sv_construct(char *cstr, size_t len);

StringView sv_split_left(StringView *sv, char delim);
StringView sv_split_right(StringView *sv, char delim);

StringView sv_cut_left(StringView *sv, size_t num);
StringView sv_cut_right(StringView *sv, size_t num);

int sv_strip_left(StringView *sv);
int sv_strip_right(StringView *sv);

int sv_find_left_char(StringView *sv, char n);
int sv_find_right_char(StringView *sv, char n);

int sv_find_left_predicate(StringView *sv, void (*predicate)(char));

bool sv_starts_with(StringView sv, StringView sv_other);
bool sv_ends_with(StringView sv, StringView sv_other);

int sv_starts_with_predicate(StringView *sv, bool (*predicate)(char));
int sv_ends_with_predicate(StringView *sv, bool (*predicate)(char));

bool sv_whitespace_predicate(char n);

bool sv_compare(StringView sv, StringView sv_other);

#define StringViewFormat "%.*s"
#define StringViewNull sv_construct(NULL, 0)
#define StringViewFromStr(liter) (sv_construct(liter, strlen(liter)))

#endif

#ifdef SV_IMPLEMENTATION
#undef SV_IMPLEMENTATION

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

char *read_file_cstr(char *filename)
{
	/*Read whole file and returns string C style NULL terminated.
	Also enshures that new line and return are added at the end of the file.
	The string is malloc-ed and need to be freed after.*/
	FILE *f = fopen(filename, "rb");
	if (f == NULL)
	{
		printf("Could not open file!");
		exit(1);
	}
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET); /* same as rewind(f); */

	char *string = malloc((fsize + 3) * sizeof(char));
	fread(string, fsize, 1, f);
	fclose(f);

	// Guarantee that the file ends with carrage return, newline. Null termination. BS.
	if ((string[fsize - 1] != '\n') && (string[fsize - 2] != '\r'))
	{
		string[fsize] = 13;
		string[fsize + 1] = 10;
		string[fsize + 2] = 0;
	}
	else
	{
		string[fsize] = 0;
		string[fsize + 1] = 0;
		string[fsize + 2] = 0;
	}
	return string;
}

StringView sv_from_cstr(char *cstr)
{ /*Maybe not needed...*/
	size_t len = strlen(cstr);
	StringView sv = {.data = cstr, .len = len};
	return sv;
}

StringView sv_construct(char *cstr, size_t len)
{
	StringView sv;
	sv.len = len;
	sv.data = cstr;
	return sv;
}

StringView sv_split_left(StringView *sv, char delim)
{
	if (sv->len <= 0)
		return StringViewNull;
	int n = sv_find_left_char(sv, delim);
	if (n < 0)
		return *sv;

	StringView piece = {.data = sv->data, .len = n};

	sv->data = sv->data + n + 1;
	sv->len = sv->len - n - 1;
	return piece;
}

StringView sv_split_right(StringView *sv, char delim)
{
	if (sv->len <= 0)
		return StringViewNull;
	int n = sv_find_right_char(sv, delim);
	if (n < 0)
		return StringViewNull;

	StringView piece = {.data = sv->data + n + 1, .len = sv->len - n - 1};

	sv->len = n;
	return piece;
}

StringView sv_cut_left(StringView *sv, size_t num)
{
	if ((sv->len <= 0) | (num <= 0))
		return StringViewNull;

	StringView piece;
	if (num > sv->len)
	{
		piece.data = sv->data;
		piece.len = sv->len;
		*sv = StringViewNull;
	}
	else
	{
		piece.data = sv->data;
		piece.len = num;

		sv->data = sv->data + num;
		sv->len = sv->len - num;
	}
	return piece;
}

StringView sv_cut_right(StringView *sv, size_t num)
{
	if ((sv->len <= 0) | (num <= 0))
		return StringViewNull;

	StringView piece;
	if (num > sv->len)
	{
		piece.data = sv->data;
		piece.len = sv->len;
		*sv = StringViewNull;
	}
	else
	{
		piece.data = sv->data + sv->len - num;
		piece.len = num;
		sv->data = sv->data;
		sv->len = sv->len - num;
	}
	return piece;
}

int sv_find_left_char(StringView *sv, char n)
{
	for (size_t i = 0; i < sv->len; i++)
	{
		if (sv->data[i] == n)
			return i;
	}
	return -1;
}

int sv_find_right_char(StringView *sv, char n)
{
	for (int i = sv->len - 1; i >= 0; i--)
	{
		if (sv->data[i] == n)
			return i;
	}
	return -1;
}

int sv_find_left_predicate(StringView *sv, bool (*predicate)(char))
{
	{
		for (int i = 0; i < sv->len; i++)
			if ((*predicate)(sv->data[i]))
				return i;
		return -1;
	}
}

#define WHITESPACE_SYMBOLS " \t\r\n"
#define WHITESPACE_SYMBOLS_LEN sizeof(WHITESPACE_SYMBOLS)
bool sv_whitespace_predicate(char n)
{
	for (size_t i = 0; i < WHITESPACE_SYMBOLS_LEN; ++i)
	{
		if (n == WHITESPACE_SYMBOLS[i])
			return true;
	}
	return false;
}

int sv_strip_left(StringView *sv)
{
	int num_spaces = sv_starts_with_predicate(sv, sv_whitespace_predicate);
	sv_cut_left(sv, num_spaces);
	return num_spaces;
}

int sv_strip_right(StringView *sv)
{
	int num_spaces = sv_ends_with_predicate(sv, sv_whitespace_predicate);
	sv_cut_right(sv, num_spaces);
	return num_spaces;
}

bool sv_starts_with(StringView sv, StringView sv_other)
{
	if (sv.len < sv_other.len)
		return false;
	int res = memcmp(sv.data, sv_other.data, sv_other.len);
	return (bool)(res == 0);
}

int sv_starts_with_predicate(StringView *sv, bool (*predicate)(char))
{
	if (sv->len == 0)
		return 0;
	int count = 0;
	size_t i = 0;
	char n = sv->data[i];
	while (predicate(n))
	{
		count += 1;
		i += 1;
		n = sv->data[i];
	}
	return count;
}

bool sv_ends_with(StringView sv, StringView sv_other)
{
	if (sv.len < sv_other.len)
		return false;
	int diff = sv.len - sv_other.len;
	int res = memcmp(sv.data + diff, sv_other.data, sv_other.len);
	return (bool)res == 0;
}

int sv_ends_with_predicate(StringView *sv, bool (*predicate)(char))
{
	if (sv->len == 0)
		return 0;
	// int count = 0;
	// size_t i = sv->len;
	// char n = sv->data[i];
	// while (predicate(n))
	// {
	// 	count += 1;
	// 	i -= 1;
	// 	n = sv->data[i];
	// }
	int count = 0;
	for (size_t i = sv->len - 1; i >= 0 && predicate(sv->data[i]); i--)
		count += 1;
	return count;
}

bool sv_compare(StringView sv, StringView sv_other)
{
	/*Compares two string views.*/
	return (sv.len == sv_other.len) &&
		   ((sv.data == sv_other.data) || (memcmp(sv.data, sv_other.data, sv.len) == 0));
}

#endif