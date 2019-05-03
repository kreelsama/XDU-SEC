#! /usr/bin/python3

import binascii
from Crypto.Cipher import AES


def enc(inp:str):
    key = binascii.unhexlify(''.join(["FC" for x in range(16)]))
    iv = binascii.unhexlify(''.join(["6A" for x in range(16)]).encode())
    inp = inp.replace(';','#').replace('=','#')
    cip = AES.new(key, AES.MODE_CBC, IV = iv)
    txt = cip.encrypt(inp)
    return txt

def server(inp : str):
    key = binascii.unhexlify(''.join(["FC" for x in range(16)]))
    iv = binascii.unhexlify(inp[0:32].encode())
    inp = inp[32:]
    dec = AES.new(key, AES.MODE_CBC, IV=iv)
    pt = dec.decrypt(binascii.unhexlify(inp.encode()))
    print(pt)
    if b";admin=true" in pt:
        print("Tampering success!")
    else:
        print("Tampering wrong!")

def adv():
    prefix = "comment1=cooking%20MCs;userdata="
    data ='#admin#true###########'#24 bytes
    suffix = ";comment2=%20like%20a%20pound%20of%20bacon"
    cipher = list(enc(prefix+data+suffix))
    print(len(cipher))
    cipher[16] = cipher[16] ^ ord('#') ^ ord(';')
    cipher[16 + 6] = cipher[16+6] ^ ord('#') ^ ord('=')

    cipher = ''.join(["%02x" % each for each in cipher])
    server(cipher)

if __name__ == '__main__':
    adv()