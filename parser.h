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

typedef struct {
	
} parser;

typedef struct {
	size_t line;
	size_t indent;
} parseNode;

typedef enum {
	TOKEN_HEADER,
	TOKEN_PROPERTY,
	TOKEN_CLASS,
	TOKEN_FIELD,
	TOKEN_METHOD,
	TOKEN_PARAMETER,
	TOKEN_LOCAL_VARIABLE,
	TOKEN_COMMENT,
	TOKEN_ERROR,
	TOKEN_EOF
} token_type;

typedef struct {
	token_type type;
	size_t indent;
	size_t line;
	char *error;
} token;


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  tiny_parser_create
 *  Description:  Creates a parser from the given file
 * =====================================================================================
 */
parser
*tiny_parser_create(FILE *file);


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  tiny_parser_open
 *  Description:  Opens the given file and creates the parser.
 *  		  Returns the parser struct or NULL if something
 *  		  went wrong.
 * =====================================================================================
 */
parser
*tiny_parser_open(const char *file, const char *mode);

#endif PARSER_H

#ifndef PARSER_IMPL
#define PARSER_IMPL

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void
_tiny_parser_parse(char *input)
{
	const char *line_start = input;
	size_t line = 0;
	size_t indent = 0;


	while (*input) {
		token_type token = TOKEN_ERROR;
					    
		switch (*input) {
			case '\n':
				line_start=input+1;
				indent=0;
				line++;
				break;
			case '\t':
				indent++;
				break;;
			case 'c':
				if (indent == 0)
					token = TOKEN_CLASS;
				break;
			case 'f':
				if (indent == 1)
					token = TOKEN_FIELD;
				break;
			case 'm':
				if (indent == 1)
					token = TOKEN_METHOD;
				break;
			case 'p':
				if (indent == 2)
					token = TOKEN_PARAMETER;
				break;
			default:
				goto consume;
		}

		printf("Line: %d, Indent: %d, Char: %c, Token: %d\n", line, indent, *input, token);

consume:
		input++;
	}
}

parser
*tiny_parser_create(FILE *file)
{
	if (NULL == file)
		return NULL;

	fseek( file, 0, SEEK_END);
	long size = ftell( file );
	fseek( file, 0, SEEK_SET );

	char *buf = malloc( size+1 );
	if (NULL == buf)
		return NULL;

	fread(buf, 1, size, file);
	buf[size] = '\0';

	fclose(file);

	_tiny_parser_parse( buf );
	return NULL;
}

parser
*tiny_parser_open(const char *file, const char *mode)
{
	char *_mode = mode;

	if (NULL == file)
		return NULL;

	if (NULL == _mode)
		_mode = "r";

	FILE *fd = fopen( file, _mode );
	if (NULL == fd)
		return NULL;

	return tiny_parser_create( fd );
}


#endif
