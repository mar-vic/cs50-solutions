// Determine the reading level of a text according to Coleman-Liau index

#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);
double calculate_index(int letter_count, int word_count, int sentence_count);

int main(void)
{
  string text = get_string("Text: ");

  int letter_count = count_letters(text);
  int word_count = count_words(text);
  int sentence_count = count_sentences(text);
  int index = (int) calculate_index(letter_count, word_count, sentence_count);

  if (index >= 16)
    {
      printf("Grade 16+\n");
    }
  else if (index < 1)
    {
      printf("Before Grade 1\n");
    }
  else
    {
      printf("Grade %i\n", index);
    }
}

int count_letters(string text)
{
  int count = 0;

  for (int i = 0; text[i] != '\0'; i++)
    {
      if (isalpha(text[i]))
        {
          count++;
        }
    }

  return count;
}

int count_words(string text)
{
  if (text[0] == '\0')
    {
      return 0;
    }

  int count = 0;

  for (int i = 0; text[i] != '\0'; i++)
    {
      if (text[i] == ' ')
        {
          count++;
        }
    }

  return count + 1;
}

int count_sentences(string text)
{
  int count = 0;

  for (int i = 0; text[i] != '\0'; i++)
    {
      if (text[i] == '.' || text[i] == '?' || text[i] == '!')
        {
          count++;
        }
    }

  return count;
}

double calculate_index(int letter_count, int word_count, int sentence_count)
{
  float L = (((float) letter_count / (float) word_count) * 100); // average number of letters per 100 words
  float S = (((float) sentence_count / (float) word_count) * 100); // average number of sentences per 100 words

  return round(0.0588 * L - 0.296 * S - 15.8);
}
