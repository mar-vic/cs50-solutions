import csv
import re
from sys import argv

def main():
    if len(argv) < 2:
       print("Usage: python dna.py data.csv sequence.txt")
       return 1
    else:
        dbfilename = argv[1]
        seqfilename = argv[2]

    # Open the database and sequence files
    dbfile = open(dbfilename, "r")
    seqfile = open(seqfilename, "r")
    dbreader = csv.reader(dbfile)
    dnaseq = seqfile.read()

    rows_read = 0
    for row in dbreader:
        if rows_read == 0:
            # strsegments = [row[1], row[2], row[3]]
            strsegments = row[1:]
            stats = strstats(dnaseq, strsegments)
            print(stats)
            rows_read += 1
        else:
            # print(range(len(strsegments)))
            for i in range(len(strsegments)):
                # print("Cycling through " + row[0] + " STRs")
                if (row[0] == "Lavender"):
                    print(row[0] + "'s " + row[1:][i] + " reps.")
                    print("Sequence reps: " + str(stats[strsegments[i]]))
                if (stats[strsegments[i]] == int(row[1:][i])):
                    print("Matched " + strsegments[i] + " segment for " + row[0])
                    match = True
                else:
                    match = False
                    break

            if match:
                print(row[0])
                dbfile.close()
                seqfile.close()
                return 0

    dbfile.close()
    seqfile.close()

    print("No match")
    return 0


def longest_str_rep(sequence, strsegment):
    pattern = re.compile("(" + strsegment + "){1,}")
    sresult = pattern.search(sequence)
    span = None

    print(pattern.pattern)

    while sresult:
        span = sresult.span()
        reps = (span[1] - span[0]) / len(strsegment)
        pattern = re.compile("(" + strsegment + "){" + str(reps + 1) + ",}")
        print(pattern.pattern)
        sresult = pattern.search(sequence)

    if span == None:
        return 0
    else:
        return reps


def strstats(dnaseq, strsegments):
    strstats = dict()

    for segment in strsegments:
        strstats[segment] = longest_str_rep(dnaseq, segment)

    return strstats


main()
