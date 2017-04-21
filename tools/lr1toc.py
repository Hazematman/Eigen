#!/usr/bin/env python
import sys


fileTemplate = """#include "lr1.h"
/* This file was autogenerated by the
 * lr1toc.py script to convert cfg file
 * to a usable lr1 machine
 */

enum Rules {{
{}}};

struct Cfg *lr1Create() {{
    struct Cfg *lr1Cfg = cfgCreate({});

    /* Add all the rules */
{}
    /* Add all the state transitions */
{}
    return lr1Cfg;
}}
"""

def readTokens(inFile, tokens, numTokens):
    for i in range(numTokens):
        tok = inFile.readline().strip()
        tokens.append(tok)

def readNonTerm(inFile, nonTerms, numNonTerm):
    for i in range(numNonTerm):
        nonTerm = inFile.readline().strip()
        nonTerms.append(nonTerm)

def readRules(inFile, rules, numRules):
    for i in range(numRules):
        rule = inFile.readline().strip().split(" ")
        rules.append(rule)

def readTrans(inFile, trans, numTrans):
    for i in range(numTrans):
        t = inFile.readline().strip().split(" ")
        trans.append(t)


def genCFile(outFile, numStates, rules, trans, tokens, nonTerms):
    # Create enum string for rule names:
    nameStr = ""
    for nt in nonTerms:
        nameStr += "    RULE_{},\n".format(nt.upper())
    
    # Create string for all the added rules
    ruleStr = ""
    ii = 0
    for i in range(len(rules)):
        rule = rules[i]
        val = "MAX_TOKEN_VAL + RULE_{}".format(rule[0].upper())
        l = len(rule)-1
        for i in range(1,len(rule)):
            if rule[i] == "e":
                l -= 1
        ruleStr += "    cfgAddRule(lr1Cfg, {}, MAX_TOKEN_VAL + {}, {});\n".format(val, ii, l)
        ii += 1

    # Create string for all the transitions
    transStr = ""
    for t in trans:
        token = ""
        if t[1] not in tokens:
            token = "MAX_TOKEN_VAL + RULE_{}".format(t[1].upper())
        else:
            token = "TOKEN_{}".format(t[1])

        tt = "REDUCE"
        if t[2] == "shift":
            tt = "SHIFT"

        transStr += "    cfgAddTransition(lr1Cfg, {}, {}, {}, {});\n".format(t[0], token, tt, t[3])


    outStr = fileTemplate.format(nameStr, numStates, ruleStr, transStr)

    outF = open(outFile, "w")
    outF.write(outStr)
    outF.close()

    return

def main():
    if(len(sys.argv) < 3):
        print("Usage: {} <lr1 file input> <c file output>".format(sys.argv[0]))
        return 0

    inFile = open(sys.argv[1], "r")

    numTokens = int(inFile.readline())
    tokens = []

    readTokens(inFile, tokens, numTokens)

    numNonTerm = int(inFile.readline())
    nonTerms = []

    readNonTerm(inFile, nonTerms, numNonTerm)

    startRule = inFile.readline().strip()

    numRules = int(inFile.readline())
    rules = []

    readRules(inFile, rules, numRules)

    numStates = int(inFile.readline())
    numTrans = int(inFile.readline())
    trans = []

    readTrans(inFile, trans, numTrans)

    inFile.close()

    genCFile(sys.argv[2], numStates, rules, trans, tokens, nonTerms)
    return 0

if __name__ == "__main__":
    sys.exit(main())
