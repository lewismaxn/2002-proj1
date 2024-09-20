#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_TOKENS 10000
#define MAX_TOKEN_VALUE 100
#define MAX_ERROR 1000

typedef enum {
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
    Token token;
    int pos = 0;

    token = get_next_token(line, &pos);
    while (token.type != TOKEN_END) {
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
            
            // VELJKO

            // return error to stderr

            //ERROR CHECK HERE

            printf("Identifiers must be 12 alphanumeric characters");
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
void build_fbody(Token * tokens);
void build_print(Token * tokens, FILE* fout, int * pos);
const FunctionType check_function_type(Token * tokens, int * pos);

void parse_tokens(Token * tokens, FILE* fout, int token_count) {
    // declaring a variable for our function type:
        // defines if the funtion being parsed will return a value or not
    FunctionType f_type;
    // a marker for our position in the token array
    int pos = 0;
    // check the token type at the position in the array
    while (pos < token_count) {
        // check for function, assignment or print
        switch (tokens[pos].type) {
            case TOKEN_END: {
                pos++;
            }
            case TOKEN_FUNCTION: {
                f_type = check_function_type(tokens, &pos);
                pos++;
                build_fheader(tokens, f_type, &pos, fout);
                break;
            }
            case TOKEN_INDENT: {
                build_body(tokens, &pos);
            }
            case TOKEN_IDENTIFIER: {
                build_assignment(tokens, &pos, fout);
                break;
            }
            case TOKEN_PRINT: {
                pos++;
                build_print(tokens, fout, &pos);
                break;
            }
            default: {
                break;
            }
        }
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
            return NUM;
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
        fprintf(fout, "(double ");
        while (tokens[*pos].type != TOKEN_INDENT) {
            fprintf(fout, ", double ");
            fprintf(fout, "%s", tokens[*pos].value);
        }
        fprintf(fout, ") {\n");
    }
    if (f_type == NUM) {
        fprintf(fout, "double ");
        fprintf(fout, "%s", tokens[*pos].value);
        fprintf(fout, "(double ");
        while (tokens[*pos].type != TOKEN_INDENT) {
            fprintf(fout, ", double ");
            fprintf(fout, "%s", tokens[*pos].value);
        }
        fprintf(fout, ") {\n");
    }
}

void build_assignment(Token * tokens, int *pos, FILE* fout) {
    if (tokens[*pos+1].type == TOKEN_ASSIGNMENT) {
        fprintf(fout, "double ");
        fprintf(fout, "%s", tokens[*pos].value);
        fprintf(fout, " = ");
        // skipping over the assignment and identifier char
        *pos += 2;
        while (tokens[pos].type != TOKEN_END) {
            fprintf(fout, "%s", tokens[*pos].value)
            *pos++;
        }
    }
}

void build_print(Token *tokens, FILE* fout, int * pos) {
    fputs("double printout =", fout);
    while (tokens[*pos].type != TOKEN_END) {
        fputs(" ", fout);
        fputs(tokens[*pos].value, fout);
        *pos += 1;
    }
    fputs(";", fout);
    char printing[] = "if ((printout - (int)printout) == 0) {\nprintf(\"%d\", (int)printout);\n}\nelse {\nprintf(\"%lf\", printout);\n}";
    fputs(printing, fout);
}

int main(void) {
    // filename added for testing (mod when you want to change sources)
    char filename[] = "program1.ml";
    // buffer for the line to sit in while we play with it
    char line[BUFSIZ];
    // counter for the number of tokens per line
    int token_count = 0;
    // allocating some memory for our tokens to sit in
    // define pointer to this block
    Token* tokens = malloc(MAX_TOKENS * sizeof(Token));
    // file pointer to our file in read mode
    FILE* infile = fopen(filename, "r");
    FILE* fout = fopen("out.c", "w");

    int line_count = 0;

    // grab a line from our file
    while (fgets(line, sizeof(line), infile) != NULL) {
        // this is for debugging (showing the origional line string)
        printf("line: %s\n", line);
        // check that the line is not a comment
        if (not_comment(line)) {
            // get all the tokens from the line
            // the tokens will be appended in the token array
            get_line_tokens(line, tokens, &token_count);
        }
        line_count++;
    }
    
    // parse_tokens(tokens, fout, token_count, line_count);

    print_tokens(tokens, token_count);

    parse_tokens(tokens, fout, token_count);

    // we are legendary programmers BRO
    free(tokens);
}