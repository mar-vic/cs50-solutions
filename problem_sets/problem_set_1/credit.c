#// Requests a number from the user and determines whether it is a
// valid credit card number.

#include <cs50.h>
#include <stdio.h>

int get_checksum(long number);
int get_number_of_digits(long number);
int get_digit_from_number(long number, int index);

int main(void)
{
    // int checksum = get_checksum(get_long("Number: "));

    // if (checksum % 10 == 0)
    // {
       // printf("%i is VALID", checksum);
    // }
    // else
    // {
       // printf("%i is INVALID", checksum);
    // }

    long number = get_long("Number: ");

    printf("Number of digits: %i\n", get_number_of_digits(number));
    printf("Third digit: %i\n", get_digit_from_number(number, 3));
}

int get_checksum(long number)
{
    bool is_every_other = false;
    int checksum = 0;

    do
    {
        int remainder = number % 10;

        if (is_every_other && ((remainder * 2) / 10) >= 1)
        {
            checksum += 1 + (remainder * 2) % 10;
        }
        else if (is_every_other)
        {
            checksum += remainder * 2;
        }
        else
        {
            checksum += remainder;
        }

        number = (number - remainder) / 10;
        is_every_other = !is_every_other;
    } while (number != 0);

    return checksum;
}

int get_number_of_digits(long number)
{
    int digit_counter = 0;

    do
    {
        digit_counter++;
        number = number / 10;
    } while (number >= 1);

    return digit_counter;
}

int get_digit_from_number(long number, int index)
{
    int number_of_digits = get_number_of_digits(number);

    for (int i = number_of_digits; i >= -(index - number_of_digits); i--)
    {
        number = (number - (number % 10)) / 10;
        printf("%li\n", number);
    }

    return number % 10;
}
