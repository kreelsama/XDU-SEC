
# This file was *autogenerated* from the file attack.sage
from sage.all_cmdline import *   # import sage library

_sage_const_768 = Integer(768); _sage_const_1 = Integer(1); _sage_const_0 = Integer(0); _sage_const_7 = Integer(7); _sage_const_5 = Integer(5); _sage_const_2 = Integer(2); _sage_const_9 = Integer(9); _sage_const_8 = Integer(8); _sage_const_512 = Integer(512); _sage_const_13 = Integer(13); _sage_const_11 = Integer(11); _sage_const_10 = Integer(10); _sage_const_17 = Integer(17); _sage_const_16 = Integer(16); _sage_const_15 = Integer(15); _sage_const_14 = Integer(14); _sage_const_22 = Integer(22); _sage_const_18 = Integer(18); _sage_const_256 = Integer(256); _sage_const_24 = Integer(24)
import binascii
load("coppersmith.sage")
idx = [_sage_const_1 ,_sage_const_5 ,_sage_const_7 ,_sage_const_9 ,_sage_const_10 ,_sage_const_11 ,_sage_const_13 ,_sage_const_14 ,_sage_const_15 ,_sage_const_17 ,_sage_const_18 ]
path = "../frames/Frame"
N, e, c = [], [], []
for each in idx:
	with open(path+str(each), 'r') as fp:
		tmp = fp.read()
	N.append(int(tmp[_sage_const_0   :_sage_const_256 ],_sage_const_16 ))
	e.append(int(tmp[_sage_const_256 :_sage_const_512 ], _sage_const_16 ))
	c.append(int(tmp[_sage_const_512 :_sage_const_768 ], _sage_const_16 ))

prefix = "9876543210abcdef0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
found = [_sage_const_0  for i in range(len(idx))]
messages = []
for i in range(_sage_const_2 ,_sage_const_8 ):
	print "-------------------------------------------------------------------------------------testing for i = " + str(i)
	m = int(prefix[_sage_const_0 :_sage_const_22 ] + "%02x" % i + prefix[_sage_const_24 :],_sage_const_16 )
	beta = _sage_const_1 
	epsilon = beta / _sage_const_7 
	for j in range(len(idx)):
		if found[j] == _sage_const_1 :
			continue
		print "test for message" + str(idx[j])
		ZmodN = Zmod(N[j])     
		P = PolynomialRing(ZmodN, names=('x',)); (x,) = P._first_ngens(1)
		f = (m + x)**e[j] - c[j]
		dd = f.degree()
		mm = ceil(beta**_sage_const_2  / (dd * epsilon))
		tt = floor(dd * mm * ((_sage_const_1 /beta) - _sage_const_1 ))
		XX = ceil(N[j]**((beta**_sage_const_2 /dd) - epsilon))
		roots = coppersmith_howgrave_univariate(f, N[j], beta, mm, tt, XX)
		if roots:
			found[j] = _sage_const_1 
			res = roots[_sage_const_0 ]
			messages.append((idx[j],res + m))
			cleartxt = hex(res + m)[-_sage_const_16 :]
			print("Found frame "+ str(idx[j]) + ":" + binascii.unhexlify(cleartxt.encode()).decode())
			

print "----------------RESULT--------------------"
for each in messages:
	print "Frame " + str(each[_sage_const_0 ])
	print hex(each[_sage_const_1 ])
	print "Frame " + str(each[_sage_const_0 ]) + ":" + binascii.unhexlify(hex(each[_sage_const_1 ])[-_sage_const_16 :].encode()).decode()

