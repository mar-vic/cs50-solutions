#include <cs50.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
  // Open file
  FILE *file = fopen("phonebook.csv", "r");
  char c;

  while ((c = getc(file)) != EOF)
    {
      printf("%c", c);
    }

  /* // Get strings from user */
  /* char *name = get_string("Name: "); */
  /* char *number = get_string("Number: "); */

  /* // Print (write) strings to file */
  /* fprintf(file, "%s,%s,\n", name, number); */

  // Close file
  fclose(file);
}



