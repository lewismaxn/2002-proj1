## Assumption:

We will not see a function call before the function is defined (Prototypes arent required)

Variables can only be numbers ===> return can only return a number

a float cant be bigger than the allowable float size limit

variables cannot be reassigned



Problem:
    void or number

void function(a, b) {
    some stuff
    NO RETURN
}

double function(a, b) {
    return number
}

Do we check for the last indent for the last line of function body?

EX:
function random a b
	a <- 6.7
	b <- 7.4
	c <- a + b
	print c

in c:

void random(a,b) {
    double a = 6.7;
    double b = 7.4;
    c = a + b;
    printf("%d", c);
}

VS 

EX:
function increment value
    print value
	return value + one

double increment(double value) {
    if (typeof(value) == double)
        printf("%lf", value);
    else
        printf("%d", value);
    return (value += one);
}

Do we take tokens and put into another array
add lines until we find an indent
look for a return statement and then define double


## Convention?

add () to any return or print statement to account for evaluation



## printing function calls

when printing we need to evaluate what comes after the print statement before printing
then check the type before printing

for example:

say we have print multiply(12, 6)

we take the expression after print and put that in brackets to evaluate it
and assign that to a double variable:

double printout = (multiply(12, 6))

then do a check:

if ((printout - (int)printout) == 0) {
    printf("%d", printout);
}
else {
    printf("lf", printout);
}


same again for another statement

print multiply(3.4, 4.5) * 6

double printout = multiply(3.4, 4.5) * 6

if ((printout - (int)printout) == 0) {
    printf("%d", printout);
}
else {
    printf("lf", printout);
}

thats how prints should work?



issue with single function call statements