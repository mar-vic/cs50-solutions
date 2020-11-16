import re


def main():
    # Get text from user
    text = input("Text: ")
    # Initialize counters for number of characters, words and sentences in the text
    ccount = 0.0
    wcount = 0.0
    scount = 0.0
    inword = False

    for c in text:
        if re.search("[a-z]|[A-Z]", c):
            if not inword:
                inword = True
                wcount += 1.0
            ccount += 1.0
        else:
            if inword and c == ' ':
                inword = False

            if re.search("[.!?]", c):
                scount += 1.0

    L = (ccount / wcount) * 100  # average number of characters per 100 words
    S = (scount / wcount) * 100  # average number of sentences per 100 words
    clindex = int(round(0.0588 * L - 0.296 * S - 15.8))  # Coleman-Liau readability index

    if clindex < 1:
        print("Before Grade 1")
    elif clindex >= 16:
        print("Grade 16+")
    else:
        print("Grade ", clindex)


main()
