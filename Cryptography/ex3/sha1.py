#! /usr/bin/python3
from Crypto.Hash import SHA
from itertools import permutations as perm

def sha1(text):
    obj = SHA.new()
    obj.update(text.encode())
    return obj.hexdigest()

def iterator(obj, depth):
    try:
        for each in obj[depth]:
            for every in iterator(obj, depth + 1):
                yield each + every
    except:
        yield ''


if __name__=='__main__':
    chars = ['Qq@','Ww','5%','(8[','Nn','Ii','=0}','+~*']
    val = '67ae1a64661ac8b4494666f58c4822408dd0a3e4'
    f = 0
    for each in iterator(chars, 0):
        for every in perm(each):
            candidate = ''.join(every)
            if sha1(candidate) == val:
                print(candidate)
                f = 1
                break
        if f == 1:
            break