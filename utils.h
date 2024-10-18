#ifndef __UTILS_H
#define __UTILS_H

#include "interpreter.h"
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SMALLPRIME 33
#define BIGPRIME 989999
#define LIMIT 1000000
#define MAX_SIZE_OPTION 1000

typedef Question *Question_B;
typedef struct __string_node string_node;

struct __string_node {
	char *str;
	string_node *next;
};

void assign(Question_B question, char string_before[], char string_after[]);
void assignType(Question_B question, char *string_after);
void assignDiff(Question_B question, char *string_after);
void assignText(Question_B question, char *string_after);
void assignOpt(Question_B question, char *string_after);
void assignOptCheck(Question_B question, char *string_after);
void assignAns(Question_B question, char *string_after);
char *stripWhitespace(char *line, int *length);
bool isBlankLine(char *line);

int hashString(const char *str);
int insertString(const char *str, int n, string_node *hashtable[]);
int findString(const char *str, int n, string_node *hashtable[]);

#endif
