# File class to generate the base wave to the different physiological signals
# @Author: Fabian Castano
# @Date: 2022-09-23
##

import numpy as np


class BaseWave:
    # Constructor
    def __init__(self, x_vector, amplitude, duration, time, frequency=60, sample_rate=250):
        self.x_vector = x_vector  # sample vector in seconds
        self.amplitude = amplitude  # Signal amplitude in mV
        self.duration = duration  # Signal duration in seconds
        self.time = time  # Signal time in seconds
        self.frequency = frequency  # Signal frequency of pattern in hertz
        self.sample_rate = sample_rate  # Frequency sample rate
        self.signal = np.zeros(len(self.x_vector))

    # Signal generator
    def signal_gen(self):
        n_waves = np.round(
            (len(self.x_vector)*self.frequency)/(60*self.sample_rate))

        for i in range(0, int(n_waves)):
            self.signal += self.amplitude * \
                np.exp(-((self.x_vector-self.time-i*(60/self.frequency))
                       ** 2.0)/(2*self.duration**2))
        return self.signal
