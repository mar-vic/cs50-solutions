// Implements a program for (de)ciphering text with substitution cipher.

#include<stdio.h>
#include<cs50.h>
#include<string.h>
#include<ctype.h>

// Declarations of local functions
int validate_key(string key);
char cipher_letter(char letter, string key);
void print_cipher_text(string text, string key);

int main(int argc, string argv[])
{
  if (argc != 2)
    {
      printf("Usage: ./substitution key\n");
      return 1;
    }

  string key = argv[1];
  int validation_status = validate_key(key);

  if (validation_status == 1)
    {
      printf("The key should have 26 characters.\n");
      return 1;
    }
  else if (validation_status == 2)
    {
      printf("The key should contain only alphabetic characters.\n");
      return 1;
    }
  else if (validation_status == 3)
    {
      printf("The key shouldn't have redundancies.\n");
      return 1;
    }

  string plaintext = get_string("plaintext: ");

  printf("plaintext: %s\n", plaintext);
  print_cipher_text(plaintext, key);
  printf("\n");

  return 0;
}

int validate_key(string key)
{
  // Checks wether the key is 26 character long (otherwise returns 1), contains
  // only alphabetic characters (otherwise returns 2), or does contain each
  // character only once (otherwise returns 3). Returns 0 if all conditions are
  // satisified.

  int key_length = strlen(key);

  // Check for the lenght. Return 1 if not equal to 26.
  if (key_length != 26)
    {
      return 1;
    }

  // Check wether all characters are alphabetic. Return 2 if not.
  for (int i = 0; i < key_length; i++)
    {
      if (isalpha(key[i]) == 0)
        {
          return 2;
        }
    }

  // Check for redundancy. Return 3 if there is some.
  for (int i = 0; i < key_length; i++)
    {
      for (int j = 0; j < key_length; j++)
        {
          if (key[i] == key[j] && i != j)
            {
              return 3;
            }
        }
    }

  return 0;
}

char cipher_letter(char letter, string key)
{
  // If the letter is alphabetic, substitute it according to the key while
  // preserving its case. In case the letter is not alphabetic return the letter
  // as is.

  if (islower(letter) != 0)
    {
      return tolower(key[(int) letter - 97]);
    }
  else if(isupper(letter) != 0)
    {
      return toupper(key[(int) letter - 65]);
    }
  else
    {
      return letter;
    }
}

void print_cipher_text(string text, string key)
{
  printf("ciphertext: ");
  for (int i = 0; text[i] != '\0'; i++)
    {
      printf("%c", cipher_letter(text[i], key));
    }
}
