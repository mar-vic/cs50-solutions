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
void print_voting_records(void);
bool cycle_check(int source_winner, int loser);

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
    print_voting_records();

    sort_pairs();
    print_voting_records();

    lock_pairs();
    print_voting_records();

    print_winner();

    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
  // Search for a candidate with given name. If there is such candidate on
  // ballot, assign them provided rank and return true. Otherwise false is
  // returned.
  for (int i = 0; i < candidate_count; i++)
    {
      if (strcmp(candidates[i], name) == 0)
        {
          ranks[i] = rank;
          return true;
        }
    }
  return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
  for (int i = 0; i < candidate_count; i++)
    {
      for (int j = 0; j < candidate_count; j++)
        {
          if (ranks[i] < ranks[j])
            {
              preferences[i][j]++;
            }
        }
    }
  return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
  pair_count = 0;

  for (int i = 0; i < candidate_count; i++)
    {
      for (int j = 0; j < candidate_count; j++)
        {
          pair two_way_match;

          if (preferences[i][j] > preferences[j][i])
            {
              two_way_match.winner = i;
              two_way_match.loser = j;
              pairs[pair_count] = two_way_match;
              pair_count++;
            }
          /* else if(preferences[j][i] > preferences[i][j]) */
          /*   { */
          /*     two_way_match.winner = j; */
          /*     two_way_match.loser = i; */
          /*     pairs[pair_count] = two_way_match; */
          /*     pair_count++; */
           /* } */
        }
    }
  return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
  int is_sorted = 0;

  while (is_sorted == 0)
    {
      for (int i = 0; i < pair_count; i++)
        {
          if (pair_count != (i+1))
            {
              pair ith_pair = pairs[i];
              pair next_pair = pairs[i+1];
              int ith_strength  = preferences[ith_pair.winner][ith_pair.loser];
              int next_strength = preferences[next_pair.winner][next_pair.loser];

              if (next_strength > ith_strength)
                {
                  pairs[i] = next_pair;
                  pairs[i+1] = ith_pair;
                }
            }
        }

      is_sorted = 1;

      for (int i = 0; i < pair_count; i++)
        {
          if ((i + 1) < pair_count)
            {
              pair ith_pair = pairs[i];
              pair next_pair = pairs[i+1];
              int ith_strength  = preferences[ith_pair.winner][ith_pair.loser];
              int next_strength = preferences[next_pair.winner][next_pair.loser];

              if (ith_strength < next_strength)
                {
                  is_sorted = 0;
                  break;
                }

            }
        }
    }

  return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
  for (int i = 0; i < pair_count; i++)
    {
      int winner = pairs[i].winner;
      int loser = pairs[i].loser;

      if (!cycle_check(winner, loser))
        {
          locked[winner][loser] = true;
        }
    }

  return;
}

// Check whether the given edge would generate cycle in the graph.
bool cycle_check(int source_winner, int loser)
{
  // Cycle through all candidates and check whether the loser is already locked
  // as a winner in some other edge od the graph.
  for (int i = 0; i < candidate_count; i++)
    {
      if (locked[loser][i])
        {
          // If the loser in already locked edge is identical to original
          // winner, then cycle is generated and thus true is returned.
          if (source_winner == i)
            {
              return true;
            }
          // Otherwise, check whether the cycle is not generated indirectly.
          else
            {
              return cycle_check(source_winner, i);
            }
        }
    }

  return false;
}

// Print the winner of the election
void print_winner(void)
{
  if (candidate_count == 1)
    {
      printf("%s\n", candidates[0]);
      return;
    }

  for (int i = 0; i < pair_count; i++)
    {
      int winner = pairs[i].winner;
      int loser = pairs[i].loser;
      bool is_source = true;

      if (locked[winner][loser])
        {
          for (int j = 0; j < candidate_count; j++)
            {
              if (locked[j][winner])
                {
                  is_source = false;
                }
            }
        }

      if (is_source)
        {
          printf("%s\n", candidates[winner]);
          return;
        }
    }
}

// Print voting records
void print_voting_records(void)
{
  printf("VOTING RECORDS:\n");
  for (int i = 0; i < candidate_count; i++)
    {
      for (int j = 0; j < candidate_count; j++)
        {
          printf("%i ", preferences[i][j]);
        }
      printf("\n");
    }

  printf("ANALYSIS:\n");
  for (int i = 0; i < pair_count; i++)
    {
      printf("%s beats %s by %i - %i\n",
             candidates[pairs[i].winner],
             candidates[pairs[i].loser],
             preferences[pairs[i].winner][pairs[i].loser],
             preferences[pairs[i].loser][pairs[i].winner]);
    }

  printf("LOCKED EDGES:\n");
  for (int i = 0; i < candidate_count; i++)
    {
      for (int j = 0; j < candidate_count; j++)
        {
          if (locked[i][j])
            {
              printf("%s to %s is locked.\n", candidates[i], candidates[j]);
            }
        }
    }

  printf("\n");
}
