#ifndef INTERPRETER_H
#define INTERPRETER_H

#define HASH 1007

struct Question {
	int hashq;		   // stores the hash value of the question text
	char *type;		   // defaults to MCQ for version 1 (type = "MCQ")
	char *text;		   // has the text of the question
	double difficulty; // stores the difficulty level of the question (max = 1)
	int no_options;	   // stores the number of options for the question
	char **options;	   // contains the options in an array of strings
	char *correct_option; // stores the correct option of the question
};

typedef struct Question Question;

struct Question_Bank {
	int no_questions; // stores the number of questions in this question bank
	Question **questions; // Array of pointers to "struct Question pointers"
};
/*Represents a collection of questions, storing the number of questions and an array of pointers to Question structs.*/
typedef struct Question_Bank Question_Bank;

typedef struct User_Parameters User_Parameters;
struct User_Parameters {
	char *type;			// defaults to MCQ for version 1 (type = "MCQ")
	double difficulty;	// difficulty level which the user requires
	int no_questions;	// number of questions which the user needs
	char comparator[2]; // can be "==", "<", "<=", ">", ">="
};

typedef struct __user_parameters_set User_Parameters_Set;
struct __user_parameters_set {
	int no_params;
	User_Parameters *params; // array of user params
};

int comparator_fn_diff(double A, double B, char comparator[2]);
int comparator_fn_type(char *Question_type, User_Parameters *UP);
int get_Questions(User_Parameters *UP, Question_Bank *QB);
int create_QuestionPaper(Question **Question_collection, int size, int N);
int print_QuestionPaper(Question **QuestionPaper, int N);

#endif
