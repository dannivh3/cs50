// Get Libraries
#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int main(int argc, string argv[])
{
    if (argc == 2)
    {
        // Now is a good time to assign a var to the string and get the length of the string
        string key = argv[1];
        int key_len = strlen(key);
        char key_array[key_len];

        if (key_len == 26)
        {
            // Iterating through characters in the key string and if valid they get stored in an array
            for (int i = 0, l = key_len; i < l; i++)
            {
                char c = key[i];
                if (isalpha(c))
                {

                    for (int j = 0; j < 0 + i; j++)
                    {

                        if (toupper(c) == key_array[j] || tolower(c) == key_array[j])
                        {
                            printf("Key has duplicate characters");
                            return 1;
                        }
                    }
                    key_array[i] = c;
                }
                else
                {
                    printf("Key must only contain alphabetical characters.\n");
                    return 1;
                }
            }
        }
        else
        {
            printf("Key must contain 26 characters.\n");
            return 1;
        }
        // Time to cypher a plaintext
        string plaintext = get_string("plaintext:  ");
        printf("ciphertext: ");
        char cyphertext[strlen(plaintext)];
        // Iterating through every letter in plaintext
        for (int i = 0, l = strlen(plaintext); i < l; i++)
        {
            char c = plaintext[i];
            // checking what character c is and working it
            if (c >= 'a' && c <= 'z')
            {
                int char_dec = c - 97; // Just subtracting the ascii decimal point so i get the right array index
                c = key_array[char_dec];
                c = tolower(c);
                cyphertext[i] = c;
            }
            else if (c >= 'A' && c <= 'Z')
            {
                int char_dec = c - 65;
                c = key_array[char_dec];
                c = toupper(c);
                cyphertext[i] = c;
            }
            // if its not a letter just leave it as is
            else
            {
                cyphertext[i] = c;
            }
            // printing out the character without leaving newLine
            printf("%c", cyphertext[i]);
        }
        printf("\n");
    }
    else
    {
        printf("Usage: ./subsitution key\n");
        return 1;
    }
}

