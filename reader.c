#include <stdio.h>
#include <stdlib.h>




int main(int argc, char *argv[])
{
    //currently will take 1 command line argument, will expect a filename
    if (argc != 2) 
    {
        printf("Usage of %s: %s takes 1 command line argument.", argv[0], argv[0]);
        exit(EXIT_FAILURE);
    }
    //  pass filename to a reader function
    reader(argv[1]);
}


//do we want to make our reader read and process line by line
// take a line and perform a translation on that line?
// if so, do we write to a c source file and compile that?

int reader(char * file[]) 
{
    // buffer for line to live in while processing
    char line[BUFSIZ];
    int count = 0;

    // declare an input file pointer for the file declared in argument
    FILE *fptr = fopen(file, "r");
    // while a line != null, read a line and perform an operation
    while (fgets(line, sizeof(line), fptr) != NULL) 
    {
        parse(line);
    }
}


int parse(char * line[]) 
{
    if (line[0] != '#') 
    {
        return 1;
    }
    
}