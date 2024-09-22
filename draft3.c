#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <sys/wait.h>


/*
We will be using a lexical analysis approach to solving the
problem presented.

We will tokenise the input file line by line and parse those
tokens to a function that builds the final output c file

This will happen in two steps, writing our function definitions
and our program function into seperate c files before linking the
two files into one output source file
*/

// defining some useful constants
#define MAX_TOKENS 10000
#define MAX_VALUE 100
#define MAX_ERROR 1000
#define MAX_FUNCTIONS 50
#define MAX_ARGS 50

// defining some counter variables that need to be static
// over our program
static int printCounter = 0;
static int functionsCounter = 0;


// defining tokens
typedef enum {
	/*
	Token list:
	
	TOKEN_INT,
	TOKEN_FLOAT,
	TOKEN_IDENTIFIER    : string of alpha characters < 12
	TOKEN_ASSIGNMENT    : <- keyword
	TOKEN_OPERATOR      : (+,-,*,/) math operators
	TOKEN_RETURN        : return keyword
	TOKEN_FUNCTION      : function keyword
	TOKEN_PRINT         : print keyword
	TOKEN_COMMA         : (,)
	TOKEN_LPAREN        : (
	TOKEN_RPAREN        : )
	TOKEN_END           : \0 escape character
	TOKEN_INDENT        : \t tab character
	*/
	
	TOKEN_INT,
	TOKEN_FLOAT,
	TOKEN_IDENTIFIER,
	TOKEN_ASSIGNMENT,
	TOKEN_OPERATOR,
	TOKEN_RETURN,
	TOKEN_FUNCTION,
	TOKEN_PRINT,
	TOKEN_COMMA,
	TOKEN_LPAREN,
	TOKEN_RPAREN,
	TOKEN_END,
	TOKEN_INDENT
} TokenType;

// enumeration object that defines the category of function
typedef enum {
	VOID,
	NUM
} FunctionType;

// our token structure
typedef struct {
	// enumerator defining type of token
	TokenType type;
	// value of said token
	char value[MAX_VALUE];
} Token;

// structure for storing the identifier and arguments of a
// generated function
typedef struct {
	char name[MAX_VALUE];
	char args[MAX_ARGS][MAX_VALUE];
} Function;


Token get_next_token(char *line, int *pos);
const char* token_type_to_string(TokenType type);


void get_line_tokens(char *line, Token *tokens, int *token_count) {
	// declaring a token and a position in our line
	Token token;
	int pos = 0;

	// get the next token from the line
	token = get_next_token(line, &pos);

	// while generated token != \0 char
	while (token.type != TOKEN_END) 
	{
		tokens[*token_count] = token;
		token = get_next_token(line, &pos);
		(*token_count)++;
	}
	tokens[*token_count] = token;
	(*token_count)++;
	Token end = {TOKEN_END, "end"};
	tokens[*token_count] = end;
}


// this one is some brain melting shit 

Token get_next_token(char* line, int* pos) {
	/*
	This function will take a pointer to an integer that dictates
	the current position in the token array. It will iterate through the
	array and check each characters value and generate tokens.

	We should be able to step through the line grabbing tokens for every
	keyword, number and assignment in the line. The function will also
	identify indents so that we can mark the body of functions.
	*/
	
	
	// declaring a token (this is what we will return when function is called)
	Token token;
	
	/* 
	we are going to be skipping whitespaces so lets check tabs first
	this was a headache, we will compare the address of the first char in the
	line to the address of where the \t or 4 whitespaces first appears in said
	line, this will allow us to identify if the line is tabbed
	*/

	if (*pos == 0 && (line == strstr(line, "    ") || line == strstr(line, "\t"))) {
			// we can add a counter for the number of tabs here if we need
			strcpy(token.value, "INDENT");
			token.type = TOKEN_INDENT;
			(*pos)++;
			return token;
	}
	//Here we are returning the token to avoid reassignment later 
	// Skip whitespace
	// We will just move pos along until a char is found
	while (isspace(line[*pos])) (*pos)++;  


	// Here we are checking for alphanumeric tokens (identifiers and keywords)
	if (isalpha(line[*pos])) {
		// We are identifying the start position of the token string
		int start = *pos;
		// It's going to iterate as long as we have an alphanumeric char
		// This will give us a position along the array
		while (isalnum(line[*pos])) (*pos)++;        
		// Here we are copying the string from start to pos into token.value
		
		strncpy(token.value, &line[start], *pos - start);
		// add the exit char when at the final position in the string
		token.value[*pos - start] = '\0';

		// compare the value we found to our keywords
		if (strcmp(token.value, "print") == 0)
			token.type = TOKEN_PRINT;
		else if (strcmp(token.value, "function") == 0)
			token.type = TOKEN_FUNCTION;
		else if (strcmp(token.value, "return") == 0)
			token.type = TOKEN_RETURN;
		
		// if a keyword isn't found then string must be an identifier
		else
			// check for conditions of an identifier - 12 lowercase letters
			if (*pos - start > 12) {
			fprintf(stderr, "Identifier '%s' is longer than 12 characters.\n", token.value);
			}
			else {
				// if condition is ok we assign token type
				token.type = TOKEN_IDENTIFIER;
			}
	}


	// Now handling numbers

	// check for a digit 
	else if (isdigit(line[*pos])) {
		// same trick to put the value of the number in the token.value
		int start = *pos;
		// if number continues or a dp is found then just keep pos++
		while (isdigit(line[*pos]) || line[*pos] == '.') (*pos)++;
		// one number ends copy the value as the string to the token.value
		strncpy(token.value, &line[start], *pos - start);
		// and append exit char
		token.value[*pos - start] = '\0';

		// identify if the number is a float or an int
		for (int i = 0; i < (*pos - start); i++) {
			if (token.value[i] == '.') {
				token.type = TOKEN_FLOAT;
				break;
			}
			else {
				token.type = TOKEN_INT;
			}
		}
	} 


	// TOKEN_ASSIGNMENT

	// check now for assignment: if '<' char is found check
	// for '-' in next position and if found set type to assignment
	else if (line[*pos] == '<' && line[*pos + 1] == '-') { 
		(*pos) += 2;
		token.type = TOKEN_ASSIGNMENT;
		strcpy(token.value, "<-");
	}


	// TOKEN_OPERATOR

	// check for an operator and copy into token.value
	else if (line[*pos] == '+' || line[*pos] == '-' || line[*pos] == '*' || line[*pos] == '/') { 
		token.value[0] = line[*pos];
		// appending a exit to the string
		token.value[1] = '\0';
		token.type = TOKEN_OPERATOR;
		(*pos)++;
	}

	// these are all similar but use strcpy to avoid using exit chars
	else if (line[*pos] == '(') {
		token.type = TOKEN_LPAREN;
		strcpy(token.value, "(");
		(*pos)++;
	} 
	else if (line[*pos] == ')') {
		token.type = TOKEN_RPAREN;
		strcpy(token.value, ")");
		(*pos)++;
	}
	else if (line[*pos] == ',') {
		token.type = TOKEN_COMMA;
		strcpy(token.value, ",");
		(*pos)++;
	} 

	// if the line is finished line[pos] will be the exit char
 	else if (line[*pos] == '\0') {
		token.type = TOKEN_END;
		strcpy(token.value, "end");
	} 
	
	// finally, if nothing makes sense we call an error
	else {
		fprintf(stderr, "Unexpected character: %c\n", line[*pos]);
		exit(1);
	}

	// return our generated token to the function
	return token;
}

bool not_comment(char * line) {
	// returning true when the first char is NOT a #
	return (line[0] != '#');
}

const char* token_type_to_string(TokenType type) {
	// switch to turn the identifier to a string of the same name
	// used for debugging

	switch (type) {
		case TOKEN_IDENTIFIER: return "TOKEN_IDENTIFIER";
		case TOKEN_INT: return "TOKEN_INT";
		case TOKEN_FLOAT: return "TOKEN_FLOAT";
		case TOKEN_ASSIGNMENT: return "TOKEN_ASSIGN";
		case TOKEN_OPERATOR: return "TOKEN_OPERATOR";
		case TOKEN_PRINT: return "TOKEN_PRINT";
		case TOKEN_FUNCTION: return "TOKEN_FUNCTION";
		case TOKEN_RETURN: return "TOKEN_RETURN";
		case TOKEN_LPAREN: return "TOKEN_LPAREN";
		case TOKEN_RPAREN: return "TOKEN_RPAREN";
		case TOKEN_COMMA: return "TOKEN_COMMA";
		case TOKEN_END: return "TOKEN_END";
		case TOKEN_INDENT: return "TOKEN_INDENT";
		default: return "UNKNOWN_TOKEN";
	}
}

void print_tokens(Token * tokens, int token_count) {
	// another function for debugging
	
	for (int i = 0; i < token_count; i++) {
			// I hope you arent reading this on vim otherwise this statement will be very longggggggggggggggggggg
			printf("Token Type: %-20s Token Value: %s\n", token_type_to_string(tokens[i].type), tokens[i].value);
		}
		printf("\n\n");
}


void build_fheader(Token * tokens, FunctionType type, int *pos, FILE *fout, Function* functionList);
void build_fbody(Token * tokens, FILE* fout, Function* functionList, int * pos);
void build_print(Token * tokens, FILE* fout, int * pos);
void build_assignment(Token * tokens, FILE* outfile, int *pos);
void build_return(Token * tokens, FILE* fout, int * pos);
const FunctionType check_function_type(Token * tokens, int * pos);
void build_function_call(Token * tokens, FILE* main, int * pos);

void build_function_close(FILE * functions) {
	// terminating curly brace for end of function
	fprintf(functions, "}\n\n");
}

void parse_tokens(Token * tokens, FILE* functions, FILE* main, FILE* variables, int token_count, Function * functionList) {
	// declaring a variable for our function type:
	// defines if the funtion being parsed will return a value or not
	// a marker for our position in the token array
	int pos = 0;
	
	/*
	when we build the body of a function we need to know so that we can
	terminate it with a curly brace one we have finished with the build
	*/
	bool buildingBody = false; // true when building body
	
	// 
	bool inList = false;


	/*
	function will find keywords and perform translations to an output
	file.

	function -  identify the type of function
				build function declaration with identifiers specified
				iterate pos until end of line

				check function type - checks for a return statement
				to assign void or numerical return

	print -     build an assignment for the expression being printed
				print out evaluated expression

	assignment- checks identifier and builds an assignment

	indent -    builds a functions body when an indent char is found
	*/

	// until we reach the end of the token array
	while (pos < token_count) {
		// checking the type of the pos-th token
		switch (tokens[pos].type) {
			case TOKEN_END: {
				// If we find the end of the line, we want to keep moving along.
				// The end token is a utility, not a marker.
				pos++;
				break;
			}
			case TOKEN_INDENT: {
				// If it recognises an indent, it will incremend and build the body from the next token
				// a line in the function
				pos++;
				buildingBody = true;
				build_fbody(tokens, functions, functionList, &pos);
				break;
			}
			case TOKEN_FUNCTION: {
				// We are checking if its already a function body, and if it is we are closing it so we can start a new one
				if (buildingBody) 
				{
					build_function_close(functions);
					buildingBody = false;
				}

				// This will start the function declaration
				FunctionType f_type;
				f_type = check_function_type(tokens, &pos);
				pos++;
				build_fheader(tokens, f_type, &pos, functions, functionList);
				break;
			}
			case TOKEN_IDENTIFIER: {
<<<<<<< HEAD
				// finishing function
				
=======
				// If we encounter an identifier while already builing, we want to close the function
>>>>>>> 7700726c6d9d0b2702ef160d001dcd8a2a414f00
				if (buildingBody) 
				{
					build_function_close(functions);
					buildingBody = false;
				}
				
				inList = false;
				
				// Here we check if the identifier we encountered is already in our function list
				for (int i = 0; i < functionsCounter; i++) 
				{
					if (strcmp(functionList[i].name, tokens[pos].value) == 0) 
					{
						build_function_call(tokens, main, &pos);
						inList = true;
						break;
					}
				}
				if (!inList)
				{
					build_assignment(tokens, main, &pos);
					break;
				}
				break;
			}
			case TOKEN_PRINT: {
				if (buildingBody)
				{
					build_function_close(functions);
					buildingBody = false;
				}
				build_print(tokens, main, &pos);
				break;
			}
			default: {
				break;
			}
		}
	}
	if (buildingBody) {
		build_function_close(functions);
	}
}

const FunctionType check_function_type(Token *tokens, int *pos) {
	// We are defining an int for the position in the array
	int i = *pos;
	while (true) {
		if (tokens[i].type == TOKEN_END && tokens[i+1].type != TOKEN_INDENT) {
			break;
		}
		if (tokens[i].type == TOKEN_INDENT && tokens[i+1].type == TOKEN_RETURN) {
			if (tokens[i+2].type == TOKEN_END) {
				return VOID;
			}
			else {
				return NUM;
			}
		}
		i++;
	}
	return VOID;
}


void build_void_function(Token * tokens, FILE* fout, Function* functionList, int * pos) {
	Function function;
	int argCount = 0;

	// Function declaration
	fprintf(fout, "void ");
	// Function identifier
	fprintf(fout, "%s", tokens[*pos].value);
	// Copy to function name
	strcpy(function.name, tokens[*pos].value);
	(*pos)++; // We will move to arguments

	// Printing out incoming function arguments
	fprintf(fout, "(");

	// This is finding and printing arguments
	while (tokens[*pos].type != TOKEN_END) 
	{
		fprintf(fout, "double %s", tokens[*pos].value);
		strcpy(function.args[argCount], tokens[*pos].value);
		// There are more arguments
		if (tokens[*pos+1].type != TOKEN_END) 
		{
			fprintf(fout, ", ");
		}
		(*pos)++;
	}
	fprintf(fout, ") {\n");

	functionList[functionsCounter] = function;
	functionsCounter++;
}

void build_num_function(Token * tokens, FILE* fout, Function* functionList, int * pos) {
	Function function;
	int argCount = 0;

	// We declare the function here
	fprintf(fout, "double ");
	// This is the function identifier
	fprintf(fout, "%s", tokens[*pos].value);
	// Copy to function name
	strcpy(function.name, tokens[*pos].value);
	(*pos)++; // Move to arguments

	// Incoming function arguments
	fprintf(fout, "(");

	// This is finding and printing arguments
	while (tokens[*pos].type != TOKEN_END) 
	{
		fprintf(fout, "double %s", tokens[*pos].value);
		strcpy(function.args[argCount], tokens[*pos].value);
		// Check if there are more arguments
		if (tokens[*pos+1].type != TOKEN_END) 
		{
			fprintf(fout, ", ");
		}
		(*pos)++;
	}
	fprintf(fout, ") {\n");

	functionList[functionsCounter] = function;
	functionsCounter++;
}

void build_function_call(Token * tokens, FILE* main, int * pos) {
<<<<<<< HEAD
	fprintf(main, "%s", tokens[*pos].value);
		(*pos)++;
	while (tokens[*pos].type != TOKEN_RPAREN) {
=======
	//This function will allow us to call functions from main
	fprintf(main, "%s", tokens[*pos].value);
		(*pos)++;
	while (tokens[*pos].type != TOKEN_RPAREN && tokens[*pos].type != TOKEN_END){
>>>>>>> 7700726c6d9d0b2702ef160d001dcd8a2a414f00
		fprintf(main, "%s", tokens[*pos].value);
		(*pos)++;
	}
	fprintf(main, ")");
	(*pos)++;
}

void build_fheader(Token * tokens, FunctionType f_type, int *pos, FILE* functions, Function* functionList) {
	if (tokens[*pos].type != TOKEN_IDENTIFIER) {
		fprintf(stderr, "!Trying to define function without an identifier");
		// This will scan through the funtion until the last line of body 
		// It will also break out of the loop when we dont find an indent
		while (true) {
			if (tokens[*pos].type == TOKEN_END && tokens[*pos+1].type != TOKEN_INDENT) {
			(*pos)++;
			return;
			}
			(*pos)++;
		}
	}
	if (f_type == VOID) {
		build_void_function(tokens, functions, functionList, pos);
	}
	if (f_type == NUM) {
		build_num_function(tokens, functions, functionList, pos);
	}
}

void build_fbody(Token * tokens, FILE* functions, Function * functionList, int * pos) {
	if (tokens[*pos].type == TOKEN_PRINT) {
		build_print(tokens, functions, pos);
	}
	if (tokens[*pos].type == TOKEN_RETURN) {
		build_return(tokens, functions, pos);
	}
	if (tokens[*pos].type == TOKEN_IDENTIFIER) {
		build_assignment(tokens, functions, pos);
	}	
}

void build_return(Token * tokens, FILE* fout, int * pos) {
	// This function writes out the return statement for return functions
	(*pos)++;
	fprintf(fout, "return ");
	while (tokens[*pos].type != TOKEN_END) {
		fprintf(fout, "%s", tokens[*pos].value);
		(*pos)++;
	}
	fprintf(fout, ";\n");
}

void build_assignment(Token * tokens, FILE* outfile, int *pos) {
	int count = 0;
	if (tokens[(*pos)+1].type == TOKEN_ASSIGNMENT) {
		count++;
		fprintf(outfile, "double ");
		fprintf(outfile, "%s", tokens[*pos].value);
		fprintf(outfile, " = ");
<<<<<<< HEAD
		// skipping over the assignment and identifier tokens
=======
		// We are skipping over the assignment and identifier char
>>>>>>> 7700726c6d9d0b2702ef160d001dcd8a2a414f00
		*pos += 2;
		while (tokens[*pos].type != TOKEN_END) {
			fprintf(outfile, "%s", tokens[*pos].value);
			(*pos)++;
		}
		fprintf(outfile, ";\n");
	}
}

void build_print(Token *tokens, FILE* fout, int * pos) {
<<<<<<< HEAD
=======
	// This function is checking the type of the value to be printed
	// It then prints it accordingly depending on the value type
>>>>>>> 7700726c6d9d0b2702ef160d001dcd8a2a414f00
	(*pos)++;

	// ERROR FOR CHECKER

	if (tokens[*pos].type == TOKEN_END) {
		fprintf(stderr, "!Print statement with no expression");
		exit(EXIT_FAILURE);
	}

	fprintf(fout, "double printout%d =", printCounter);
	while (tokens[*pos].type != TOKEN_END) {
		fputs(" ", fout);
		fputs(tokens[*pos].value, fout);
		*pos += 1;
	}
	fputs(";\n", fout);
	fprintf(fout, "if ((printout%d - (int)printout%d) == 0) {\nprintf(", printCounter, printCounter);
	fputs("\"%d\\n\"", fout);
	fprintf(fout, ", (int)printout%d);\n}\nelse {\nprintf(", printCounter);
	fputs("\"%lf\\n\"", fout);
	fprintf(fout, ", printout%d);\n}\n", printCounter);
	printCounter++;
}

void compiler(){
	//This function will compile the out.c file and check for errors
	int result = system("cc -std=c11 -Wall -Werror -o out out.c");
	if (result !=0) {
		fprintf(stderr, "!Error in compilation \n");
		exit(EXIT_FAILURE);
	}
	system("chmod +x ./out");
}

void execution(){
	//This function will execute the out.c file
	int result = system("./out");
	if (result !=0) {
		fprintf(stderr, "Error in execution \n");
	}
}

void remove_files(void) {
	system("rm functions.c");
	system("rm main.c");
	system("rm out.c");
}

void write_to_out(FILE* fout) {
<<<<<<< HEAD
	FILE* variables = fopen("variables.c", "w");
=======
	//This function is initialising the functions.c and main.c files 
	
>>>>>>> 7700726c6d9d0b2702ef160d001dcd8a2a414f00
	FILE* functions = fopen("functions.c", "r");
	FILE* main = fopen("main.c", "r");
	
	if (functions == NULL) {
		printf("cannot open functions");
	}
	if (main == NULL) {
		printf("cannot open main");
	}

	char buffer[BUFSIZ];

	fprintf(fout, "#include <stdio.h>\n\n");
	while(fgets(buffer, sizeof(buffer), variables) != NULL) {
		fprintf(fout, "%s", buffer);
	}
	while(fgets(buffer, sizeof(buffer), functions) != NULL) {    
		fprintf(fout, "%s", buffer);
	}
	fprintf(fout, "\n\nint main(void) {\n");
	while(fgets(buffer, sizeof(buffer), main) != NULL) {
		fprintf(fout, "%s", buffer);
	}
	fprintf(fout, "return 0;\n}\n");

	fclose(fout);
}

int main(void) {
<<<<<<< HEAD
	// filename added for testing (mod when you want to change sources)
	char filename[] = "example.ml";
=======
	// Filename added for testing (mod when you want to change sources)
	char filename[] = "program1.ml";
>>>>>>> 7700726c6d9d0b2702ef160d001dcd8a2a414f00

	
	// Allocating a pointer to our tokens array
	Token* tokens = malloc(MAX_TOKENS * sizeof(Token));
	int token_count = 0;
	
	Function* functionList = malloc(MAX_FUNCTIONS * sizeof(Function));
	
	// File pointer to our file in read mode
	FILE* infile = fopen(filename, "r");
	if (infile == NULL) 
	{
		fprintf(stderr, "!File: %s could not be opened.", filename);
	}

	// Buffer for our line to sit in
	char line[BUFSIZ];
	
	// Get lines sequentially
	while (fgets(line, sizeof(line), infile) != NULL) 
	{
		// comment check, get tokens for token array
		if (not_comment(line)) 
		{
			get_line_tokens(line, tokens, &token_count);
		}
	}

	// File pointers for our outputted c code
	FILE* fout = fopen("out.c", "w");
	FILE* variables = fopen("variables.c", "w");
	FILE* functions = fopen("functions.c", "w");
	FILE* main = fopen("main.c", "w");

<<<<<<< HEAD
	// parsing the tokens in the tokens array
	parse_tokens(tokens, functions, main, variables, token_count, functionList);
=======
	// Parsing the tokens in the tokens array
	parse_tokens(tokens, functions, main, token_count, functionList);
>>>>>>> 7700726c6d9d0b2702ef160d001dcd8a2a414f00
	
	printf("\nprinting functions\n\n");
	for (int i = 0; i < functionsCounter; i++) {
		printf("%s\n", functionList[i].name);
	}

	fclose(variables);
	fclose(functions);
	fclose(main);

	// Free our allocated memory
	free(tokens);
	
	write_to_out(fout);

	// compiler();
	// execution();
    // remove_files();
}