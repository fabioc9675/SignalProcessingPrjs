# File function to compute the Short-Term Fourier Transform
# @Author: Fabian Castano
# @Date: 2022-09-23
##

import re
import numpy as np
from scipy.fft import fft, ifft
from scipy import signal
import matplotlib.pyplot as plt


def spect_fab(x, fs, window='hann', w_size=None, w_over=None, nfft=None, process=False):
    # longitud de x
    N = len(x)
    # definicion del tamaño de la ventana
    if w_size == None:
        w_size = len(x)
    # definicion del tamaño del overlaping
    if w_over == None:
        w_over = 0
    elif w_over >= w_size:
        w_over = 0
        print("Error, w_over debe ser menor que w_size")
    # definicion del tamaño del fft
    if nfft == None:
        nfft = len(x)
    # frequency vector
    f_ = fs*np.arange(0, (nfft))/(2*nfft)
    # time vector
    t_len = int((N-w_size)/(w_size-w_over))
    t_ = np.linspace(0, N/fs, t_len)

    # Implementacion del algoritmo
    # creacion de la ventana
    w = signal.get_window(window, w_size)
    my_STFT = np.zeros([nfft, t_len])
    X_orig = np.zeros([w_size, t_len])

    # Calculo del STFT
    for i in range(0, t_len):
        x_seg = x[int(i*(w_size-w_over)):int(i*(w_size-w_over)+w_size)]
        x_seg = x_seg*w  # aplicacion de ventana a la señal

        # Calculo del FFT
        Xn = fft(x_seg, n=2*nfft)
        Xn = np.array(Xn[0:nfft])
        Pxn = (1/(fs*nfft))*(np.abs(Xn)**2)
        my_STFT[:, i] = Pxn
        X_orig[:, i] = x_seg

    if process == True:
        plt.figure(figsize=(8, 6))
        plt.subplot(2, 1, 1)
        plt.plot(X_orig)
        plt.title("Ventanas")
        plt.subplot(2, 1, 2)
        plt.plot(f_, my_STFT)
        plt.title("Espectros")
        plt.show()

    return f_, t_, my_STFT
