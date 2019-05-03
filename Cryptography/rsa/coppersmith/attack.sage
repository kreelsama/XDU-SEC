import binascii
load("coppersmith.sage")
idx = [1,5,7,9,10,11,13,14,15,17,18]
path = "../frames/Frame"
N, e, c = [], [], []
for each in idx:
	with open(path+str(each), 'r') as fp:
		tmp = fp.read()
	N.append(int(tmp[0  :256],16))
	e.append(int(tmp[256:512], 16))
	c.append(int(tmp[512:768], 16))

prefix = "9876543210abcdef0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
found = [0 for i in range(len(idx))]
messages = []
for i in range(2,8):
	print "-------------------------------------------------------------------------------------testing for i = " + str(i)
	m = int(prefix[0:22] + "%02x" % i + prefix[24:],16)
	beta = 1
	epsilon = beta / 7
	for j in range(len(idx)):
		if found[j] == 1:
			continue
		print "test for message" + str(idx[j])
		ZmodN = Zmod(N[j])     
		P.<x> = PolynomialRing(ZmodN)
		f = (m + x)^e[j] - c[j]
		dd = f.degree()
		mm = ceil(beta**2 / (dd * epsilon))
		tt = floor(dd * mm * ((1/beta) - 1))
		XX = ceil(N[j]**((beta**2/dd) - epsilon))
		roots = coppersmith_howgrave_univariate(f, N[j], beta, mm, tt, XX)
		if roots:
			found[j] = 1
			res = roots[0]
			messages.append((idx[j],res + m))
			cleartxt = hex(res + m)[-16:]
			print("Found frame "+ str(idx[j]) + ":" + binascii.unhexlify(cleartxt.encode()).decode())
			

print "----------------RESULT--------------------"
for each in messages:
	print "Frame " + str(each[0])
	print hex(each[1])
	print "Frame " + str(each[0]) + ":" + binascii.unhexlify(hex(each[1])[-16:].encode()).decode()