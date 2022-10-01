import matplotlib.pyplot as plt
import numpy as np
from EcgGenerator import SimEcg


if __name__ == '__main__':

    # Datos para derivacion 1
    a_D1 = np.array([.7, -1.5, 5.5, -1.5, 1.3, 0.1])
    d_D1 = np.array([0.03, 0.011, 0.011, 0.011, 0.047, 0.0076])
    t_D1 = np.array([0.11, 0.27, 0.3, 0.33, 0.605, 0.833])

    ecgObj_D1 = SimEcg(a_D1, d_D1, t_D1)
    ecg_D1, t = ecgObj_D1.ecg_gen()

    # Datos para derivacion 2
    a_D2 = np.array([0.9, -2.2, 12, -2.2, 3.1, 0.2])
    d_D2 = np.array([0.03, 0.011, 0.011, 0.011, 0.047, 0.0076])
    t_D2 = np.array([0.11, 0.27, 0.3, 0.33, 0.605, 0.833])

    ecgObj_D2 = SimEcg(a_D2, d_D2, t_D2)
    ecg_D2, t = ecgObj_D2.ecg_gen()

    plt.plot(t, ecg_D2)
    plt.show()
