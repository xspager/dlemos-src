#!/usr/bin/python
import sys, subprocess, urllib

def fale(langcode='en',texto='Alguma coisa'):
    textToSpechURL='http://translate.google.com.br/translate_tts?'
    url = textToSpechURL + urllib.urlencode({'tl':langcode, "q":texto, "ie":'UTF-8'})
#    print url
    subprocess.call(['mpg123', '-q', '-b 128', url])

"""
f = open(sys.argv[1])
linhas = f.readlines()

texto = ""
for arg in sys.argv[1:]:
    texto += arg + ' '

fale('pt', texto)

for linha in linhas:
    linha = linha[]
    print linha
    fale('pt-BR',linha)
"""
