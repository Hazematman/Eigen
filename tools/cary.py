#!/usr/bin/env python
import sys

def main():
    if(len(sys.argv) < 3):
        print("Usage: {} <in-file> <out-file>".format(sys.argv[0]))

    inFile = open(sys.argv[1], "r")
    lines = []
    for line in inFile:
        lines.append(line.strip())
    inFile.close()

    outFile = open(sys.argv[2], "w")
    for line in lines:
        outFile.write("    \"{}\",\n".format(line))
    outFile.close()
    return 0

if __name__ == "__main__":
    sys.exit(main())
