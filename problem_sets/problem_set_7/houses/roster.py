import sqlite3
from sys import argv


def main():
    if (len(argv) < 2):
        print("Usage: ./python roster.py house_name")
        return 1

    house = argv[1]
    conn = sqlite3.connect("./students.db")
    cursor = conn.cursor()

    for row in cursor.execute("SELECT first, middle, last, birth FROM students WHERE house = ? ORDER BY last, first", [house]):
        if (row[1] == None):
            print(row[0] + " " + row[2] + ", born " + str(row[3]))
        else:
            print(row[0] + " " + row[1] + " " + row[2] + ", born " + str(row[3]))

    conn.close()
    return 0


main()



