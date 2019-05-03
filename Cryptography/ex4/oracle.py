#! /usr/bin/python3
import multiprocessing
import requests
import binascii

plaintext = ['' for i in range(3)]


def request(data):
    target = "http://crypto-class.appspot.com/po?er="
    url = target + data.decode()
    ret = requests.get(url)
    print(url)
    code = ret.status_code
    print(code)
    if code == 403 or code == 200:
        return 1
    elif code == 404:
        return 2
    else:
        return 0


def b2h(x):
    res = ''
    for each in x:
        res += "%02x" % each
    return res


def getPlainText(data,index):
    blk = [binascii.unhexlify(data[i*32:i*32+32].encode()) for i in range(len(data)//32)]
    tmp = blk
    enc = list(tmp[-1])
    for j in range(1, len(tmp[-1]) + 1):
        forgeable = [0 for i in range(len(tmp[-1]))]
        for k in range(1, j):
            forgeable[-k] = enc[-k] ^ j
        for k in range(0, 0xFF + 1):
            print(k)
            forgeable[-j] = k
            s = b2h(forgeable)
            code = request(s.encode()+binascii.hexlify(tmp[-1]))
            if code == 1 :
                if k == 255:
                    return
                continue
            elif code == 2 :
                enc[-j] = k ^ j
                break
            else:
                continue
    global plaintext
    plaintext[index] = b2h([enc[i] ^ tmp[-2][i] for i in range(len(tmp[-1]))])

if __name__ == '__main__':
    data = ["f20bdba6ff29eed7b046d1df9fb7000058b1ffb4210a580f748b4ac714c001bd",
            "58b1ffb4210a580f748b4ac714c001bd4a61044426fb515dad3f21f18aa577c0",
            "4a61044426fb515dad3f21f18aa577c0bdf302936266926ff37dbf7035d5eeb4"]
    p1 = multiprocessing.Process(target=getPlainText, args=(data[0],0))
    p2 = multiprocessing.Process(target=getPlainText, args=(data[1],1))
    p3 = multiprocessing.Process(target=getPlainText, args=(data[2],2))
    p1.start()
    p2.start()
    p3.start()
    p1.join()
    p2.join()
    p3.join()
    print(plaintext)
