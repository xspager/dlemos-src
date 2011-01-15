import sys
from lxml import etree, objectify
from xml.sax.saxutils import unescape

def backupToLines(f):

    backupXML = etree.parse(f)

    twittes = backupXML.xpath('//text')

    linhas = []

    for twitte in twittes:
        linhas.append(unescape(twitte.text).encode('utf-8'))

    return linhas[6:]

