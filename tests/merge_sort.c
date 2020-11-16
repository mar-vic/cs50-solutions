#include <cs50.h>
#include <stdio.h>

void sort(int list[], int length);
void selection_sort(int list[], int length);
int * merge_sort(int list[], int length);

int main(void)
{
  int numbers[] = {3, 5, 0, 1, 15, 20};
  int length = 6;

  printf("Unsorted: ");
  for (int i = 0; i < length; i++)
    {
      printf("%i  ", numbers[i]);
    }

  int * sorted = merge_sort(numbers, length);

  printf("\nSorted: ");
  for (int i = 0; i < length; i++)
    {
      printf("%i  ", sorted[i]);
    }

  printf("\n");

  /* printf("Unsorted: "); */

  /* for (int i = 0; i < length; i++) */
  /*   { */
  /*     printf("%i, ", numbers[i]); */
  /*   } */

  /* selection_sort(numbers, length); */

  /* printf("\nSorted: "); */
  /* for (int i = 0; i < length; i++) */
  /*   { */
  /*     printf("%i, ", numbers[i]); */
  /*   } */

  /* printf("\n"); */
}

void sort(int list[], int length)
{
  int is_sorted = 0;

  while (is_sorted == 0)
    {
      for (int i = 0; i < length; i++)
        {
          if (list[i] > list[i+1] && length != (i+1))
            {
              int ith = list[i];
              int next = list[i+1];

              list[i] = next;
              list[i+1] = ith;
            }
        }

      is_sorted = 1;

      for (int i = 0; i < length; i++)
        {
          if ((i + 1) < length && list[i] > list[i+1])
            {
              is_sorted = 0;
              break;
            }
        }
    }
}

void selection_sort(int list[], int length)
{
  int unsorted_beginning = 0;
  int index_of_smallest = 0;

  while (unsorted_beginning < length)
    {
      for (int i = index_of_smallest; i < length; i++)
        {
          if (list[i] < list[index_of_smallest])
            {
              index_of_smallest = i;
            }
        }

      int smallest = list[index_of_smallest];
      list[index_of_smallest] = list[unsorted_beginning];
      list[unsorted_beginning] = smallest;
      unsorted_beginning++;
      index_of_smallest = unsorted_beginning;
    }
}

void merge_sort_static(int list[], int length)
{
  if (length == 1)
    {
      return list;
    }

  if (length == 2)
    {
      if ()
    }
}

int * merge_sort(int list[], int length)
{
  if (length == 1)
    {
      return list;
    }

  if (length == 2)
    {
      if (list[0] > list[1])
        {
          int smaller = list[1];
          int bigger = list[0];

          list[0] = smaller;
          list[1] = bigger;
        }
    }

  int left_part_length;
  int right_part_length;

  if (length % 2 == 0)
    {
      left_part_length = length / 2;
      right_part_length = left_part_length;
    }
  else
    {
      left_part_length = (length - 1) / 2;
      right_part_length = left_part_length + 1;
    }

  int left_part[left_part_length];
  int right_part[right_part_length];

  for (int i = 0; i < left_part_length; i++)
    {
      left_part[i] = list[i];
    }

  for (int i = 0; i < right_part_length; i++)
    {
      right_part[i] = list[i + left_part_length];
    }

  /* int * sorted_left_part = merge_sort(left_part, left_part_length); */
  /* int * sorted_right_part = merge_sort(right_part, right_part_length); */
  /* int * sorted_whole; */

  int * sorted_left_part = merge_sort(left_part, left_part_length);
  int * sorted_right_part = merge_sort(right_part, right_part_length);

  for (int i = 0, j = 0; i < length; i = i + 2, j++)
    {
      if (j != left_part_length)
        {
          if (sorted_right_part[j] < sorted_left_part[j])
            {
              list[i] = sorted_right_part[j];
              list[i + 1] = sorted_left_part[j];
            }
          else
            {
              list[i] = sorted_left_part[j];
              list[i + 1] = sorted_right_part[j];
            }
        }
      else
        {
          list[i] = sorted_right_part[j];
        }
    }

  return list;
}

