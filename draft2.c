#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_TOKENS 10000
#define MAX_TOKEN_VALUE 100

typedef enum {
    TOKEN_NUMBER,
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
            fprintf(stderr, "! Error: Identifiers must be 12 alphanumeric characters \n");
            }
            else {
                token.type = TOKEN_IDENTIFIER;
            }
    } 

    // check for a digit 
    else if (isdigit(line[*pos])) {  // Numbers
        int start = *pos;
        // if number continues or a dp is found then just increase position
        while (isdigit(line[*pos]) || line[*pos] == '.') (*pos)++;
        // copy the value as a string to the token.value
        strncpy(token.value, &line[start], *pos - start);
        token.value[*pos - start] = '\0';
        token.type = TOKEN_NUMBER;
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
        case TOKEN_NUMBER: return "TOKEN_NUMBER";
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
void build_fheader(Token * tokens);
void build_fbody(Token * tokens);
void build_print(Token * tokens);

void parse_tokens(Token * tokens, int *build_state, int token_count) {
    int pos = 0;
    while (pos < token_count) {
        switch (tokens[0].type) {
            case TOKEN_FUNCTION: {
                build_fheader(&tokens[pos]);
                (*build_state) = BUILDING_FUNCTION;
                pos++;
                break;
            }
            // case TOKEN_INDENT: {
            //     build_fbody(&tokens[pos]);
            //     build_state = BUILDING_BODY;
            //     pos++;
            //     break;
            // }
            // case TOKEN_PRINT: {
            //     build_print(&tokens[pos]);
            //     build_state = BUILDING_PRINT;
            //     pos++;
            //     break;
            // }
        }
    }
}


void build_fheader (Token * tokens) {
    printf("void %s(", tokens[1].value);
    int i = 2; 
    while (tokens[i].type != TOKEN_INDENT) { //because we will not come across an assignment in the header its just easier to do it like this so we can increment
        if (tokens[i].type == TOKEN_IDENTIFIER){
            i +=1;
            printf("int %s", tokens[i].value); 
            if (tokens[i].type == TOKEN_COMMA) {
                printf(",");
            }
        }
        i++;
    }
    printf(") {\n");
}



int main(void) {
    // filename added for testing (mod when you want to change sources)
    char filename[] = "program.ml";
    // buffer for the line to sit in while we play with it
    char line[BUFSIZ];
    // counter for the number of tokens per line
    int token_count = 0;
    // allocating some memory for our tokens to sit in
    Token *tokens = malloc(MAX_TOKENS * sizeof(Token));
    // file pointer to our file in read mode
    FILE* infile = fopen(filename, "r");

    // grab a line from our file
    while (fgets(line, sizeof(line), infile) != NULL) {
        // this is for debugging (showing the origional line string)
        printf("line: %s\n", line);
        // check that the line is not a comment
        if (not_comment(line)) {
            // get all the tokens from the line
            // the tokens will put in the tokens array
            get_line_tokens(line, tokens, &token_count);
        }

        // DEBUGGING
        // just printing all the tokens
        print_tokens(tokens, token_count);
        
        BuildState build = INITIAL;
        parse_tokens(tokens, &build, token_count);
        
        token_count = 0;
    }

    // we are legendary programmers BRO
    free(tokens);
}