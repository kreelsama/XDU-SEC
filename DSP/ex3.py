#! /usr/bin/python
import numpy as np
from matplotlib import pyplot as plt

def problem1():
    x1 = [1, 1, 1, 1]
    x2 = [1, 2, 3, 4, 4, 3, 2, 1]
    x3 = [4, 2, 1, 1, 1, 2, 3, 4]
    Y1 = np.fft.fft(x1, 8), np.fft.fft(x1, 16)
    Y2 = np.fft.fft(x2, 8), np.fft.fft(x2, 16)
    Y3 = np.fft.fft(x3, 8), np.fft.fft(x3, 16)
    X = np.linspace(0, 2*np.pi, 8), np.linspace(0, 2*np.pi, 16)

    plt.subplot(3,2,1)
    plt.title("x1 spectrum: 8 points")
    plt.stem(X[0], np.abs(Y1[0]), 'r')
    plt.subplot(3,2,2)
    plt.stem(X[1], np.abs(Y1[1]), 'b')
    plt.title("x1 spectrum: 16 points")

    plt.subplot(3,2,3)
    plt.stem(X[0], np.abs(Y2[0]), 'r')
    plt.title("x2 spectrum: 8 points")
    plt.subplot(3,2,4)
    plt.stem(X[1], np.abs(Y2[1]), 'b')
    plt.title("x2 spectrum: 16 points")

    plt.subplot(3,2,5)
    plt.stem(X[0], np.abs(Y3[0]), 'r')
    plt.title("x3 spectrum: 8 points")
    plt.subplot(3,2,6)
    plt.stem(X[1], np.abs(Y3[1]), 'b')
    plt.title("x3 spectrum: 16 points")

    plt.show()


def problem2():
    n  = np.arange(0, 20, 1)
    x4 = np.cos(np.pi/4 * n)
    x5 = np.cos(np.pi/4 * n) + np.cos(np.pi/8 * n)
    Y4 = np.fft.fft(x4, 8), np.fft.fft(x4, 16)
    Y5 = np.fft.fft(x5, 8), np.fft.fft(x5, 16)
    X  = np.arange(0,8,1), np.arange(0,16,1)

    plt.subplot(2,2,1)
    plt.stem(X[0], np.abs(Y4[0]), 'r')
    plt.title("x4 spectrum: 8 points")

    plt.subplot(2,2,2)
    plt.stem(X[1], np.abs(Y4[1]), 'b')
    plt.title("x4 spectrum: 16 points")

    plt.subplot(2,2,3)
    plt.stem(X[0], np.abs(Y5[0]), 'r')
    plt.title("x5 spectrum: 8 points")

    plt.subplot(2,2,4)
    plt.stem(X[1], np.abs(Y5[1]), 'b')
    plt.title("x4 spectrum: 16 points")

    plt.show()



def problem3():
    from numpy import cos,pi
    fs = 64
    T = 1 / fs
    Ts = 2.5 # Observation duration: 2.5s
    x = lambda t: cos(8*pi*t) + cos(16*pi*t) + cos(20*pi*t)
    n = np.linspace(0, 2.5, int(Ts * fs))
    xn = x(n)
    Y16 = np.fft.fft(xn, 16)
    Y32 = np.fft.fft(xn, 32)
    Y64 = np.fft.fft(xn, 64)
    plt.subplot(3,1,1)
    plt.stem(np.linspace(0,2*pi, 16), np.abs(Y16))
    plt.title("16 points")

    plt.subplot(3,1,2)
    plt.stem(np.linspace(0,2*pi, 32), np.abs(Y32))
    plt.title("32 points")

    plt.subplot(3,1,3)
    plt.stem(np.linspace(0,2*pi,64), np.abs(Y64))
    plt.title("64 points")

    plt.show()

if __name__ == '__main__':
    problem1()
    problem2()
    problem3()
