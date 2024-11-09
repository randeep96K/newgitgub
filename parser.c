#include "parser.h"
#include "interpreter.h"
#include "listwrapper.h"
#include "utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

parser_state state;

Question_Bank parse_qb_file(FILE *fstream) /*{{{*/
{
	state.in_question = false;
	state.line_number = 0;

	Question_Bank ret_qb;
	ret_qb.no_questions = 0;
	ret_qb.questions = malloc(sizeof(Question *) * MAX_NUM_QUESTIONS);
	// Currently, the limit on the number of questions is hard-coded

	Question *cur_qn;

	size_t line_size = MAX_LINE_SIZE;
	char *line = (char *)malloc(line_size * sizeof(char));

	// read a line from the file and find brackets
	line = fgets(line, line_size, fstream);
	while (line != NULL) {
		// remove newline character from the line
		char *newline = strchr(line, '\n');
		if (newline)
			*newline = '\0';

		state.line_number++;

		// we're in the question if question string was found at start of line
		if ((state.in_question == false) && (strstr(line, QN_STRING) == line)) {
			state.in_question = true;

			// allocate memory for the current qn
#ifdef PARSER_DEBUG
			printf("allocating memory for cur_qn!\n");
#endif
			cur_qn = (Question *)malloc(sizeof(Question));
		}

		// we're out of the question if we encounter a blank line
		else if (isBlankLine(line) && (state.in_question == true)) {
			state.in_question = false;

			// once we're out of the question, add it to the question bank
			ret_qb.questions[ret_qb.no_questions] = cur_qn;
			ret_qb.no_questions++;

			// and invalidate cur_qn now
			cur_qn = NULL;
		}

		else {
			char *bracket_open = strchr(line, '{');
			char *bracket_close = strchr(line, '}');

			if (bracket_open == NULL || bracket_close == NULL ||
				bracket_open > bracket_close) {
				fprintf(stderr,
						"Error: mismatched/missing brackets on line %d!\n",
						state.line_number);
				exit(EXIT_FAILURE);
			}

			// tokenize the line, before and after delim
			// and don't include the tokens themselves
			char *pre_token = strtok(line, TOKEN_DELIMITERS) + 1;
			char *post_token = strtok(NULL, TOKEN_DELIMITERS);
			*bracket_close = '\0';

			if (cur_qn == NULL) {
				fprintf(stderr, "Error: unexpected error occured at line %d!\n",
						state.line_number);
				exit(EXIT_FAILURE);
			}

			// clean the substrings and call assign
			else {
				int pre_token_length = strlen(pre_token),
					post_token_length = strlen(post_token);
				char *pre_token_sanitized =
					stripWhitespace(pre_token, &pre_token_length);
				char *post_token_sanitized =
					stripWhitespace(post_token, &post_token_length);

#ifdef PARSER_DEBUG
				printf("pre_token_sanitized: %s\n", pre_token_sanitized);
				printf("post_token_sanitized: %s\n", post_token_sanitized);
#endif

				assign(cur_qn, pre_token_sanitized, post_token_sanitized);

#ifdef PARSER_DEBUG
				assign(cur_qn, "type", "mcq");
				printf("LOOK HERE!\n");
				printf("cur qn\n");
				printf("type: %s\n", cur_qn->type);
				printf("text: %s\n", cur_qn->text);
#endif
			}
		}

		line = fgets(line, line_size, fstream);
	}

	// After EOF, we could've just ended a question
	state.in_question = false;

#ifdef PARSER_DEBUG
	printf("cur qn\n");
	printf("type: %s\n", cur_qn->type);
	printf("text: %s\n", cur_qn->text);
#endif

	// once we're out of the question, add it to the question bank
	*(ret_qb.questions + ret_qb.no_questions) = cur_qn;
	ret_qb.no_questions++;

#ifdef PARSER_DEBUG
	printf("ret_qb.questions[ret_qb.no_questions - 1]\n");
	printf("type: %s\n", ret_qb.questions[ret_qb.no_questions - 1]->type);
	printf("text: %s\n", ret_qb.questions[ret_qb.no_questions - 1]->type);
#endif

	// and invalidate cur_qn now
	cur_qn = NULL;

	return ret_qb;
} /*}}}*/

User_Parameters_Set parse_up_file(FILE *fstream) /*{{{*/
{
#ifdef PARSER_DEBUG
	printf("parse_up_file() function called!\n");
#endif
	state.in_question = false;
	state.line_number = 0;

	User_Parameters_Set ret_user_params;
	User_Parameters cur_params;
	list params_list;
	listInit(&params_list, sizeof(User_Parameters));

	size_t line_size = MAX_LINE_SIZE;
	char *line = (char *)malloc(line_size * sizeof(char));

	// get lines from the file one by one
	line = fgets(line, line_size, fstream);
	while (line != NULL) {

		// remove newline character from the line
		char *newline = strchr(line, '\n');
		if (newline)
			*newline = '\0';

#ifdef PARSER_DEBUG
		printf("reading in a new line: %s\n", line);
#endif

		state.line_number++;

		// we're in the question if user params string was found at start of
		// line
		if ((state.in_question == false) && (strstr(line, UP_STRING) == line)) {
			state.in_question = true;

			// clear cur_params to default
			cur_params.type = NULL;
			cur_params.difficulty = 0;
			cur_params.no_questions = 0;
			cur_params.comparator[0] = ' ';
			cur_params.comparator[1] = ' ';
		}

		// we're out of the question if we encounter a blank line
		else if (isBlankLine(line) && (state.in_question == true)) {
			state.in_question = false;

			// now append it to the list
			User_ParametersListAppend(&params_list, cur_params);

#ifdef PARSER_DEBUG
			printf("DEBUG:\n");
			printf("type: %s\n", cur_params.type);
			printf("difficulty: %lf\n", cur_params.difficulty);
			printf("number: %d\n", cur_params.no_questions);
			printf("comparator: %c%c\n", cur_params.comparator[0],
				   cur_params.comparator[1]);
#endif
		}

		else {
			char *bracket_open = strchr(line, '{');
			char *bracket_close = strchr(line, '}');

			if (bracket_open == NULL || bracket_close == NULL ||
				bracket_open > bracket_close) {
				fprintf(stderr,
						"Error: mismatched/missing brackets on line %d!\n",
						state.line_number);
				exit(EXIT_FAILURE);
			}

			// generate the substring in between {}
			size_t sublen = (size_t)(bracket_close - bracket_open + 1);
			char *substr = malloc((sublen + 1) * sizeof(char));
			// +1 in first line because close and open point to the start of
			// each char +1 in the second line to account for null terminator

			memcpy(substr, bracket_open, sublen);
			substr[sublen] = '\0'; // null terminate

			// convert the substring to all uppercase
			for (int index = 0; index < sublen; index++) {
				substr[index] = toupper(substr[index]);
			}

			// Here we must do the parsing slightly differently because
			// difficulty might have a two char comparator
			if (strstr(substr, "TYPE") != NULL) {
				strtok(
					substr,
					TOKEN_DELIMITERS); // pre_token should be "type" either way
				char *post_token = strtok(NULL, TOKEN_DELIMITERS);

				// remove the closing bracket from the string
				char *close_bracket = strchr(post_token, '}');
				*close_bracket = '\0';

				int post_token_len = strlen(post_token);
				char *post_token_sanitized =
					stripWhitespace(post_token, &post_token_len);

				cur_params.type = post_token_sanitized;
			}

			else if (strstr(substr, "NUMBER") != NULL) {
				strtok(substr, TOKEN_DELIMITERS); // pre_token should be
												  // "number" either way
				char *post_token = strtok(NULL, TOKEN_DELIMITERS);

				int post_token_len = strlen(post_token);
				char *post_token_sanitized =
					stripWhitespace(post_token, &post_token_len);

				cur_params.no_questions = atoi(post_token_sanitized);
			}

			else if (strstr(substr, "DIFFICULTY") != NULL) {

				if (strstr(substr, ">=") != NULL) {
					cur_params.comparator[0] = '>';
					cur_params.comparator[1] = '=';
				}
				else if (strstr(substr, "<=") != NULL) {
					cur_params.comparator[0] = '<';
					cur_params.comparator[1] = '=';
				}
				else if (strstr(substr, "=") != NULL) {
					cur_params.comparator[0] = '=';
					cur_params.comparator[1] = '=';
				}
				else if (strstr(substr, ">") != NULL) {
					cur_params.comparator[0] = '>';
					cur_params.comparator[1] = '\0';
				}
				else if (strstr(substr, "<") != NULL) {
					cur_params.comparator[0] = '<';
					cur_params.comparator[1] = '\0';
				}

				strtok(substr,
					   ">=<"); // pre_token should be "number" either way
				char *post_token = strtok(NULL, ">=<");

				int post_token_len = strlen(post_token);
				char *post_token_sanitized =
					stripWhitespace(post_token, &post_token_len);

				cur_params.difficulty = strtod(post_token_sanitized, NULL);
			}
		}

		// finally after processing the current line, get a new one
		line = fgets(line, line_size, fstream);
	}

	// after EOF we could've possible just left a question
	state.in_question = false;
	// now append it to the list
	User_ParametersListAppend(&params_list, cur_params);

#ifdef PARSER_DEBUG
	printf("DEBUG:\n");
	printf("type: %s\n", cur_params.type);
	printf("difficulty: %lf\n", cur_params.difficulty);
	printf("number: %d\n", cur_params.no_questions);
	printf("comparator: %c%c\n", cur_params.comparator[0],
		   cur_params.comparator[1]);
#endif

	// now params_list contains everything, so we copy the user params
	// to the set to be returned

	ret_user_params.no_params = params_list.num_elems;
	ret_user_params.params = params_list.buf;

	return ret_user_params;
} /*}}}*/
