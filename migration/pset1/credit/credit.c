#include <stdio.h>
#include <cs50.h>

int main(void)
{
    
    // Getting the user input
    long credit_number = get_long("Input your cretit-card number\n");
    
    // Setting up the variables
    long every_first_number = 0;
    long every_second_number = 0;
    long current = credit_number;
    int valid_num = 0;
    string card;
    
    // Setting up a few perimeters for the card types(This is American express)
    if (((current > 339999999999999) && (current < 350000000000000)) || 
        ((current > 369999999999999) && (current < 380000000000000)))
    {   
        // Assigning the card type to a variable
        card = "AMEX";
        
        // The do while loop makes sure that i iterate through every number and stops when i have no more left
        do
        {
            
            
            every_first_number = every_first_number + current % 10; // taking the last number from the card and adding it to a variable
            current = current / 10; // making sure to take the last number out of the credit card and assigning the rest of it to a variable
            long every_second_number_temp = (current % 10) * 2; // taking the second to last number, multiplying by 2 and adding it to a temperory variable
            
            // Function to check if the second number is a double digits or single digit
            if (every_second_number_temp > 9)
            {
                // adding the double digit with the single digit
                every_second_number = every_second_number + every_second_number_temp % 10 + every_second_number_temp / 10; 
            }
            // if a single digit it goes through a normal adding to a variable
            else
            {
                every_second_number = every_second_number + every_second_number_temp;

            }
            // Taking the last number out of the credit card and adding the rest to a variable
            current = current / 10;
        }
        while (current > 0);
        // Fianally adding the numbers together When the loop finishes and check if it cleanly divides by 0
        valid_num = (every_first_number + every_second_number) % 10;
    }
    // Pretty much doing the same thing with all the statements but assigning the cards with a different type and the parimeters correspond with the card types
    else if ((current > 5099999999999999) && (current < 5600000000000000)) 
    {   
        
        card = "MASTERCARD";
        
        do
        {
            
            every_first_number = every_first_number + current % 10;
            current = current / 10;
            long every_second_number_temp = (current % 10) * 2;
            
            if (every_second_number_temp > 9)
            {
                every_second_number = every_second_number + every_second_number_temp % 10 + every_second_number_temp / 10;
            }
            else
            {
                every_second_number = every_second_number + every_second_number_temp;
            }
            current = current / 10;
        }
        while (current > 0);
        
        valid_num = (every_first_number + every_second_number) % 10;
        
    }
        
    else if (((current > 3999999999999) && (current < 5000000000000)) || 
             ((current > 3999999999999999) && (current < 5000000000000000)))
    {   
    
        card = "VISA";
    
        do
        {
            
            every_first_number = every_first_number + current % 10;
            current = current / 10;
            long every_second_number_temp = (current % 10) * 2;
            
            if (every_second_number_temp > 9)
            {
                every_second_number = every_second_number + every_second_number_temp % 10 + every_second_number_temp / 10;
            }
            else
            {
                every_second_number = every_second_number + every_second_number_temp;
            }
            current = current / 10;
            
        }
        while (current > 0);
        
        valid_num = (every_first_number + every_second_number) % 10;
        
    }
    // If the input doesnt correspond with the parameters then i will assign valid_num to a number i know is going to return a Invalid response
    else
    {
        valid_num = 1;
    }
    
    
    // checking if the card is valid and putting in the card type if it is
    if (valid_num == 0)
    {
        printf("%s\n", card);
    }
    else
    {
        printf("INVALID\n");
    }
    
    
}