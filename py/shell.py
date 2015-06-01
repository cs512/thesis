#!/usr/bin/env python
from multiprocessing.dummy import Pool as ThreadPool
import os

pool = ThreadPool(3)
#path = "./full/ukbench01768.jpg"
path = './*.jpg'
os.system('mkdir ./out')
os.system('mkdir /dev/shm/full')
os.system('mkdir ./out/full')

def convert(path):
    tmpfile = '/dev/shm/{0}.pgm'.format(path)
    outfile = './out/{0}.sift.desc'.format(path)
    x = os.system('djpeg {0} | ppmtopgm | pnmnorm -bpercent=0.01 -wpercent=0.01 > {1}'.format(path, tmpfile))
    x = os.system('./compute_descriptors_linux64 -i {0} -o1 {1} -hesaff -sift'.format(tmpfile, outfile))
    os.system('rm -f {0}'.format(tmpfile))
    return

files = [eachLine[:-1] for eachLine in os.popen('ls {0}'.format(path)).readlines()]

results = pool.map(convert, files)

pool.close()
pool.join()

