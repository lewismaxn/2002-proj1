# file that should have the worst possible statement set

#assignment of variables
x <- 4.56
y <- 3

function addthree (x, y, z)
    # assignment
    a <- x + y + z    
    return a

function printaddthree (x, y, z)
    # assigning a function result to a variable
    print addthree(x, y, z)

<!-- NOTE:
Here just worth noting that we can have both returning functions and
void functions, here addthree will return a value whereas printaddthree
will just print without having an output

also noting that variables must be local within these functions -->

z <- 5.32

function allpossible (x, y, z)
    #assignment
    a <- x + y
    #print
    print a
    #function call
    a <- a + addthree(x, y, z)
    printaddthree(x, y, z)
    #return
    return a

print allpossible(x, y, z)

