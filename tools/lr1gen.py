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
reductions = {}

CFGState = namedtuple('CFGState', ['rule', 'pos'])
st = namedtuple('st', ['state', 'tok'])

def keyer(x):
    return ''.join(x.rule) + "A"*x.pos

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

def multiFirst(l):
    cur = 0
    o = []
    while True:
        if cur >= len(l):
            break

        for e in first[l[cur]]:
            if e not in o:
                o.append(e)

        if "e" in first[l[cur]]:
            cur += 1
        else:
            break

    return o
    

def makeFollow():
    for nt in nonterm:
        if nt == rules[0][0]:
            follow[nt] = ['$']
        else:
            follow[nt] = []

    change = True
    while change == True:
        for nt in nonterm:
            change = False
            for rule in rules:
                if nt in rule[1:]:
                    i = rule[1:].index(nt)
                    i += 1
                    if i == (len(rule) - 1):
                        for e in follow[rule[0]]:
                            if e not in follow[rule[i]]:
                                follow[rule[i]].append(e)
                                change = True
                    else:
                        #l = first[rule[i+1]][:]
                        l = multiFirst(rule[i+1:])
                        for e in l:
                            if e != "e" and e not in follow[rule[i]]:
                                follow[rule[i]].append(e)
                                change = True

def makeRule(r):
    if r.pos < len(r.rule):
        if r.rule[r.pos] == "e":
            nR = makeRule(CFGState(r.rule, r.pos+1))
            return nR

    return r

def cClose(config):
    cS = config
    while True:
        added = False
        i = 0
        while i < len(cS):
            r = cS[i]
            if r.pos < len(r.rule):
                if r.rule[r.pos] in nonterm:
                    for nR in nontermRules[r.rule[r.pos]]:
                        nS = makeRule(CFGState(nR, 1))
                        if nS not in cS:
                            cS.append(nS)
                            added = True
            i += 1

        if added == False:
            break

    
    cS.sort(key=keyer)

    return cS

def makeConfigSet(rule):
    #if rule[1] == "e":
    #    return [CFGState(rule, 2)]

    cS = [makeRule(CFGState(rule,1))]

    if rule[1] in term:
        return cS

    return cClose(cS)

def succ(c,x):
    nS = []

    for r in c:
        if r.pos < len(r.rule):
            if r.rule[r.pos] == x:
                nS.append(makeRule(CFGState(r.rule, r.pos+1)))

    return cClose(nS)

def makeF():
    f = [makeConfigSet(rules[0])]

    while True:
        added = False
        for c in f:
            for r in c:
                for x in r.rule:
                    s = succ(c, x)
                    if len(s) != 0:
                        if s not in f:
                            f.append(s)
                            added = True

        if added == False:
            break

    return f

    
def main():
    if(len(sys.argv) < 3):
        print("Usage: {} <in-file> <out-file>".format(sys.argv[0]))
        return 0
    global rules

    inFile = open(sys.argv[1], "r")
    for line in inFile:
        rules.append(line.strip().split(" "))
    inFile.close()

    # Augment the grammar
    #rules.insert(0,["S'",rules[0][0],"$"])

    for rule in rules:
        tok = rule[0]
        if tok not in nonterm:
            nonterm.append(tok)
            nontermRules[tok] = []

    for rule in rules:
        nontermRules[rule[0]].append(rule)



    makeFirst()
    makeFollow()

    debug = True
    if debug:
        for t in nonterm:
            print("first of {} is {}".format(t, first[t]))

        for t in nonterm:
            print("follow of {} is {}".format(t, follow[t]))

    F = makeF()

    #for r in rules:
    #    print("{} -> {}".format(r[0],' '.join(r[1:])))

    numT = 0
    for i in F:
        ind = F.index(i)
        for cfg in i:
            if cfg.pos >= len(cfg.rule):
                if cfg.rule != rules[0]:
                    for tok in follow[cfg.rule[0]]:
                        S = st(ind, tok)
                        if S in reductions:
                            print("Reduce reduce error on".format(S))
                            return 1
                        reductions[st(ind,tok)] = rules.index(cfg.rule)
                        numT += 1

            else:
                tok = cfg.rule[cfg.pos]
                if tok in term:
                    #j = F.index(succ(i, tok))
                    sL = succ(i, tok)
                    if len(sL) != 0:
                        j = F.index(sL)
                        transitions[st(ind, tok)] = j
                        numT += 1

            for nt in nonterm:
                sL = succ(i, nt)
                if len(sL) != 0:
                    j = F.index(sL)
                    transitions[st(ind, nt)] = j
                    numT += 1


    for k in transitions:
        if k in reductions:
            print("Shift reduce error for {}".format(k))
            return 1

    of = open(sys.argv[2], "w")
    of.write("{}\n".format(len(term)))
    for t in term:
        of.write("{}\n".format(t))
    of.write("{}\n".format(len(nonterm)))
    for nt in nonterm:
        of.write("{}\n".format(nt))
    of.write("{}\n".format(rules[0][0]))
    of.write("{}\n".format(len(rules)))
    for r in rules:
        of.write("{}\n".format(' '.join(r)))
    of.write("{}\n{}\n".format(len(F), len(transitions)+len(reductions)))
    for t,s in transitions.iteritems():
        of.write("{} {} shift {}\n".format(t.state, t.tok, s))
    for t,s in reductions.iteritems():
        of.write("{} {} reduce {}\n".format(t.state, t.tok, s))

    return 0

if __name__ == "__main__":
    sys.exit(main())
