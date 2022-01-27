#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // setting up the variables
    int starting_llamas = 0;
    int ending_llamas = 0;
    int years = 0;
    // Asking for current llama population inputs 9 or higher
    do
    {
        starting_llamas = get_int("What is the starting llama population\n");
    }
    while (starting_llamas < 9);
    // Asking for desired llama population input higher than the starting population
    do
    {
        ending_llamas = get_int("What is your desired llama population\n");
    }
    while (ending_llamas < starting_llamas);
    // Making a calculation for how many years it takes to reach the desired population
    while (ending_llamas > starting_llamas)
    {
        starting_llamas += starting_llamas / 3 - starting_llamas / 4;
        years++; // adding 1 year for each iteration
    }
    // returning the years
    printf("Years: %i\n", years);
}