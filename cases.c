// only includes we need for our ml file will be <stdio.h>? declaring that here
#include <stdio.h>

// implementing a prototype when a function is found
float addthree(float, float, float);
void printaddthree(float, float, float);
float allpossible(float, float, float);

// returning a float
float addthree(float x, float y, float z)
{
    float a = x + y + z;
    return a;
}

// void program as there is no return value
void printaddthree(float x, float y, float z)
{
    printf("%f", addthree(x, y, z));
}


float z = 5.32;


float allpossible(float x, float y, float z)
{
    float a = x + y;
    printf("%f", a);
    a = a + addthree(x, y, z);
    printaddthree(x, y, z);
    return a;
}




int main(void)
{
    // assignment of variables
    // do we call them all as they are declared?
    float x = 4.56;
    int y = 3;
    // checking for int status along assignments

    // assignment later in function
    float z = 5.32;

    printf("%.6f", allpossible(x, y, z));
}