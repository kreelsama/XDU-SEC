# Challenge 43

from Crypto.Util.number import inverse
from Crypto.Hash import SHA


def ch43(p, q, g):
    print("---Challenge 43---")
    HM = 0xd2d0714f014a9784047eaeccf956520045c45265
    r = 548099063082341131477253921760299949438196259240
    s = 857042759984254168557880549501802188789837994940
    k = 0
    for i in range(0, pow(2,16) + 1):
        if r == pow(g, i, p) % q:
            k = i
            break

    if k == 0:
        print("Not found valid k")
        exit(0)

    x = (k * s - HM)*inverse(r, q) % q
    print("Decimal form:", x)
    print("Hexadecimal form:%x" % x )
    y = "%x" % x
    obj = SHA.new()
    obj.update(y.encode())
    print("SHA1 value:" ,obj.hexdigest())
    print("Expected value:0954edd5e0afe5542a4adf012611a91912a3ec16")


def ch44(p, q, g):
    print("---Challenge 44---")
    with open('44.txt', 'r') as fp:
        txt = fp.read()
    txt = txt.split('\n')
    s = [int(each[3:], 10) for each in txt[1::4]]
    r = [int(each[3:], 10) for each in txt[2::4]]
    hm = [int(each[3:], 16) for each in txt[3::4]]
    l = len(s)
    x = 0
    found = False
    for i in range(l):
        if found:
            break
        for j in range(i + 1, l):
            if r[i] == r[j]:
                k = ((hm[i] - hm[j])*inverse(s[i] - s[j], q)) % q
                x1 = ((k*s[i]-hm[i])*inverse(r[i],q)) % q
                x2 = ((k*s[j]-hm[j])*inverse(r[j],q)) % q
                if x1 == x2:
                    x = x1
                    found = True
                    break
    print("Decimal form:", x)
    y = "%x" % x
    print("Hexadecimal form:", y)
    obj = SHA.new()
    obj.update(y.encode())
    print("SHA1 value:" , obj.hexdigest())
    print("Expected value:ca8f6f7c66fa362d40760d135b763eb8527d3d52")



if __name__ == '__main__':
    p = int(''.join('''800000000000000089e1855218a0e7dac38136ffafa72eda7
     859f2171e25e65eac698c1702578b07dc2a1076da241c76c6
     2d374d8389ea5aeffd3226a0530cc565f3bf6b50929139ebe
     ac04f48c3c84afb796d61e5a4f9a8fda812ab59494232c7d2
     b4deb50aa18ee9e132bfa85ac4374d7f9091abc3d015efc87
     1a584471bb1'''.split('\n')).replace(' ', ''),16)
    q = 0xf4f47f05794b256174bba6e9b396a7707e563c5b
    g = int(''.join('''5958c9d3898b224b12672c0b98e06c60df923cb8bc999d119
     458fef538b8fa4046c8db53039db620c094c9fa077ef389b5
     322a559946a71903f990f1f7e0e025e2d7f7cf494aff1a047
     0f5b64c36b625a097f1651fe775323556fe00b3608c887892
     878480e99041be601a62166ca6894bdd41a7054ec89f756ba
     9fc95302291'''.split('\n')).replace(' ', ''),16)
    ch43(p, q, g)
    ch44(p, q, g)