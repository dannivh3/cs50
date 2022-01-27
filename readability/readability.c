//getting libraries
#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

//declaring functions
float get_average_100(int x, int y);
int get_grade_level(string text);
int grade_formula(float x, float y);

int main(void)
{
        //getting string and working it
        string text = get_string("Text: ");
        int grade = get_grade_level(text);

        if (grade < 1)
        {
                printf("Before Grade 1\n");
        }
        else if (grade >= 16)
        {
                printf("Grade 16+\n");
        }
        else
        {
                printf("Grade %i\n", grade);
        }
}

int get_grade_level(string text)
{
        // counter variables
        int letter_count = 0;
        int word_count = 1; // I need to start at 1 becouse the first word of the string is never counted
        int sentence_count = 0;
        //iterating through every letter in string
        for (int i = 0, l = strlen(text); i < l; i++)
        {
                char x = text[i];
                //checking if letter, word or sentence with specific characters
                if ((x >= 'A' && x <= 'Z') || (x >= 'a' && x <= 'z'))
                {
                        letter_count++;
                }
                else if (x == '!' || x == '?' || x == '.')
                {
                        sentence_count++;
                }
                else if (x == ' ')
                {
                        word_count++;
                }
                else
                {
                        ;
                }
        }
        // Putting the counters in some functions
        float average_letters = get_average_100(letter_count, word_count);
        float average_sentences = get_average_100(sentence_count, word_count);
        // Putting the results in a formula to check the grade level
        float grade_level = grade_formula(average_letters, average_sentences);

        return grade_level;
}
// Average function
float get_average_100(int x, int y)
{
        return ((float)x / y) * 100;
}
// Grade formula function
int grade_formula(float x, float y)
{
        return round(0.0588 * x - 0.296 * y - 15.8);
}
