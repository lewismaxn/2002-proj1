#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_TOKENS 1000
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

typedef struct {
    TokenType type;
    char value[MAX_TOKEN_VALUE];
} Token;


void get_tokens(char* line, Token **tokens, int *token_count);
Token get_next_token(char* line, int* pos);
bool not_comment(char* line);



int main(void) {
    char filename[] = "program.ml";
    char line[BUFSIZ];
    int *token_count = 0;
    Token *tokens = malloc(MAX_TOKENS * sizeof(Token));

    FILE* infile = fopen(filename, "r");

    while (fgets(line, sizeof(line), infile) != NULL) {
        printf("line: %s", line);
        get_tokens(line, &tokens, &token_count);
    }
}

void get_tokens(char *line, Token **tokens, int *token_count) {
    Token token;
    int token_count = 0;
    int pos = 0;

    while (not_comment(line)) {
        token = get_next_token(line, &pos);

        while (token.type != TOKEN_END) {
            (*tokens)[*token_count] = token;
            token = get_next_token(line, &pos);
            token_count++;
        }
    }
    (*tokens)[*token_count] = token;

    printf("Printing tokens:\n");
    for (int i = 0; i < token_count; i++) {
        printf("Token %d:\nType: %d\nValue: %s\n", i, (*tokens)[i].type, (*tokens)[i].value);
    }
}

Token get_next_token(char* line, int* pos) {
    Token token;
    
    // we are going to be skipping whitespaces so lets check tabs
    if (*pos == 0 && line[*pos] == '\t') {
        // we can add a counter for the number of tabs here
        while (line[*pos] == '\t') {
            (*pos)++;
        }
        strcpy(token.value, "INDENT");
        token.type = TOKEN_INDENT;
        return token; //returning to avoid reassignment
    }
    
    while (isspace(line[*pos])) (*pos)++;  // Skip whitespace

    // checking for alphanumeric tokens (identifiers and keywords)
    if (isalpha(line[*pos])) {
        // identify the start point of the tokenizer
        int start = *pos;
        // iterate as long as we have an alphanumeric char
        // this will give us a postion along the array
        while (isalnum(line[*pos])) (*pos)++;        
        // copy
        strncpy(token.value, &line[start], *pos - start);
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
    else if (isdigit(line[*pos])) {  // Numbers
        int start = *pos;
        while (isdigit(line[*pos]) || line[*pos] == '.') (*pos)++;
        strncpy(token.value, &line[start], *pos - start);
        token.value[*pos - start] = '\0';
        token.type = TOKEN_NUMBER;
    } 
    // check now for assignment: if the less than char is found check
    // for a - next and if found then its an assignment
    else if (line[*pos] == '<' && line[*pos + 1] == '-') { 
        (*pos) += 2;
        token.type = TOKEN_ASSIGNMENT;
        strcpy(token.value, "<-");
    }
    // check for an operator and copy into token.value
    else if (line[*pos] == '+' || line[*pos] == '-' || line[*pos] == '*' || line[*pos] == '/') {  // Operators
        token.value[0] = line[*pos];
        token.value[1] = '\0';
        token.type = TOKEN_OPERATOR;
        (*pos)++;
    }

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

    else if (line[*pos] == '\0') {
        token.type = TOKEN_END;
    } 
    // finally, if nothing makes sense we say that 
    else {
        printf("Unexpected character: %c\n", line[*pos]);
        exit(1);
    }

    return token;
}

bool not_comment(char * line) {
    return (line[0] != '#');
}