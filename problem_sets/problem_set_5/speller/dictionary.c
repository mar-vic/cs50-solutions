// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 10;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
  // Calculate word's hashcode
  int hascode = hash(word);

  // Check whether word is present in hashtable
  node *dentry = table[hascode];

  if (dentry == NULL) // In case the bucket is empty, the word is misspelled and
                      // thus false is returned
    {
      return false;
    }
  else // In case the bucket is not empty, check the list in the bucket
    {
      if (strcasecmp(word, dentry->word) == 0)
        {
          return true;
        }
      else
        {
          while (dentry->next != NULL)
            {
              dentry = dentry->next;
              if (strcasecmp(word, dentry->word) == 0)
                {
                  return true;
                }
            }
        }
    }

  return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
  int sum = 0;
  for (int j = 0; word[j] != '\0'; j++)
    {
      sum += tolower(word[j]);
    }

  return sum % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
  // Open the dictionary file
  FILE* dfile = fopen(dictionary, "r");
  if (dfile == NULL)
    {
      return false;
    }

  // Read the file, line by line, calculate the hashes for individual lines (i.e.,
  // for dictionary entries), and insert them into the hash table.
  while (1 == 1)
    {
      char word[LENGTH + 1];
      int index = 0;
      char c;

      // Read a line from the file, which is supposed to represent a single
      // entry in the dictionary.
      for (c = (char)getc(dfile); c != '\n' && c != EOF; c = (char)getc(dfile), index++)
        {
          word[index] = (char) c;
        }

      // Jump out of the cycle when the end of file is encountered
      if (c == EOF)
        {
          break;
        }

      word[index] = '\0'; // Append the end of string character

      int hashcode = hash(word); // Calculate the hash code for dictionary entry

      // Insert entry into the hashtable
      // allocate memory for a node
      node *dentry = malloc(sizeof(node));
      if (dentry == NULL)
        {
          return false;
        }

      // initialize the node with appropriate values
      strcpy(dentry->word, word);
      dentry->next = NULL;

      // finally, test for collisions and insert the entry into the hash-table
      if (table[hashcode] == NULL) // without collision; simply assign the
                                   // addres of the entry to the pointer
        {
          table[hashcode] = dentry;
        }
      else // with collisions; add new entry at the beginning of the list
        {
          dentry->next = table[hashcode];
          table[hashcode] = dentry;
        }
    }

  // Close the file
  fclose(dfile);
  return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
  int dsize = 0;

  for (int i = 0; i < N; i++)
    {
      node *dentry = table[i];
      while (dentry != NULL)
        {
          dsize++;
          dentry = dentry->next;
        }
    }

  return dsize;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
  for (int i = 0; i < N; i++)
    {
      node *dentry = table[i];
      if (dentry == NULL)
        {
          continue;
        }
      node *nxtdentry = dentry->next;

      while (dentry != NULL)
        {
          if (nxtdentry != NULL)
            {
              node *n = nxtdentry;
              nxtdentry = nxtdentry->next;
              free(n);
            }
          else
            {
              free(dentry);
              dentry = NULL;
            }
        }
    }

    return true;
}
