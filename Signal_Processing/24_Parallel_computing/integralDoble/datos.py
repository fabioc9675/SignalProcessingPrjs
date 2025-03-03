import numpy as np

n = 500000  # 1 millón de puntos
x = np.random.rand(n)
y = np.random.rand(n)
z = np.random.rand(n)
f = np.sin(x) * np.cos(y) * np.exp(z)
np.savetxt("data1.txt", np.column_stack((x, y, z, f)), fmt="%.6f")