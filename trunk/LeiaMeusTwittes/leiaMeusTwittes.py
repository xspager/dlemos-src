#!/usr/bin/python

import sys, twitter, faleIsso

f = open(sys.argv[1])

linhas = twitter.backupToLines(f)

for linha in linhas:
    print linha
    faleIsso.fale('pt_BR', linha)
