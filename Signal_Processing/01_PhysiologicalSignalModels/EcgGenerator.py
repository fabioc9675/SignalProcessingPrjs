# File class to generate the ECG signal
# @Author: Fabian Castano
# @Date: 2022-09-23
##

import numpy as np
from BaseWave import BaseWave


class SimEcg:
    # Funcion que reproduce la senal PPG de un paciente sano
    # a = vector de amplitudes
    # d = vector de duraciones
    # t = vector de tiempos
    # Fs = Frecuencia de muestreo
    # li = frecuencia Cardiaca
    # Dur = duracion de la simulacion en segundos
    def __init__(self, amplitude, duration, time, frequency=60, sample_rate=250, period=25):
        self.amplitude = amplitude  # Signal amplitude in mV
        self.duration = duration  # Signal duration in seconds
        self.time = time  # Signal time in seconds
        self.frequency = frequency  # Signal frequency of pattern in hertz
        self.sample_rate = sample_rate  # Frequency sample rate
        self.period = period  # length

    # Signal generator
    def ecg_gen(self):
        # base time vector
        self.x = np.arange(0, self.period, 1/self.sample_rate)
        self.ecg_array = np.zeros([len(self.x), 6])
        self.base = []

        # Signal generation
        for i in range(0, 5):
            self.base.append(BaseWave(self.x, self.amplitude[i], self.duration[i],
                                      self.time[i], self.frequency, self.sample_rate))
            self.ecg_array[:, i] += self.base[i].signal_gen()

        self.ecg = np.sum(self.ecg_array, axis=1)

        return self.ecg, self.x
