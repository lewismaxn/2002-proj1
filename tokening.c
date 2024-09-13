#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

// Enum will assign a value for every token we are interested in
typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_ASSIGN,
    TOKEN_OPERATOR,
    TOKEN_PRINT,
    TOKEN_FUNCTION,
    TOKEN_RETURN,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_COMMA,
    TOKEN_END,
    TOKEN_INDENT
} TokenType;


// defining a struct for token
// including the type (through enum)
typedef struct {
    TokenType type;
    // value of the token (identifier name or keywork etc.)
    char value[100];
} Token;

// PROTOTYPES

Token get_next_token(char* line, int *pos);
void initialiser(void);
bool not_comment(char line[]);
const char* token_type_to_string(TokenType type);
void get_line_tokens(char *line, Token *line_tokens);


int main(void) {
    initialiser();
}

void initialiser(void) {
    //declaring a buffer for our line
    // declaring an int for our position to sit in
    // declaring a token variable for get_next_token
    char line[BUFSIZ];

    // file pointer for our file to read from
    // if fopen returns null then the file wasn't opened correctly
    FILE *infile = fopen("program.ml", "r");
    if (!infile) {
        perror("failed to open file");
        exit(1);
    }
    
    Token line_tokens[BUFSIZ];
    int line_no = 0;

    // checking if there is a newline available
    while (fgets(line, sizeof(line), infile) != NULL) {
        line_no++;
        get_line_tokens(line, &line_tokens);
        printf("line %d has the following tokens:", line_no);
        for (int a = 0; a < sizeof(line_tokens)/sizeof(Token); a++) {
            printf("Token Type = %25s, Value = %s\n", token_type_to_string(line_tokens[a].type), line_tokens[a].value);
        }
    }

    fclose(infile);
}

void get_line_tokens(char *line, Token *line_tokens) {
    // initialise the position in line to zero
        int pos = 0;
        int token_count = 0;
        Token token;

        // tokenise the line but first check for comment at the start of line
        while (not_comment(line)) {
            token = get_next_token(line, &pos);
            // if we find the token to be the \0 char when we break out
            if (token.type == TOKEN_END) break;
            line_tokens[token_count] = token;
            //printf("Token Type = %25s, Value = %s\n", token_type_to_string(token.type),
            // token.value);
        }
}

Token get_next_token(char *input, int *pos) {
    Token token;
    
    // we are going to be skipping whitespaces so lets check tabs
    if (*pos == 0 && input[*pos] == '\t') {
        // we can add a counter for the number of tabs here
        while (input[*pos] == '\t') {
            (*pos)++;
        }
        strcpy(token.value, "INDENT");
        token.type = TOKEN_INDENT;
        return token; //returning to avoid reassignment
    }
    
    while (isspace(input[*pos])) (*pos)++;  // Skip whitespace

    // checking for alphanumeric tokens (identifiers and keywords)
    if (isalpha(input[*pos])) {
        // identify the start point of the tokenizer
        int start = *pos;
        // iterate as long as we have an alphanumeric char
        // this will give us a postion along the array
        while (isalnum(input[*pos])) (*pos)++;        
        // copy
        strncpy(token.value, &input[start], *pos - start);
        token.value[*pos - start] = '\0';

        // check for a keyword first

        if (strcmp(token.value, "print") == 0)
            token.type = TOKEN_PRINT;
        else if (strcmp(token.value, "function") == 0)
            token.type = TOKEN_FUNCTION;
        else if (strcmp(token.value, "return") == 0)
            token.type = TOKEN_RETURN;
        
        // if a keyword isn't found its an identifier
        else
            // check for conditions of an identifier
            // - 12 lowercase letters
            if (*pos - start > 12) {
            
            //ERROR CHECK HERE

            printf("Identifiers must be 12 alphanumeric characters");
            }
            token.type = TOKEN_IDENTIFIER;
    } 
    // check for a digit 
    else if (isdigit(input[*pos])) {  // Numbers
        int start = *pos;
        while (isdigit(input[*pos]) || input[*pos] == '.') (*pos)++;
        strncpy(token.value, &input[start], *pos - start);
        token.value[*pos - start] = '\0';
        token.type = TOKEN_NUMBER;
    } 
    // check now for assignment: if the less than char is found check
    // for a - next and if found then its an assignment
    else if (input[*pos] == '<' && input[*pos + 1] == '-') { 
        (*pos) += 2;
        token.type = TOKEN_ASSIGN;
        strcpy(token.value, "<-");
    }
    // check for an operator and copy into token.value
    else if (input[*pos] == '+' || input[*pos] == '-' || input[*pos] == '*' || input[*pos] == '/') {  // Operators
        token.value[0] = input[*pos];
        token.value[1] = '\0';
        token.type = TOKEN_OPERATOR;
        (*pos)++;
    }

    else if (input[*pos] == '(') {
        token.type = TOKEN_LPAREN;
        strcpy(token.value, "(");
        (*pos)++;
    } 

    else if (input[*pos] == ')') {
        token.type = TOKEN_RPAREN;
        strcpy(token.value, ")");
        (*pos)++;
    }
    
    else if (input[*pos] == ',') {
        token.type = TOKEN_COMMA;
        strcpy(token.value, ",");
        (*pos)++;
    } 

    else if (input[*pos] == '\0') {
        token.type = TOKEN_END;
    } 
    // finally, if nothing makes sense we say that 
    else {
        printf("Unexpected character: %c\n", input[*pos]);
        exit(1);
    }

    return token;
}


bool not_comment(char line[]) 
{
    return (line[0] != '#');
}

const char* token_type_to_string(TokenType type) {
    switch (type) {
        case TOKEN_IDENTIFIER: return "TOKEN_IDENTIFIER";
        case TOKEN_NUMBER: return "TOKEN_NUMBER";
        case TOKEN_ASSIGN: return "TOKEN_ASSIGN";
        case TOKEN_OPERATOR: return "TOKEN_OPERATOR";
        case TOKEN_PRINT: return "TOKEN_PRINT";
        case TOKEN_FUNCTION: return "TOKEN_FUNCTION";
        case TOKEN_RETURN: return "TOKEN_RETURN";
        case TOKEN_LPAREN: return "TOKEN_LPAREN";
        case TOKEN_RPAREN: return "TOKEN_RPAREN";
        case TOKEN_COMMA: return "TOKEN_COMMA";
        case TOKEN_END: return "TOKEN_END";
        default: return "UNKNOWN_TOKEN";
    }
}