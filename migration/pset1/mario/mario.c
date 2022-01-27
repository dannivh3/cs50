#include <stdio.h>
#include <cs50.h>

int main(void)
{
    
    // Setting up variables
    int height;
    
    // Setting perimeters for input
    do
    {
        height = get_int("How high do you want the pyramid?\n");
    }
    while (height < 1 || height > 8);
    
    // Making a for loop for the height input, Each height is a new line of string
    for (int i = 0; i < height; i++)
    {
        // Making a nested for loop, there needs to be at least 4 characters up to 16 characters,
        for (int j = 0; j < 3 + height + i; j++)
        {
            // Setting up perimeters for printed results, if the iteration is not the desired results it will print out "#"
            if (j == height || j == height + 1 || i + j < height - 1)
            {
                printf(" ");
            }
            else
            {
                printf("#");
            }
        
        }
        printf("\n");
    }
}

