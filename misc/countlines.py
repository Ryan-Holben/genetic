import os
from collections import Counter

def main():
    folders = "core datasets evo main".split()
    filters = "BUILD WORKSPACE cpp h cc hpp c py".split()
    filecount = Counter()
    linecount = Counter()

    for root, dirs, files in os.walk("."):
        path = root.split(os.sep)
        if len(path) > 1 and path[1] not in folders:
            continue
        for file in files:
            ext = file.split(".")
            filecount.update({ext[-1]: 1})
            with open(root + os.sep + file, "r") as infile:
                linecount += Counter({ext[-1]: len(infile.read().split("\n"))})
    
    for k, v in filecount.most_common():
        if k in filters:
            print k, "\t\t", v, "files with", linecount[k], "lines"

    # print ("\nLine counts:")
    # for k, v in linecount.most_common():
    #     if k in filters:
    #         print(k, v)

main()