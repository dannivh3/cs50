#include <stdio.h>
#include <cs50.h>

int main(void)
{
    // Get user input
    string name = get_string("What is your name?\n");
    // Join a string and the var together and print out
    printf("hello, %s\n", name);
}
