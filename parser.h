#ifndef __INCLUDE__PARSER_H
#define __INCLUDE__PARSER_H

#include "interpreter.h"
#include <stdbool.h>
#include <stdio.h>

#define INIT_STRING_SIZE 10
#define MAX_LINE_SIZE 200
#define MAX_NUM_QUESTIONS 100
#define QN_STRING "\\question"
#define TOKEN_DELIMITERS "="
#define UP_STRING "\\sample"

typedef struct __parser_state parser_state;
struct __parser_state {
	bool in_question;
	int line_number;
};

Question_Bank parse_qb_file(FILE *fstream); // to parse question bank file
User_Parameters_Set
parse_up_file(FILE *fstream); // to parse user params (qn paper) file

#endif // __INCLUDE__
