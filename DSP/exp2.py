import numpy as np
from matplotlib import pyplot as plt
import math


def sample(fs):
    A = 444.128
    alpha = 50 * math.sqrt(2) * np.pi
    Omega = 50*math.sqrt(2) * np.pi
    T = 1 / fs
    x = lambda n: A * np.exp(-alpha*n*T) * np.sin(Omega*n*T)
    n = np.arange(0, 0.064, T)
    Y = x(n)
    FY = np.abs(np.fft.fft(Y, 64))
    FY = FY / max(FY)
    FX = np.arange(0,64,1)
    return FX, FY


def problem1():
    freq = [1000, 300, 200]
    for i in range(3):
        fx, fy = sample(freq[i])
        plt.subplot(3, 1, i + 1)
        plt.plot(fx, fy)
        plt.title("Sampling for %d Hz" % freq[i])
    plt.show()


def problem2():
    x = lambda t: [(1+ n) if 0<= n <= 13 else ((27 - n) if 14 <= n <= 26 else 0) for n in t]
    n = np.arange(0, 27, 1)
    y = x(n)
    omega = np.linspace(0,2*np.pi,1280)
    Y = np.zeros(omega.shape, dtype=np.complex128)
    for i in range(len(omega)):
        tmp = np.array([y[n]*np.exp(-1j*omega[i]*n) for n in range(0,27)], dtype=np.complex128)
        Y[i] = np.sum(tmp, dtype=np.complex128)
    plt.subplot(3,1,1)
    plt.plot(omega ,np.abs(Y))
    plt.title("X spectrum")

    X32 = np.linspace(0, 2*np.pi, 32)
    Y32 = Y[::40]
    plt.subplot(3,1,2)
    plt.stem(X32, np.abs(Y32))
    plt.title("X32 spectrum")

    X16 = np.linspace(0, 2*np.pi, 16)
    Y16 = Y[::80]

    plt.subplot(3,1,3)
    plt.stem(X16, np.abs(Y16))
    plt.title("X16 spectrum")
    plt.show()

    plt.subplot(3,1,1)
    plt.stem(np.arange(0, len(y), 1), y)
    plt.title("x series")

    plt.subplot(3,1,2)
    x32 = np.abs(np.fft.ifft(Y32, 32))
    plt.stem(np.arange(0, len(x32), 1), x32)
    plt.title("x32 series")

    plt.subplot(3,1,3)
    x16 = np.abs(np.fft.ifft(Y16, 16))
    plt.stem(np.arange(0, len(x16), 1), x16)
    plt.title("x16 series")
    plt.show()

if __name__ == '__main__':
    problem1()
    problem2()
