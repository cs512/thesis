#!/usr/bin/env python
#coding: utf-8

import os, commands
import re

count = 0
patt = re.compile(r'([1-9]\d*)|(0\.)')

def getNum(s):
    if s == None:
        return -1
    if s == "0.":
        return 0
    else:
        return int(s)

for i in range(0, 1, 4):
    s = "./main.elf --func find --sql ./db.sqlite3 --pic ./full/ukbench{0:05}.jpg".format(i)
    output = commands.getoutput(s)
    sortArr = []
    for eachLine in output.split('\n'):
        header = eachLine.split('$')[0]
        footer = eachLine.split('$')[1]
        footer = float(footer)
        tmp = header.split(':')
        tmp[1] = float(tmp[1]) / footer
        sortArr.append(tuple(tmp))
        sortArr.sort(key = lambda x:x[1], reverse = True)
        sortArr = sortArr[:4]
    for path, num in sortArr:
        m = patt.search(path)
        num = getNum(m.group(0))
        if( 0 <= num and num < 4):
            count += 1

f = open("res.txt", "w")
f.write("count:{0}\n".format(count))

