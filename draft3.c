#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_TOKENS 10000
#define MAX_TOKEN_VALUE 100
#define MAX_ERROR 1000

static int printCounter = 0;

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

typedef enum {
    INITIAL,
    BUILDING_PRINT,
    BUILDING_FUNCTION,
    BUILDING_BODY,
} BuildState;

typedef enum {
    VOID,
    NUM
} FunctionType;


typedef struct {
    TokenType type;
    char value[MAX_TOKEN_VALUE];
} Token;


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
    the functions position in the array. It will iterate through the
    array and check each characters value and do something.

    We should be able to step through the line grabbing tokens for every
    keyword, number and assignment in the line. The function will also
    identify indents so that we can see the body of functions.

    I'll comment to explain but hit me up to talk about it
    */
    
    
    // declaring a token (this is what we will return when called)
    Token token;
    
    // we are going to be skipping whitespaces so lets check tabs first
    if (*pos == 0 && line[*pos] == '\t') {
        // we can add a counter for the number of tabs here if we need
        while (line[*pos] == '\t') {
            // counter goes here
            (*pos)++;
        }
        strcpy(token.value, "INDENT");
        token.type = TOKEN_INDENT;
        return token; //returning to avoid reassignment
    }
    
    while (isspace(line[*pos])) (*pos)++;  // Skip whitespace

    // checking for alphanumeric tokens (identifiers and keywords)
    if (isalpha(line[*pos])) {
        // identify the start of the string value
        int start = *pos;
        // iterate as long as we have an alphanumeric char
        // this will give us a postion along the array
        while (isalnum(line[*pos])) (*pos)++;        
        // copy string from start to pos into token.value
        /*
        if you arent sure, strcpy will copy from a memory address
        &line[start] - the start of the string
        and will copy some amount of bytes 
        (*pos - start) - final position minus the start position
        to some address (token.value)
        */
        strncpy(token.value, &line[start], *pos - start);
        // add the exit char when at the final position in the string
        token.value[*pos - start] = '\0';

        // check for a keyword first
        if (strcmp(token.value, "print") == 0)
            token.type = TOKEN_PRINT;
        else if (strcmp(token.value, "function") == 0)
            token.type = TOKEN_FUNCTION;
        else if (strcmp(token.value, "return") == 0)
            token.type = TOKEN_RETURN;
        
        // if a keyword isn't found then string is an identifier
        else
            // check for conditions of an identifier - 12 lowercase letters
            if (*pos - start > 12) {
            fprintf(stderr, "Identifier '%s' is longer than 12 characters.\n", token.value);
            }
            else {
                token.type = TOKEN_IDENTIFIER;
            }
    } 

    //hello

    // check for a digit 
    else if (isdigit(line[*pos])) {  // Numbers
        int start = *pos;
        // if number continues or a dp is found then just increase position
        while (isdigit(line[*pos]) || line[*pos] == '.') (*pos)++;
        // copy the value as a string to the token.value
        strncpy(token.value, &line[start], *pos - start);
        token.value[*pos - start] = '\0';
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

    // check now for assignment: if the less than char is found check
    // for a - in next position and if found then its an assignment
    else if (line[*pos] == '<' && line[*pos + 1] == '-') { 
        (*pos) += 2;
        token.type = TOKEN_ASSIGNMENT;
        strcpy(token.value, "<-");
    }
    // check for an operator and copy into token.value
    else if (line[*pos] == '+' || line[*pos] == '-' || line[*pos] == '*' || line[*pos] == '/') {  // Operators
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

    // if the string is finished then we find an exit character and just
    // return the end token
    else if (line[*pos] == '\0') {
        token.type = TOKEN_END;
    } 
    // finally, if nothing makes sense we say that 
    else {
        printf("Unexpected character: %c\n", line[*pos]);
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
    for (int i = 0; i < token_count; i++) {
            printf("Token Type: %-20s Token Value: %s\n", token_type_to_string(tokens[i].type), tokens[i].value);
        }
        printf("\n\n");
}

void build_fheader(Token * tokens, FunctionType type, int *pos, FILE *fout);
void build_fbody(Token * tokens, FILE* fout, int * pos);
void build_print(Token * tokens, FILE* fout, int * pos);
void build_assignment(Token * tokens, FILE* fout, int *pos);
void build_return(Token * tokens, FILE* fout, int * pos);
const FunctionType check_function_type(Token * tokens, int * pos);

void parse_tokens(Token * tokens, FILE* functions, FILE* main, int token_count) {
    // declaring a variable for our function type:
    // defines if the funtion being parsed will return a value or not
    // a marker for our position in the token array
    int pos = 0;
    bool isfunctionbody = false;


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

    while (pos < token_count) {
        switch (tokens[pos].type) {
            case TOKEN_END: {
                pos++;
                break;
            }
            case TOKEN_FUNCTION: {
                if (isfunctionbody){
                    fprintf(functions, "}\n");
                    isfunctionbody = false;
                }
                FunctionType f_type;
                f_type = check_function_type(tokens, &pos);
                pos++;
                printf("state = %d", f_type);
                build_fheader(tokens, f_type, &pos, functions);
                isfunctionbody = true;
                break;
            }
            case TOKEN_INDENT: {
                if (isfunctionbody){
                    pos++;
                    build_fbody(tokens, functions, &pos);                        
                }
                break;
            }
            case TOKEN_IDENTIFIER: {
                if (isfunctionbody){
                    build_assignment(tokens, functions, &pos);
                }
                else {
                    build_assignment(tokens, main, &pos);
                }
                break;
            }
            case TOKEN_PRINT: {
                if (isfunctionbody){
                    build_print(tokens, functions, &pos);
                }
                else{
                    build_print(tokens, main, &pos);
                    
                }
                pos++;
                break;
            }
            default: {
                break;
            }
        }
    }
    if (isfunctionbody && tokens[pos].type != TOKEN_INDENT && tokens[pos].type != TOKEN_RETURN && tokens[pos].type != TOKEN_FUNCTION){
        fprintf(functions, "}\n");
        isfunctionbody = false;
    }
}

const FunctionType check_function_type(Token *tokens, int *pos) {
    // defining an int for the position in the array
    int i = *pos;
    while (true) {
        if (tokens[i].type == TOKEN_END && tokens[i+1].type != TOKEN_INDENT) {
            break;
        }
        if (tokens[*pos].type == TOKEN_INDENT && tokens[*pos+1].type == TOKEN_RETURN) {
            if (tokens[*pos+2].type == TOKEN_END) {
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

void build_fheader(Token * tokens, FunctionType f_type, int *pos, FILE* fout) {
    if (tokens[*pos].type != TOKEN_IDENTIFIER) {
        fprintf(stderr, "!Trying to define function without an identifier");
        // i want this to scan through the funtion until the last line of body and
        // just skip it all

        // break out of the loop when we dont find an indent
        while (true) {
            if (tokens[*pos].type == TOKEN_END && tokens[*pos+1].type != TOKEN_INDENT) {
            (*pos)++;
            return;
            }
            (*pos)++;
        }
    }
    if (f_type == VOID) {
        fprintf(fout, "void ");
        fprintf(fout, "%s", tokens[(*pos)].value);
        (*pos)++;
        fprintf(fout, "(double ");
        while (tokens[*pos].type != TOKEN_END) {
            fprintf(fout, "%s", tokens[*pos].value);
            if (tokens[*pos+1].type != TOKEN_END) 
            {
                fprintf(fout, ", double ");
            }
            (*pos)++;
        }
        fprintf(fout, ") {\n");
    }
    if (f_type == NUM) {
        fprintf(fout, "double ");
        fprintf(fout, "%s", tokens[*pos].value);
        (*pos)++;
        fprintf(fout, "(double ");
        while (tokens[*pos].type != TOKEN_END) 
        {
            fprintf(fout, "%s", tokens[*pos].value);
            if (tokens[*pos+1].type != TOKEN_END) 
            {
                fprintf(fout, ", double ");
            }
            (*pos)++;
        }
        fprintf(fout, ") {\n");
    }
}

void build_fbody(Token * tokens, FILE* functions, int * pos) {
    while (tokens[*pos].type == TOKEN_INDENT){
        if (tokens[*pos].type == TOKEN_PRINT) {
            build_print(tokens, functions, pos);
        }
        if (tokens[*pos].type == TOKEN_RETURN) {
            build_return(tokens, functions, pos);
        }
        if (tokens[*pos].type == TOKEN_IDENTIFIER) {
            build_assignment(tokens, functions, pos);
        }
        (*pos)++;
    }
    
}

void build_return(Token * tokens, FILE* fout, int * pos) {
    while (tokens[*pos].type != TOKEN_END) {
        fprintf(fout, "%s", tokens[*pos].value);
        (*pos)++;
    }
    fprintf(fout, ";");
}

void build_assignment(Token * tokens, FILE* fout, int *pos) {
    int count = 0;
    if (tokens[(*pos)+1].type == TOKEN_ASSIGNMENT && count < 100) {
        count++;
        fprintf(fout, "double ");
        fprintf(fout, "%s", tokens[*pos].value);
        fprintf(fout, " = ");
        // skipping over the assignment and identifier char
        *pos += 2;
        while (tokens[*pos].type != TOKEN_END) {
            fprintf(fout, "%s", tokens[*pos].value);
            (*pos)++;
        }
        fprintf(fout, ";\n");
    }
}

void build_print(Token *tokens, FILE* fout, int * pos) {
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
    int result = system("cc -std=c11 -o out out.c");
    system("chmod +x out");
    if (result !=0) {
        fprintf(stderr, "Error in compilation \n");
        exit(EXIT_FAILURE);
    }
    
}

void execution(){
    int result = system("./out");
    system("./out");
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
    while(fgets(buffer, sizeof(buffer), functions) != NULL) {
        printf("%s\n", buffer);        
        fprintf(fout, "%s", buffer);
    }
    fprintf(fout, "\n\nint main(void) {\n");
    while(fgets(buffer, sizeof(buffer), main) != NULL) {
        printf("%s\n", buffer);
        fprintf(fout, "%s", buffer);
    }
    fprintf(fout, "return 0;\n}\n");

    // fclose();
}

int main(void) {
    // filename added for testing (mod when you want to change sources)
    char filename[] = "program1.ml";
    char PLACEHOLDER[] = "filename";

    
    // allocating a pointer to our tokens array
    Token* tokens = malloc(MAX_TOKENS * sizeof(Token));
    int token_count = 0;
    
    
    // file pointer to our file in read mode
    FILE* infile = fopen(filename, "r");
    if (infile == NULL) 
    {
        fprintf(stderr, "!File: %s could not be opened.", PLACEHOLDER);
    }

    // buffer for our line to sit in
    char line[BUFSIZ];
    
    // get lines sequentially
    while (fgets(line, sizeof(line), infile) != NULL) 
    {
        // comment check, get tokens for token array
        if (not_comment(line)) 
        {
            get_line_tokens(line, tokens, &token_count);
        }
    }
    

    // file pointers for our outputted c code
    FILE* fout = fopen("out.c", "w");
    FILE* functions = fopen("functions.c", "w");
    FILE* main = fopen("main.c", "w");

    // parsing the tokens in the tokens array
    parse_tokens(tokens, functions, main, token_count);
    
    fclose(functions);
    fclose(main);

    // free our allocated memory
    free(tokens);
    
    // opening the 
    write_to_out(fout);

    compiler();
    execution();
   // remove_files();
}