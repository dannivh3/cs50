#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool lock_recursion(int winner, int loser);
int difference(int winner, int loser);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // Iterating Through all candidates
    for (int i = 0; i < candidate_count; i++)
    {
        // If i find a match i assign it the rank integer
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // iterating through the candidates preferrer
    for (int r = 0; r < candidate_count; r++)
    {
        int y = ranks[r];

        // iterating throug the candidates preferry
        for (int c = 1 + r; c < candidate_count; c++)
        {
            int x = ranks[c];
            // index in ranks will get ++ over the all the the index after it
            preferences[y][x]++;
        }
        preferences[y][y] = 0;
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // just iterating through candidates and putting it into the pairs struct
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
        }
    }
}


// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // making a selection sort
    for (int p = 0; p < pair_count; p++)
    {
        // getting the candidate index for winners and losers

        pair temp_pair = pairs[p];
        // the highest number i keep here
        int high_num = 0;
        int placement = 0;
        //  iterating through all the pairs
        for (int i = 0 + p; i < pair_count; i++)
        {
            int win = pairs[i].winner;
            int los = pairs[i].loser;
            int diff = difference(win, los);

            // if the difference is higher then the last we change it and remember the index row
            if (diff > high_num)
            {
                high_num = diff;
                placement = i;
            }

        }
        // sorting it in the right order
        pairs[p] = pairs[placement];
        pairs[placement] = temp_pair;



    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // Iterating through all the pairs
    for (int i = 0; i < pair_count; i++)
    {
        int win = pairs[i].winner;
        int los = pairs[i].loser;
        // putting it into a function that returns true or false
        locked[win][los] = lock_recursion(win, los);
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    // Iterating through the winning candidate
    for (int i = 0; i < candidate_count; i++)
    {
        // checking if candidate loses to anyone
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i])
            {
                break; // breaks out of loop if he does
            }
        }
        //checking if the candidate wins
        for (int l = 0; l < candidate_count; l++)
        {
            if (locked[i][l])
            {
                printf("%s\n", candidates[i]);
            }

        }





    }
}

bool lock_recursion(int win, int los)
{
    // here we check if we cycled to the original
    if (win == los)
    {
        return false;
    }
    else
    {   // checking if the loser wins anything if not we return true
        for (int i = 0; i < pair_count; i++)
        {
            // if the loser wins against someone we need to check if we made a chain
            if (locked[los][i] == true)
            {
                if (!lock_recursion(win, i))
                {
                    return false;
                }

            }
        }
        return true;
    }
}
int difference(int winner, int loser)
{
    return preferences[winner][loser] - preferences[loser][winner];
}

