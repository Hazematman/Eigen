#!/usr/bin/env python
import sys
from collections import namedtuple

rules = []
nonterm = []
states = []
follow = {}
first = {}
term = []

nontermRules = {}
transitions = {}
reductions = []

CFGState = namedtuple('CFGState', ['rule', 'pos'])
st = namedtuple('st', ['state', 'tok'])

def keyer(x):
    return ''.join(x.rule) + str(x.pos)

def createState(state):
    for cfgs in state:
        if cfgs.pos >= len(cfgs.rule):
            #for f in follow[cfgs.rule[0]]:
            #    c = CFGState(cfgs.rule + [f], cfgs.pos)
            #    state.append(c)

            #if len(follow[cfgs.rule[0]]) > 0:
            #    state.remove(cfgs)

            continue
        if cfgs.rule[cfgs.pos] in nonterm:
            for rule in nontermRules[cfgs.rule[cfgs.pos]]:
                c = CFGState(rule, 1)
                if c not in state:
                    state.append(c)

    state.sort(key=keyer)

def appendState(s1, s2):
    for state in s2:
        if state not in s1:
            s1.append(state)

    s1.sort(key=keyer)

def getFollow(sym):
    global rules
    fSet = []
    for rule in rules:
        if sym != rule[0]:
            if sym in rule:
                ind = rule.index(sym)
                if (ind+1) < len(rule) and rule[ind+1] not in fSet and rule[ind+1] not in nonterm:
                    fSet.append(rule[ind+1])

    return fSet

def makeFirst():
    for rule in rules:
        for tok in rule:
            first[tok] = []
            if tok not in nonterm and tok not in term:
                term.append(tok)

    for tok in term:
        first[tok] = [tok]

    for rule in rules:
        if rule[1] == "e" and len(rule) == 2:
            first[rule[0]].append("e")

    for nt in (nonterm+term):
        for rule in rules:
            if rule[1] == "e" and len(rule) == 2:
                continue
            else:
                allE = True
                for i in range(1, len(rule)):
                    for s in first[rule[i]]:
                        if s != "e" and s not in first[rule[0]]:
                            first[rule[0]].append(s)
                    if "e" not in first[rule[i]]:
                        allE = False
                        break

                if allE == True:
                    if "e" not in first[rule[0]]:
                        first[rule[0]].append("e")

def multFirst(l):
    ol = []
    for e in l:
        for j in first[e]:
            if j not in ol:
                ol.append(e)

    return ol

def makeFollow():
    for nt in nonterm:
        if nt == rules[0][0]:
            follow[nt] = []
        else:
            follow[nt] = []

    for nt in nonterm:
        for rule in rules:
            if nt in rule[1:]:
                i = rule[1:].index(nt)
                i += 1
                if i == (len(rule) - 1):
                    for e in follow[rule[0]]:
                        if e not in follow[rule[i]]:
                            follow[rule[i]].append(e)
                else:
                    l = first[rule[i+1]][:]
                    if "e" in l:
                        for e in follow[rule[0]]:
                            if e not in follow[rule[i]]:
                                follow[rule[i]].append(e)
                        l.remove("e")

                    for e in l:
                        if e not in follow[rule[i]]:
                            follow[rule[i]].append(e)

    
def main():
    if(len(sys.argv) < 3):
        print("Usage: {} <in-file> <out-file>".format(sys.argv[0]))
        return 0
    global rules

    inFile = open(sys.argv[1], "r")
    for line in inFile:
        rules.append(line.strip().split(" "))
    inFile.close()

    for rule in rules:
        tok = rule[0]
        if tok not in nonterm:
            nonterm.append(tok)
            nontermRules[tok] = []

    makeFirst()
    makeFollow()

    #for t in nonterm:
    #    print("first of {} is {}".format(t, first[t]))

    #for t in nonterm:
    #    print("follow of {} is {}".format(t, follow[t]))

    oldR = rules[:]
    nr = []
    for rule in rules:
        for f in follow[rule[0]]:
            nr.append(rule + [f])

    i = 0
    while i < len(rules):
        if len(follow[rules[i][0]]) != 0:
            rules.remove(rules[i])
            i = 0
        else:
            i += 1

    rules.extend(nr)

    #for rule in rules:
    #    print("{} -> {}".format(rule[0],' '.join(rule[1:])))

    for rule in rules:
        nontermRules[rule[0]].append(rule)

    states = [[CFGState(rules[0], 1)]]

    # Create inital state
    createState(states[0])

    while True:
        some = False
        for state in states:
            # If dot precedes non-term add non-term rules
            for cfgs in state:
                if cfgs.pos >= len(cfgs.rule):
                    continue
                if cfgs.rule[cfgs.pos] in nonterm:
                    for rule in nontermRules[cfgs.rule[cfgs.pos]]:
                        c = CFGState(rule, 1)
                        if c not in state:
                            some = True
                            state.append(c)

            # Add transitions
            for cfg in state:
                if cfg.pos < len(cfg.rule):
                    s = [CFGState(cfg.rule, cfg.pos+1)]
                    createState(s)
                    trans = st(states.index(state), cfg.rule[cfg.pos])
                    if s not in states:
                        some = True
                        states.append(s)
                        
                    if trans in transitions:
                        s1 = states[transitions[trans]]
                        #appendState(s1, s)
                        #createState(s1)
                    else:
                        transitions[trans] = states.index(s)

            allEnd = True
            for cfg in state:
                if cfg.pos < len(cfg.rule):
                    allEnd = False

            if allEnd and len(state) != 1:
                print("WE GOT REDUCE REDUCE ERROR")
                print(state)
                return 1

            oneEnd = False
            for cfg in state:
                if cfg.pos >= len(cfg.rule):
                    oneEnd = True

            if oneEnd and len(state) != 1:
                print("WE GOT SHIFT REDUCE ERROR")
                print(state)
                return 1

            if len(state) == 1:
                if(state[0].pos >= len(state[0].rule)):
                    if states.index(state) not in reductions:
                        reductions.append(states.index(state))

        if some == False:
            break

    for state in states:
        print(state)

    for t,s in transitions.iteritems():
        if s in reductions:
            ruleNum = rules.index(states[s][0].rule)
            print("{} reduces to {}".format(t,ruleNum))
        else:
            print("{} shifts to {}".format(t,s))
    return 0

if __name__ == "__main__":
    sys.exit(main())
