# -*- coding: utf-8 -*-
"""
Created on Thu Oct 11 19:02:28 2018

@author: 13175
"""
from Crypto.Hash import SHA
from Crypto.Cipher import AES
from base64 import b64decode
import binascii

string = "12345678<8<<<1110182<1111167<<<<<<<<<<<<<<<4"
# ? = 7

def sha1(string):
    h = SHA.new()
    h.update(string)
    return h.hexdigest().upper()

if __name__ == '__main__':
    cipher = "9MgYwmuPrjiecPMx61O6zIuy3MtIXQQ0E59T3xB6u0Gyf1gYs2i3K9Jxaa0zj4gTMazJuApwd6+jdyeI5iGHvhQyDHGVlAuYTgJrbFDrfB22Fpil2NfNnWFBTXyf7SDI"
    cipher = b64decode(cipher)
    MRZ = '12345678<811101821111167'
    sha_raw = sha1(MRZ.encode())
    x = sha_raw[0:32]+'00000001'
    x = binascii.a2b_hex(x)
    k_raw = sha1(x)[0:32]
    res = ''
    for i in range(len(k_raw)//2):
        parity = k_raw[i*2:i*2+2]
        if bin(int(parity,16)).count('1') % 2 != 1:
            parity = hex(int(parity,16) ^ 0x01)[2:]
        res += parity.upper()
    print(res)
    iv = '0' * 32
    obj = AES.new(binascii.unhexlify(res), AES.MODE_CBC, IV = binascii.unhexlify(iv.encode()))
    print(obj.decrypt(cipher))