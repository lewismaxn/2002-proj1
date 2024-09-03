#This file is just a test file to pass into the reader to check outputs



#ex.1
x <- 2.3

#ex.2
x <- 2.5
print x

#ex.3
print 3.5

#ex.4
x <- 8
y <- 3
print x * y

#ex.5
function printsum a b
	print a + b
printsum (12, 6)

#ex.6
function multiply a b
	return a * b
print multiply(12, 6)

#ex.7
function multiply a b
	x <- a * b
	return x
print multiply(10, 5)

#ex.8
one <- 1
function increment value
	return value + one
print increment(3) + increment(4)