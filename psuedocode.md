open the ml file in read mode
grab a line from the file
generate tokens from the file
go through every line in the file
build function headers when we find TOKEN_FUNCTION
    check for return in the function body 
        while nextline[0] = TOKEN_INDENT
            check nextline[1] for return
                if return
                    double function( arguments )
                else
                    void function ( arguments )
build function bodies when we find TOKEN_INDENT

when building lines we need to put them to fputs to write them to a c file

compile the final c file using gcc