from scipy.signal import *
from matplotlib import pyplot as plt
import numpy as np
from numpy import cos, pi
from numpy.fft import fft

def mstg():
    N = 800
    Fs = 10000
    T = 1/Fs
    Tp = N * T
    t = np.arange(0, (N-1)*T, T)
    k = np.arange(0, N, 1)
    f = k / Tp
    fc1 = Fs / 10
    fm1 = fc1 / 10
    fc2 = Fs / 20
    fm2 = fc2 / 10
    fc3 = Fs / 40
    fm3 = fc3 / 10
    xt1 = cos(2*pi*fm1*t) * cos(2*pi*fc1*t)
    xt2 = cos(2*pi*fm2*t) * cos(2*pi*fc2*t)
    xt3 = cos(2*pi*fm3*t) * cos(2*pi*fc3*t)
    st = xt1 + xt2 + xt3
    fxt = fft(st, N)

    plt.subplot(2,1,1)
    plt.plot(t, st)
    plt.xlabel('t/s')
    plt.ylabel('s(t)')
    plt.axis([0, Tp/8, min(st), max(st)])
    plt.title('Waveform of s(t)')
    plt.grid(True)

    plt.subplot(2,1,2)
    plt.stem(f, abs(fxt)/max(abs(fxt)))
    plt.axis([0, Fs/5, 0, 1.2])
    plt.xlabel('f/Hz')
    plt.ylabel('Amplitude')
    plt.title('Spectrum of s(t)')
    plt.grid(True)

    plt.show()

    return st


def myfilter(st):
    #Fixed parameters
    gpass = 0.1
    gtop = 60
    N = 800
    Fs = 10000
    T = 1/Fs
    Tp = N * T
    f = np.arange(0, N - 1, 1) / Tp

    #Lowpass
    wp =  2 * 300 / Fs
    ws = 2 * 400 / Fs
    N, wpo = ellipord(wp, ws, gpass, gtop)
    B, A = ellip(N, rp=gpass, rs=60, Wn=wpo, btype='lowpass')
    y1 = lfilter(B, A, st)
    y = fft(y1)
    plt.subplot(3,1,1)
    plt.plot(f, abs(y)/max(abs(y)))
    plt.axis([0, Fs / 5, 0, 1.2])
    plt.title("Lowpass Spectrum")
    plt.grid(True)


    #bandpass
    wp = 2*430 / Fs, 2* 600 / Fs
    ws = 2*380 / Fs, 2*700 /Fs
    N, wpo = ellipord(wp, ws, gpass, gtop)
    B, A = ellip(N, rp=gpass, rs=60, Wn=wpo, btype='bandpass')
    y1 = lfilter(B, A, st)
    y = fft(y1)
    plt.subplot(3,1,2)
    plt.plot(f, abs(y)/max(abs(y)))
    plt.axis([0, Fs / 5, 0, 1.2])
    plt.title("Bandpass Spectrum")
    plt.grid(True)


    #Highpass
    wp = 2*850 / Fs
    ws = 2*800 / Fs
    N, wpo = ellipord(wp, ws, gpass, gtop)
    B, A = ellip(N, rp=gpass, rs=60, Wn=wpo, btype='highpass')
    y1 = lfilter(B, A, st)
    y = fft(y1)
    plt.subplot(3,1,3)
    plt.plot(f, abs(y)/max(abs(y)))
    plt.axis([0, Fs / 5, 0, 1.2])
    plt.title("Highpass Spectrum")
    plt.grid(True)


    plt.show()



if __name__ == '__main__':
    st = mstg()
    myfilter(st)