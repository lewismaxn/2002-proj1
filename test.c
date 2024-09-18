#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

typedef struct {
    char value[100];
} Token;

void build_print(Token *tokens, int length) {
    FILE* fout = fopen("out.c", "w");
    fputs("double printout =", fout);
    
    for (int i = 1; i < length; i++) {
        fputs(" ", fout);
        fputs(tokens[i].value, fout);
    }
    fputs(";\n", fout);

    const char *printing = "if ((printout - (int)printout) == 0) {printf(\"%d\", printout);} else {printf(\"%lf\", printout);}";
    
    fputs(printing, fout);

    fclose(fout);
}

int main(void) {
    Token tokens[] = {
        {"print"},
        {"3"},
        {"+"},
        {"4"},
        {"+"},
        {"5"}
    };

    int length = 6;

    build_print(tokens, length);
}