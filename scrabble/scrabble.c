#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // TODO: Print the winner
    printf("Player 1 score: %i\n", score1);
    printf("Player 2 score: %i\n", score2);
    if (score1 > score2)
    {
        printf("Player 1 Wins!\n");
    }
    else if (score2 > score1)
    {
        printf("Player 2 Wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}

int compute_score(string word)
{
    // Storing the score
    int score = 0;
    // getting length of word and iterating throught the string
    for (int i = 0, length = strlen(word); i < length; i++)
    {
        // Putting the character to uppercase and getting the score for the character
        char c = toupper(word[i]);
        if (c >= 'A' && c <= 'Z')
        {
            c = POINTS[c - 65];
        }
        else
        {
            c = 0;
        }

        score = score + c;
    }
    return score;
}
