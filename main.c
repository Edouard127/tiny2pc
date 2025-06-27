/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/24/25 17:53:26
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Edouard127 (https://edouard127.christmas), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "parser.h"

int
main()
{
	TinyV2File file;
	tiny_v2_parser_open("./mappings.tiny", "r", &file);

	return 0;
}

