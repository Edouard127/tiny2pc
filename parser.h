/*
 * =====================================================================================
 *
 *       Filename:  parser.h
 *
 *    Description:  Tiny V2 C Parser 
 *
 *        Version:  1.0
 *        Created:  06/24/25 15:50:30
 *       Revision:  none
 *       Compiler:  clang
 *
 *         Author:  Edouard127 (https://edouard127.christmas), 
 *
 * =====================================================================================
 */

#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif
/* 
 * ===  STRUCT  ========================================================================
 *         Name:  TinyV2Name 
 *  Description:  
 * =====================================================================================
 */
typedef struct {
	char *intermediary;
	char *named;
} TinyV2Name;

/* 
 * ===  STRUCT  ========================================================================
 *         Name:  TinyV2Field
 *  Description: 
 * =====================================================================================
 */
typedef struct {
	char *comment;
	TinyV2Name *names;
	size_t names_count;
} TinyV2Field;

/* 
 * ===  STRUCT  ========================================================================
 *         Name:  TinyV2Method
 *  Description:  
 * =====================================================================================
 */
typedef struct {
	char *comment;
	TinyV2Name *names;
	size_t names_count;
} TinyV2Method;

/* 
 * ===  STRUCT  ========================================================================
 *         Name:  TinyV2Class
 *  Description:  
 * =====================================================================================
 */
typedef struct {
	TinyV2Name *names;
	size_t names_count;
	TinyV2Field *fields;
	size_t fields_count;
	TinyV2Method *methods;
	size_t methods_count;
} TinyV2Class;

/* 
 * ===  STRUCT  ========================================================================
 *         Name:  TinyV2File
 *  Description:  
 * =====================================================================================
 */
typedef struct {
	size_t version_major;
	size_t version_minor;
	char **namespaces;
	size_t namespaces_count;
	TinyV2Class *classes;
	size_t classes_count;
} TinyV2File;


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  tiny_v2_parser_create
 *  Description:  Creates a parser from the given file
 *  		  Returns an int code for success or failure.
 * =====================================================================================
 */
int
tiny_v2_parser_create(FILE *in, TinyV2File *out);


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  tiny_v2_parser_open
 *  Description:  Opens the given file and creates the parser.
 *  		  Returns an int code for success or failure.
 * =====================================================================================
 */
int
tiny_v2_parser_open(const char *file, const char *mode, TinyV2File *out);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  tiny_v2_free
 *  Description:  Frees all the memory of a TinyV2File struct
 * =====================================================================================
 */
void
tiny_v2_free(TinyV2File *file);


#ifdef __cplusplus
}
#endif

#ifndef PARSER_IMPL
#define PARSER_IMPL

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define SEPARATOR '\t'

static size_t
peek_until(char **input, int c)
{
	char *start = *input;
	while (c != **input) 
		(*input)++;
	
	return *input - start;
}

static char*
next_token(char **s)
{
	char *start = *s;
	while (**s &&
			**s != '\t' &&
			**s != '\v' &&
			**s != '\f' &&
			**s != '\r' &&
			**s != '\n' &&
			**s != ' ')
		(*s)++;
	if (start == *s) return NULL;

	char *token = malloc(*s - start + 1);
	if (NULL != token) {
		memcpy(token, start, *s - start);
		token[*s-start] = '\0';
	}

	return token;
}

static void
parse_line(char *line, TinyV2File *file, TinyV2Class *current_class)
{
	char *tok;

	size_t indent = peek_until(&line, SEPARATOR);
	line += indent;
	
	tok = next_token(&line);
	if (NULL == tok) return; // memory issue or empty line

	// TODO:
}

int
tiny_v2_parser_create(FILE *in, TinyV2File *out)
{
	char buf[1024];

	TinyV2Class *current_class = NULL;
	memset(out, 0, sizeof(TinyV2File));

	while (fgets(buf, sizeof(buf), in)) {
		parse_line(buf, out, current_class);

		if (current_class && out->classes_count > 0)
			current_class = &out->classes[out->classes_count-1];
	}

	fclose(in);

	return 0;
}

int
tiny_v2_parser_open(const char *file, const char *mode, TinyV2File *out)
{
	FILE *fd = fopen(file, mode);
	if (NULL == fd)
		return -1;

	return tiny_v2_parser_create(fd, out);
}

void
tiny_v2_free(TinyV2File *file)
{
	for (int i=0; i<file->namespaces_count; i++)
	       free(file->namespaces[i]);
	free(file->namespaces);

	for (int i=0; i<file->classes_count; i++) {
		TinyV2Class *class = &file->classes[i];

		for (int j=0;j<class->names_count; j++) {
			free(class->names[j].intermediary);
			free(class->names[j].named);
		}
		free(class->names);

		for (int j=0; j<class->fields_count; j++) {
			free(class->fields[j].comment);
			for (int k=0; k<class->fields[j].names_count; k++) {
				free(class->fields[j].names[k].intermediary);
				free(class->fields[j].names[k].named);
			}
			free(class->fields[j].names);
		}
		free(class->fields);

		for (int j=0; j<class->methods_count; j++) {
			free(class->methods[j].comment);
			for (int k=0; k<class->methods[j].names_count; k++) {
				free(class->methods[j].names[k].intermediary);
				free(class->methods[j].names[k].named);
			}
			free(class->methods[j].names);
		}
		free(class->methods);
	}
	free(file->classes);
}


#endif
#endif
