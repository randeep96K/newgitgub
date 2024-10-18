#include "interpreter.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

Question **Question_collection;
Question **QP;

// returns 1 if the user requirement is fulfilled
int comparator_fn_diff(double A, double B, char comparator[2])
{
	// B is the difficulty level specified by the user
	// A is the difficulty level of the question

	if (!(strcmp(comparator, "==")))
		return (A == B);

	else if (!(strcmp(comparator, "<")))
		return (A < B);

	else if (!(strcmp(comparator, "<=")))
		return (A <= B);

	else if (!(strcmp(comparator, ">")))
		return (A > B);

	else
		return (A >= B);
}

// returns 1 if the user requirement is fulfilled (else 0)
//Compares the type of a question (Question_type) with the type specified by the user (UP->type) and returns 1 if they match or if the user wants questions of both types
int comparator_fn_type(char *Question_type, User_Parameters *UP)
{
	if (!(strcmp(UP->type, "BOTH")))
		return 1;

	if (!(strcmp(UP->type, Question_type)))
		return 1;

	return 0;
}

//Filters questions from a question bank based on user parameters (UP) such as difficulty level, question type, and comparator.
// stores the questions (in an array) which meet the requirement of the user
int get_Questions(User_Parameters *UP, Question_Bank *QB)
{
	int curr_size = 1;
	int incr_size = 1;

	Question_collection = NULL;

	Question_collection = (Question **)malloc(curr_size * sizeof(Question *));

	double diffA;
	double diffB = UP->difficulty;

	int check_diff, check_type;

	int j = 0;
	for (int i = 0; i < QB->no_questions; i++) {
		diffA = QB->questions[i]->difficulty;
		check_diff = comparator_fn_diff(diffA, diffB, UP->comparator);
		check_type = comparator_fn_type(QB->questions[i]->type, UP);

		if (check_diff && check_type) {
			// increment the size of array to avoid overflow
			if (j == curr_size) {
				curr_size = curr_size + incr_size;
				Question_collection = (Question **)realloc(
					Question_collection, curr_size * sizeof(Question *));
			}

			Question_collection[j] = (Question *)malloc(sizeof(Question));
			Question_collection[j] = QB->questions[i];
			j++;
		}
	}

	return j;
}

string_node **hash_Table = NULL;

//@param Creates a question paper (QP) of size N from the filtered question collection
//Ensures that each question appears only once in the question paper.
int create_QuestionPaper(Question **Question_collection, int size, int N)
{
	if (hash_Table == NULL) {
		hash_Table = (string_node **)malloc(sizeof(string_node *) * HASH);
	}
	srand(time(0));

	QP = NULL;

	if (size >= N) {
		QP = (Question **)malloc(N * sizeof(Question *));

		int i = 0;
		int index = 0;
		int j = 0;
//Begin a while loop to select questions for the question paper until the desired count N is reached.
		while (i < N) {
			index = rand() % size;

			// using linear probing to randomly pick a question
			if (Question_collection[index] == NULL) {
				int k = 0;
				j = (index + 1) % size;
				while (k < size) {
					if (Question_collection[j] != NULL)
						break;
					j = (j + 1) % size;
					k++;
				}
			}

			else
				j = index;
	//Check if the selected question is not already included in the question paper.
			if (findString(Question_collection[j]->text, HASH, hash_Table) ==
				0) {
				insertString(Question_collection[j]->text, HASH, hash_Table);
				QP[i] = (Question *)malloc(sizeof(Question));
				QP[i] = Question_collection[j];
				Question_collection[j] = NULL;
				i++;
			}

			int k = 0;
			int ct = 0;

			while (k < size) {
				if (Question_collection[k] != NULL) {
					ct = 1;
					break;
				}
				k++;
			}
//error handling
			if (ct == 0) {
				printf("No more questions available!\n");
				break;
			}
		}
	}

	else {
		printf("Not enough Questions.\n");
		QP = NULL;
	}

	return 0;
}

int print_QuestionPaper(Question **QuestionPaper, int N)
{
	static int i = 0;
	if (QuestionPaper != NULL) {
		for (int j = 0; j < N; j++) {
			i++;
			printf("%d. %s\n", i, QuestionPaper[j]->text);

			if (strcmp(QuestionPaper[j]->type, "MCQ") == 0) {
				for (int k = 0; k < QuestionPaper[j]->no_options; k++) {
					printf("  %c) %s\n", 'a' + k, QuestionPaper[j]->options[k]);
				}
			}
			putchar('\n');
		}
	}
	//error handling
	else
		fprintf(stderr, "No Questions to display!\n");

	return 0;
}
