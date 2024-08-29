# 2002-proj1


## Project Discription

We're all very familiar with higher-level programming languages, such as Python, Java, and C. They have many well-defined features, and are supported by standard libraries and modules. We'll term these large-languages.

At the other end of the scale are mini-languages, that are often embedded in other programs that don't require the support of a full programming language, or are invoked from the command-line or in a shellscript. You could consider the use of macros in MS-Excel, or the Unix command-line program bc (see man bc), as examples of a mini-languages. Chapter 8 of The Art of Unix Programming provides an overview of some (older) mini-languages [not required reading].

This project requires you write a C11 program to compile and execute a program written in a mini-language that we'll name ml. Note that there already exists a very successful programming language named ML (for Meta Language), but our mini-language is unrelated to ML (or to Machine Learning).

Writing a compiler for any programming language is an enormous task, clearly not one suited for this project. However, what many have recognised, is that C is an excellent language to support other languages, and that C has an extensive toolchain supporting compilation and linking. The strategy is to first translate programs written in other languages, such as our ml, to C, to compile that translated C code using a standard C compiler, and to finally execute the resultant program. This sequence is often termed transpiling, the 'joining' of the words translating and compiling. In this role, C is often described as a high-level assembly language, sometimes a 'wallpaper language'.

The goal of this project is to implement a C11 program, named runml, which accepts a single command-line argument providing the pathname of a text file containing a program written in ml, [added 28/8] and any optional command-line arguments to be passed the transpiled program when it is executed . Successful execution of runml will involve checking the syntax of the ml program, translating that valid ml program to a C11 program, compilation of the resultant C program and, finally, execution of the compiled program.


## Our ml language

the syntax of ml programs: samples found in syntax_samples.md
1. programs are written in text files whose names end in .ml
2. statements are written one-per-line (with no terminating semi-colon)
3. the character '#' appearing anywhere on a line introduces a comment which extends until the end of that line
4. only a single datatype is supported - real numbers, such as 2.71828
5. identifiers (variable and function names) consist of 1..12 lowercase alphabetic characters, such as budgie
6. there will be at most 50 unique identifiers appearing in any program
7. variables do not need to be defined before being used in an expression, and are automatically initialised to the (real) value 0.0
8. the variables arg0, arg1, and so on, provide access to the program's command-line arguments which provide real-valued numbers
9. a function must have been defined before it is called in an expression
10. each statement in a function's body (one-per-line) is indented with a tab character
11. functions may have zero-or-more formal parameters
12. a function's parameters and any other identifiers used in a function body are local to that function, and become unavailable when the function's execution completes
13. programs execute their statements from top-to-bottom and function calls are the only form of control-flow (yes, the language would be more useful with loops and conditions, but this project is not about designing programming languages - future work for those interested)

## Project Requirements

1. Your project must be written in the C11 programming language, in a single source-code file named runml.c
2. Your project must perform as a standard utility program - checking its command-line arguments, displaying a usage message on error, printing 'normal' output to stdout and error messages to stderr, terminate with a exit status reflecting its execution success.
3. Your project must not depend upon any libraries (such as from 3rd-party, downloaded from the internet) other than the system-provided libraries (providing OS, C11, and POSIX functions).
4. All syntax errors detected in invalid ml programs must be reported via stderr on a line commencing with the '!' character. Your runml program must be able to detect all invalid ml programs - EXCEPT that your program will not be tested with any invalid expressions, so you do not need to validate the syntax of expressions.
5. The only 'true' output produced by your translated and compiled program (when running) is the result of executing ml's print statement. Any 'debug' printing should appear on a line commencing with the '@' character.
6. When printed, numbers that are exact integers must be printed without any decimal places; other numbers must be printed with exactly 6 decimal places.
7. The project can be successfully completed without using any dynamic memory allocation in C (such as with malloc()). You may choose to use dynamic memory allocation, but will not receive additional marks for doing so.

