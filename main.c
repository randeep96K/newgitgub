
#include "interpreter.h"
#include "list.h"
#include "listwrapper.h"
#include "parser.h"
#include "utils.h"
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include<string.h>

void print_AnswerKey(Question **QP, int num_questions) {
    printf("\nAnswer Key\n");
    printf("----------------------\n");

    for (int i = 0; i < num_questions; i++) {
        
        printf("Question.%d]  %s\n", i+1, QP[i]->correct_option);
       
    }
     printf("----------------------\n");
}


int main(int argc, char *const argv[])
{
	// in_file is the file from which the question bank is read
	// up_file is the file from which the user parameters are read
	char *in_file = NULL, *up_file = NULL;
	FILE *instream = stdin;
	FILE *upstream = NULL;

	// parse commandline arguments
	int opt;
	while ((opt = getopt(argc, argv, "i:u:o:")) != -1) {
		switch (opt) {
		case 'i': in_file = optarg; break;
		case 'u': up_file = optarg; break;
		case 'h':
		default:
			fprintf(
				stderr,
				"\nUsage: %s -i [question bank file] -u [user params file]\n\n",
				argv[0]);
			return 0;
		}
	}

	if (optind > argc && argc != 1) {
		fprintf(stderr, "Expected argument after options\n");
		return 0;
	}

	if (in_file != NULL)
		instream = fopen(in_file, "r");

	if (up_file != NULL)
		upstream = fopen(up_file, "r");

	Question_Bank qb = parse_qb_file(instream);

	User_Parameters_Set ups = parse_up_file(upstream);

	extern Question **Question_collection;
	extern Question **QP;
	int width = 80;
	char str[] = "COEP TECHNOLOGICAL UNIVERSITY";
	int length = sizeof(str) - 1;  
	int pad = (length >= width) ? 0 : (width - length) / 2;

	printf("%*.*s%s\n", pad, pad, " ", str);
	int width1 = 80;
	char str1[] = "SUBJECT: DSA I / DSA II";
	int length1 = sizeof(str1) - 1;  
	int pad1 = (length1 >= width1) ? 0 : (width1 - length1) / 2;

	printf("%*.*s%s\n", pad1, pad1, " ", str1);

	int choice;
	printf("1. For Generating Question Paper and Answer key\n");
	printf("2. For Generating Question Paper only\n");
	scanf("%d", &choice);
	while(1){
		if(choice==1){
		
			printf("Question Paper\n");
			printf("All Questions are COMPULSORY\n\n");
			printf("----------------------\n");

			int num_qualified_questions;
			for (int i = 0; i < ups.no_params; i++) {
				num_qualified_questions = get_Questions(ups.params + i, &qb);

				create_QuestionPaper(Question_collection, num_qualified_questions,
									 ups.params[i].no_questions);

				print_QuestionPaper(QP, ups.params[i].no_questions);
			}

			printf("----------------------\n");
			printf("End of question paper\n");
			for (int i = 0; i < ups.no_params; i++) {
				print_AnswerKey(QP, ups.params[i].no_questions);
			}
			exit(0);
		}
		else if(choice==2){
			printf("Question Paper\n");
			printf("All Questions are COMPULSORY\n\n");
			printf("----------------------\n");

			int num_qualified_questions;
			for (int i = 0; i < ups.no_params; i++) {
				num_qualified_questions = get_Questions(ups.params + i, &qb);

				create_QuestionPaper(Question_collection, num_qualified_questions,
									 ups.params[i].no_questions);

				print_QuestionPaper(QP, ups.params[i].no_questions);
			}

			printf("----------------------\n");
			printf("End of question paper\n");
			exit(0);
		}
		else{
			printf("Make a valid choice. Exiting..\n");
			exit(0);
		}
	}
	return 0;
}
