# This will compile your out.c file with gcc
gcc -std=c11 -Wall -Werror -o out out.c

# Check if the compilation was successful
if [ $? -eq 0 ]; then
    # If successful, run the executable
    ./out
else
    # If there is an error, print the error message
    echo "Compilation failed"
fi
