# RSA Attacks
from gmpy2 import *
import binascii
import multiprocessing

def read(lst:list)->tuple:
    N = []
    e = []
    c = []
    dir = r"./frames/Frame"
    for each in lst:
        path = dir + str(each)
        with open(path,'r') as fp:
            tmp = fp.read()
        N.append(mpz(tmp[0:256],base = 16))
        e.append(mpz(tmp[256:512],base = 16))
        c.append(mpz(tmp[512:768],base = 16))
    return N, e, c


def CRT(N, c):
    '''
    Chinese Remainder Theorem
    '''
    mul = mpz(1)
    for each in N:
        mul *= each
    ans = mpz(0)
    for i, each in enumerate(N):
        tmp = div(mul, each)
        y = invert(tmp, each)
        ans += (c[i] * tmp * y) % mul
    return ans % mul


def CMA(N, e1, e2, c1, c2):
    '''
    Common Modulus Attack
    '''
    g, s1, s2 = gcdext(e1, e2)
    m =(powmod(c1,s1,N)*powmod(c2,s2,N))%N
    return m


def pollard_p(n, B):
    a = mpz(2)
    e = mpz(2)
    while e<=B:
        a = powmod(a, e, n)
        p = gcd(a-1, n)
        if 1<p<n:
            return p
        e += 1
    p = gcd(a-1, n)
    if 1<p<n:
        return p
    else:
        return -1

def pollard_rho(N, idx):
    print("pollard-rho for" + str(idx))
    x_fixed = mpz(2)
    cycle_size = mpz(2)
    x = mpz(2)
    factor = mpz(1)
    while factor == 1:
        count = 1
        while count <= cycle_size and factor <= 1:
            x = (x*x + 1) % N
            factor = gcd(x-x_fixed, N)
            count += 1
        cycle_size *= 2
        x_fixed = x
    print("Frame" + str(idx) + "FOUND:" + str(factor));
    return factor


def fermat(num):
    a = isqrt_rem(num)[0] + 1
    b = a**2 - num
    while 1:
        q = isqrt_rem(b)
        if q[1] == 0:
            return a - q[0]
        a += 1
        b = a**2 - num

if __name__ == '__main__':
    print("----------------Frame0,4: Common Modulus Attack------------------")
    N,e,c = read([0,4])
    m = CMA(N[0], e[0], e[1], c[0], c[1])
    print(hex(m))
    print(binascii.unhexlify(hex(m)[-16:].encode()).decode())
    print("-----------------Frame3,8,12,16,20:Hastad Attack-----------------")
    N, e, c = read([3,8,12,16,20])
    m = CRT(N, c)
    m = iroot(m, len(N))#m = m^(1/5)
    print(hex(m[0]))
    print(binascii.unhexlify(hex(m[0])[-16:].encode()).decode())
    print("-------------------Frame2,6,19:Pollard p-1 Factorization------------------")
    frames = [2,6, 19]
    N, e, c = read(frames)
    for i in range(len(frames)):
        p = pollard_p(N[i],mpz(10**6))
        q = div(N[i], p)
        phi = (p-1) * (q-1)
        d = invert(e[i], phi)
        print("Public key:" + hex(e[i]))
        print("Private key:" + hex(d))
        m = powmod(c[i], d, N[i])
        print(hex(m))
        print("Frame"+str(frames[i])+':'+ binascii.unhexlify(hex(m)[-16:].encode()).decode())
    print("-------------------Frame10,14: Fermat Factorization------------------")
    idx = [10,  14]
    N, e, c = read(idx)
    for i, each in enumerate(N):
        print("Testing for frame " + str(idx[i]))
        p = fermat(each)
        q = div(each, p)
        phi = (p-1) * (q-1)
        d = invert(e[i], phi)
        m = powmod(c[i], d, each)
        print("Public key:" + hex(e[i]))
        print("Private key:" + hex(d))
        print(hex(m))
        print("Frame" + str(idx[i]) +" :" + binascii.unhexlify(hex(m)[-16:].encode()).decode())
