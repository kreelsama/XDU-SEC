from Crypto.Util import number
from progressbar import *

p = 13407807929942597099574024998205846127479365820592393377723561443721764030073546976801874298166903427690031858186486050853753882811946569946433649006084171
g = 11717829880366207009516117596335367088558084999998952205599979459063929499736583746670572176471460312928594829675428279466566527115212748467589894601965568
h = 3239475104050450443565264378728065788649097520952449527834792452971981976143292558073856937958553180532878928001494706097394108577585732452307673444020333

widgets = ['Progress: ',Percentage(), ' ', Bar('#'),' ', Timer()]
tbl = {}
bar1 = ProgressBar(maxval=pow(2,20), widgets=widgets)
bar1.start()
for x1 in range(0, pow(2,20) + 1):
    bar1.update(x1)
    try:
        tmp = (h * number.inverse(pow(g, x1, p), p)) % p
        tbl[tmp] = x1
    except Exception:
        continue
# Using Dictionary, indexing complexity O(1). List indexing is O(n)
# I learned that dict is already using hash table, see:
# https://stackoverflow.com/questions/39980323/are-dictionaries-ordered-in-python-3-6
bar1.finish()
mid = pow(g,pow(2,20), p)
bar2 = ProgressBar(maxval=pow(2,20), widgets=widgets)
bar2.start()
for x0 in range(0, pow(2,20) + 1):
    bar2.update(x0)
    tmp = pow(mid, x0, p)
    try:
        idx = tbl[tmp]
        res = x0
    except KeyError:
        continue
bar2.finish()

x = res * pow(2,20) + idx
print(x)