#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_TOKENS 1000
#define MAX_TOKEN_VALUE 10000

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



void print_tokens(Token * tokens, int token_count);
void modify_tokens(Token * tokens, int token_count);
void add_token(Token * tokens, char* string, int *token_count);



int main(void) {
    char filename[] = "program.ml";
    char line[BUFSIZ];
    int token_count = 0;

    // pointer to an array with 1000 tokens
    Token *tokens = malloc(MAX_TOKENS * sizeof(Token));

    FILE* infile = fopen(filename, "r");
    

    while (fgets(line, sizeof(line), infile) != NULL) {
        add_token(tokens, line, &token_count);
    }

    print_tokens(tokens, token_count);

    return 0;
}

void print_tokens(Token * tokens, int token_count) {
    printf("Tokens array is at: %zu\n", tokens);

    for (int i = 0; i < token_count; i++){
        printf("Token type: %-15dToken value: %s\n", tokens[i].type, tokens[i].value);
    }
}

void modify_tokens(Token * tokens, int token_count) {
    printf("modifying tokens...\n");

    for (int i = 0; i < token_count; i++) {
        tokens[i].type = (i * 2);
        sprintf(tokens[i].value, "value %d", (i+3));
    }
}

void add_token(Token * tokens, char* string, int *token_count) {
    tokens[*token_count].type = TOKEN_NUMBER;
    sprintf(tokens[*token_count].value, "value %d: %s", *token_count, string);
    (*token_count)++;
}