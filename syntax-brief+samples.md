# _ml_ syntax
### note 

  |       means a choice
  
  [...]   means zero or one
  
  (...)*  means zero or more


# program:

           ( program-item )*

### means
lewis:
a program consists of one or more program item


# program-item:

           statement
        |  function identifier ( identifier )*
           ←–tab–→ statement1
           ←–tab–→ statement2
           ....

### means
lewis:
a program consists of one or more statements __or__ one or more function declaration


# statement:

           identifier "<-" expression
        |  print  expression
        |  return expression
        |  functioncall      

### means
lewis:
a statement can be:
1. an assignment of an expression to an identifier
2. a print operation
3. a return from a function
4. a function call


# expression:

          term   [ ("+" | "-")  expression ]

### means
lewis:
an expression consists of a term
a term _can_ be accompanied with addition __or__ subtraction of another expression


# term:

          factor [ ("*" | "/")  term ]

### means
lewis:
an term consists of a factor
the term _can_ be accompanied with multiplication __or__ division of another term
that term be a single factor or the chain can continue


# factor:

          realconstant
        | identifier
        | functioncall
        | "(" expression ")"

### means
lewis:
a factor is either:
1. a real constant - int or float
2. an identifier (variable)
3. a function call (returning a result)
4. an evaluated expression or chain of expressions within brackets
    a. expressions in brackets must be evaluated before passing as a factor


# functioncall:

          identifier "(" [ expression ( "," expression )* ] ")"

### means
lewis:
function calls are in form:
  identifier/name of function then __one or more expressions__ enclosed in brackets

valid functions:
  function1 (expression)
  function2 (expression, expression)
  function3 (expression, expression, ...)
  




# EXAMPLES

## EXAMPLE 1

_an assignment statement, nothing is printed_

x <- 2.3

### take away
lewis:
single line statements - look for newline char for division of statements
assignment syntax "<-"


## EXAMPLE 2

_an assignment statement, 2.500000 is printed_

x <- 2.5
print x

### take away
lewis:
any non int value has to be rounded to 6dp's
check for int status on statement read and check for difference on calculations?


## EXAMPLE 3

_3.500000 is printed_

print 3.5

### take away
lewis:
again 6dp printing on non-int values

## EXAMPLE 4

_24 is printed_

x <- 8
y <- 3
print x * y

### take away
lewis:
calculation must be done before function call


## EXAMPLE 5

_18 is printed_

function printsum a b
	print a + b
printsum (12, 6)

### take away
lewis:
function definition formatting
operation to be evaluated before print is performed


## EXAMPLE 6

_72 is printed_

function multiply a b
	return a * b
print multiply(12, 6)

### take away
lewis:
function must evaluate the function statement before printing
printing of type int must be with no dp's


## EXAMPLE 7

_50 is printed_

function multiply a b
	x <- a * b
	return x
print multiply(10, 5)

### take away
lewis:
assignment to variable within function
return as an int/float
we maybe pass a struct with form:
'''
  struct num:
    float value;
    bool int;
'''
with int as true when the number is an int


## EXAMPLE 8

_9 is printed_

one <- 1
function increment value
	return value + one
print increment(3) + increment(4)

### take away
lewis:
global variables must be accessable by the program during function definitions
expression evaluated before print
