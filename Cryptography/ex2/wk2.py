# -*- coding: utf-8 -*-

from Crypto.Cipher import AES
from Crypto.Util import Counter
import binascii

def CBC(msg, key):
    '''
    CBC with random IV
    '''
    iv = msg[0:32]
    msg = msg[32:]
    obj = AES.new(binascii.unhexlify(key.encode()), AES.MODE_CBC, IV = binascii.unhexlify(iv.encode()))
    return obj.decrypt(binascii.unhexlify(msg.encode()))

def CTR(msg, key):
    iv = msg[0:32]
    msg = msg[32:]
    obj = AES.new(binascii.unhexlify(key.encode()), AES.MODE_CTR, counter = Counter.new(128, initial_value=int(iv, 16)))
    #, IV = binascii.unhexlify(iv.encode()))
    return obj.decrypt(binascii.unhexlify(msg.encode()))

def encrypt(msg, key, iv, mode):
    #print(msg)
    obj = AES.new(binascii.unhexlify(key.encode()), AES.MODE_ECB)
    cipher = iv
    if mode == AES.MODE_CBC:
        raw = iv
        loop = len(msg) // len(key)
        for i in range(loop):
            tmp = "%032x" % (int(raw,16) ^ int(msg[32*i:32*i+32],16))
            raw = binascii.hexlify(obj.encrypt(binascii.unhexlify(tmp))).decode()
            cipher += raw
        return cipher
    elif mode == AES.MODE_CTR:
        iv = int(iv, 16)
        loop = len(msg) // len(key)  + 1
        for  i in range(loop):
            IV = "%032x" % iv
            tmp = binascii.hexlify(obj.encrypt(binascii.unhexlify(IV))).decode()
            ms = msg[32*i:32*i+32]
            while len(ms) < len(IV):
                ms += '0'
            
            tmp = "%032x" % (int(ms,16) ^ int(tmp, 16))
            iv += 1
            cipher += tmp
        return cipher

if __name__ == '__main__':
    cipher = ['4ca00ff4c898d61e1edbf1800618fb2828a226d160dad07883d04e008a7897ee2e4b7465d5290d0c0e6c6822236e1daafb94ffe0c5da05d9476be028ad7c1d81',
              '5b68629feb8606f9a6667670b75b38a5b4832d0f26e1ab7da33249de7d4afc48e713ac646ace36e872ad5fb8a512428a6e21364b0c374df45503473c5242a253',
              '69dda8455c7dd4254bf353b773304eec0ec7702330098ce7f7520d1cbbb20fc388d1b0adb5054dbd7370849dbf0b88d393f252e764f1f5f7ad97ef79d59ce29f5f51eeca32eabedd9afa9329',
              '770b80259ec33beb2561358a9f2dc617e46218c0a53cbeca695ae45faa8952aa0e311bde9d4e01726d3184c34451']
    key = ['140b41b22a29beb4061bda66b6747e14','140b41b22a29beb4061bda66b6747e14',
           '36f18357be4dbd77f050515c73fcf9f2','36f18357be4dbd77f050515c73fcf9f2']
    plain = []
    for msg, k in zip(cipher, key):
        plain.append(CBC(msg, k) if cipher.index(msg) < 2 else CTR(msg, k))
        print(plain[-1])
    print('-------------------------------------------------------')
    for msg, cip, k in zip(plain, cipher, key):
        print(msg)
        iv = cip[0:32]
        print(len(msg), len(k))
        if cipher.index(cip)<2:
            res = encrypt(binascii.hexlify(msg).decode(), k, iv, AES.MODE_CBC)
        else:
            res = encrypt(binascii.hexlify(msg).decode(), k, iv, AES.MODE_CTR)
        print(res)
