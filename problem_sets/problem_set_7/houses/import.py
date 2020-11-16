import csv
import re
from sys import argv
import sqlite3


def main():
    if (len(argv) < 2):
        print("Usage: python ./import.py characters.csv")
        return 1
    else:
        csvfilename = argv[1]

    with open(csvfilename, "r") as file:
        reader = csv.DictReader(file)
        conn = sqlite3.connect("./students.db")
        cursor = conn.cursor()


        # Import all rows of csv file into students database
        for row in reader:
            # Use regex to extract first, middle and last name from csv data
            fullname = re.compile("\S+").findall(row["name"])

            if len(fullname) < 3:
                firstname = fullname[0]
                middlename = None
                lastname = fullname[1]
            else:
                firstname = fullname[0]
                middlename = fullname[1]
                lastname = fullname[2]

            cursor.execute("INSERT INTO students (first, middle, last, house, birth) VALUES (?, ?, ?, ?, ?)",
                           [firstname, middlename, lastname, row["house"], row["birth"]])


        conn.commit()
        conn.close()

    return 0


main()
