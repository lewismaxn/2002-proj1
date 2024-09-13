#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

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
    TOKEN_END
} TokenType;







int reader(char[]);
bool decomment(char[]);
void parse(char[]);
/*
void functiondef(char[]);
void printout(char[]);
void assign(char []);
*/







int main(int argc, char *argv[])
{
    /*currently will take 1 command line argument, will expect a filename
    if (argc != 2) 
    {
        printf("Usage of %s: %s takes 1 command line argument.", argv[0], argv[0]);
        exit(EXIT_FAILURE);
    }
    */
    // change to argv[1] when we are ready to come together
    
        //error
    // out_fp = fopen("source.c", "w");
    //  pass filename to a reader function
    reader("program.ml");
}


//do we want to make our reader read and process line by line
// take a line and perform a translation on that line?
// if so, do we write to a c source file and compile that?

// I think using tokenisation for this is better

int reader(char file[]) 
{
    // buffer for line to live in while processing
    char line[BUFSIZ];
    int count = 0;

    // declare an input file pointer for the file declared in argument
    FILE *fptr = fopen("program.ml", "r");
    // while a line != null, read a line and perform an operation
    printf("test");
    while (fgets(line, sizeof(line), fptr) != NULL) 
    {
        // if comment, will evaluate false
        if (decomment(line))
        {
            // parse any lines that aren't comments
            parse(line);
        }
    }
    fclose(fptr);
}


// check the first char in line for comment indicator
bool decomment(char line[]) 
{
    return (line[0] != '#');
}

void parse(char line[]) 
{   
    //declaring our "seperator" and an array for our token to go to
    char delimiter[] = " ";
    char* token;
    char* expression;

    //testing bullshit ignore
    printf("\ninitial string:\n%s\n", line);
    printf("after tokenisation: \n");

    // get first token from the string
    token = strtok(line, delimiter);
    identify(token);
    // if the string is finished strtok will return NULL
    // we check here that token is not null and loop until finished
    while (token) {
        printf("%s\n", token);
        // calling strtok subsqeuently will just use NULL for the string argument
        token = strtok(NULL, delimiter);
    }
}






/*

void functiondef(char line[]) {
    ;
}

void printout(char line[]) {
    ;
}

void assign(char line[])
{
    int i = 0;
    char var[13];

    // search for variable name and add to var array
    // if var name is bigger than 12 then abort
    while (line[i] != ' ' && i < 12)
    {
        if (islower(line)) {
        var[i] = line[i];
        ++i;
        } else {
            break;
        }
    }
    
    var[i] = '\0';

    if (i > 0) {
        fputs(var, out_fp);
    }
}

*/