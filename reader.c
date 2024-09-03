#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

int reader(char[]);
bool decomment(char[]);
int parse(char[]);
void functiondef(char[]);
void printout(char[]);
void assign(char []);



static FILE* in_fp;
static FILE* out_fp;


int main(int argc, char *argv[])
{
    //currently will take 1 command line argument, will expect a filename
    if (argc != 2) 
    {
        printf("Usage of %s: %s takes 1 command line argument.", argv[0], argv[0]);
        exit(EXIT_FAILURE);
    }
    
    in_fp = fopen(argv[1], "r");
    out_fp = fopen("source.c", "w");
    //  pass filename to a reader function
    reader(argv[1]);
}


//do we want to make our reader read and process line by line
// take a line and perform a translation on that line?
// if so, do we write to a c source file and compile that?

int reader(char file[]) 
{
    // buffer for line to live in while processing
    char line[BUFSIZ];
    int count = 0;

    // declare an input file pointer for the file declared in argument
    FILE *fptr = fopen(file, "r");
    // while a line != null, read a line and perform an operation
    while (fgets(line, sizeof(line), fptr) != NULL) 
    {
        // if comment, will evaluate false
        if (decomment(line))
        {
            // parse any lines that aren't comments
            parse(line);
        }
    }
}


// check the first char in line for comment indicator
bool decomment(char line[]) 
{
    return (line[0] != '#');
}

int parse(char line[]) 
{
    // find a string within a substring
    // looking for a single word in the array
    // return statements will only be inside a function def
    // anything else can be assumed to be an assignment??

    // I dont like this method
    
    if (strstr(line, "function ") != NULL) 
    {
        functiondef(line);
    }
    else if (strstr(line, "print ") != NULL) 
    {
        printout(line);
    }
    else 
    {
        assign(line);
    }
    return 0;
}


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