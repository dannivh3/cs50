// Implements a dictionary's functionality
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <strings.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

// Size counter
unsigned int SIZE = 0;

// Returns true if word is in dictionary, else false
// Source: https://gist.github.com/srushtikanade/b0612f537a15e1265f624dd2d946a917
bool check(const char *word)
{
    // Assign variables
    int length =strlen(word);
    char copy[length+1];
    copy[length] = '\0';
    // set the copy to lowercase
    for (int i = 0; i < length; i++)
    {
        copy[i] = tolower(word[i]);
    }
    // get the index from hash function
    int index = hash(copy);
    // set the cursor to point at table
    node *cursor = table[index];

    if(cursor==NULL)
    {
        return false;
    }
    // Do untill cursor sees a NULL
    while (cursor != NULL)
    {
        // check case insesnitive
        if (strcasecmp(cursor->word, copy) == 0)
        {
            return true;
        }

            cursor = cursor->next;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // Source: http://www.cse.yorku.ca/~oz/hash.html Name: djb2. Author: dan bernstein

    unsigned long hash = 5381;
    int c;

    while ((c = *word++))
    {
        hash = ((hash << 5) + hash) + c;
    }

    return hash%N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    FILE *dic_open = fopen(dictionary,"r");
    if (dic_open == NULL)
    {
        printf("Couldn't open file");
        return false;
    }
    char word[LENGTH + 1];
    while (fscanf(dic_open, "%s", word) != EOF)
    {

        node *new_word = malloc(sizeof(node));
        if (new_word == NULL)
        {
            printf("couldnt reallocate memory!");
            fclose(dic_open);
            return false;
        }
        else
        {
            strcpy(new_word -> word, word);
            new_word->next = NULL;
            unsigned int index = hash(word);
            new_word->next = table[index];
            table[index] = new_word;
            SIZE++;
        }

    }
    fclose(dic_open);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return SIZE;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    for (int i = 0; i < N; i++)
    {

        node *cursor = table[i];
        while (cursor != NULL)
        {
            node *tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }

    }
    return true;
}


