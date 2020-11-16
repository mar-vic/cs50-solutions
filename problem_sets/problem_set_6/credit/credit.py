import re

def main():
    number = input("Number: ")
    if validate(number):
        if (re.search("^34|^37", number)): # AMEX cards start with 34 or 37
            print("AMEX")
        elif (re.search("^51|^52|^53|^54|^55", number)): # MASTERCARDs start
            # with 51, 52, 53, 54 or 55
            print("MASTER")
        elif (re.search("^4", number)): # VISAs starts with 4
            print("VISA")
        else:
            print("INVALID") # All other combinations are considered invalid
    else:
        print("INVALID")

def validate(number):
    # Return true when the number is a valid credit card number, according
    # Luhn's algorithm
    reversed = number[::-1]
    checksum = 0

    for i in range(len(reversed)): # loop through the digits of provided number
        digit = int(reversed[i])
        # In case the digit is on even position, double the digit and add the
        # sum of digits of the result to the checksum. Otherwise simply add the digit
        # to the checksum.
        if (i + 1) % 2 == 0:
            checksum += (digit * 2 - (digit * 2) % 10) / 10 + (digit * 2) % 10
        else:
            checksum += digit

    # If the checksum is divisble by 10, the number is a valid credit card
    # number, according to Luhn's algorithm. Otherwise, it is invalid.
    if (checksum % 10 == 0):
        return True
    else:
        return False

main()


# print(validate("378734493671000"))
