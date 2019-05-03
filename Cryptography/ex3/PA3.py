#!/usr/bin/env python3
# -*- coding: utf-8 -*-

from Crypto.Hash import SHA256


if __name__ == '__main__':
    file = r'./1.mp4'
    #file = r'./2.mp4'
    blks = []
    with open(file, 'rb') as fp:
        while True:
            tmp = fp.read(1024)
            if tmp:
                blks.append(tmp)
            else:
                break
            
    obj = SHA256.new()
    blks.reverse()
    obj.update(blks[0])
    for i in range(1,len(blks)):
        obj.update(blks[i] + obj.digest())
    print(obj.hexdigest())