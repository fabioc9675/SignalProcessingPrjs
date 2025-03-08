import numpy as np
import time
from multiprocessing import Pool, cpu_count

# Constantes físicas
h = 6.62607015e-34         # Constante de Planck [J·s]
pi = np.pi                  # Pi
hbar = h / (2 * pi)         # Constante de Planck reducida [J·s]
epsilon0 = 8.854187817e-12  # Permitividad del vacío [F/m]
epsilon_GaAs = 10.2 * epsilon0  # Permitividad del GaAs [F/m]
e = 1.602176634e-19        # Carga elemental [C]
xJeV = 1.0 / e             # Conversión de Julios a eV
constante = (e**2)/(4 * pi * epsilon_GaAs) * xJeV * 1e3  # Factor de conversión a meV

# Volúmenes elementales
dV1 = dV2 = (0.4)**3  # (0.4 nm)^3

def leer_datos(archivo):
    """Lee archivo de datos y devuelve coordenadas y valores de función."""
    datos = np.loadtxt(archivo)
    x, y, z = datos[:,0], datos[:,1], datos[:,2]
    fval_e = datos[:,3:33]  # 30 valores por punto
    return x, y, z, fval_e

def calcular_distancia(p1, p2):
    """Calcula distancia entre dos puntos en metros."""
    dx = (p1[0] - p2[0]) * 1e-9
    dy = (p1[1] - p2[1]) * 1e-9
    dz = (p1[2] - p2[2]) * 1e-9
    return np.sqrt(dx**2 + dy**2 + dz**2 + 1e-20)

def procesar_k(args):
    """Procesa un parámetro k en paralelo."""
    k, x1, y1, z1, fval1, x2, y2, z2, fval2 = args
    inicio_k = time.time()
    
    # Normalización
    sum1 = np.sum(fval1[:,k]**2) * dV1
    sum2 = np.sum(fval2[:,k]**2) * dV2
    
    # Cálculo principal
    suma_total = 0.0
    for i in range(len(x1)):
        psi1_sq = (fval1[i,k]**2)/sum1
        for j in range(len(x2)):
            r = calcular_distancia((x1[i], y1[i], z1[i]), 
                                  (x2[j], y2[j], z2[j]))
            if r < 1e-15: continue
            psi2_sq = (fval2[j,k]**2)/sum2
            suma_total += (psi1_sq * psi2_sq) / r * dV1 * dV2
    
    total = suma_total * constante
    tiempo = time.time() - inicio_k
    return (k, total, tiempo)

if __name__ == "__main__":
    inicio_total = time.time()
    
    # Leer datos
    print("Leyendo datos...")
    x1, y1, z1, fval_e1 = leer_datos("data1.txt")
    x2, y2, z2, fval_e2 = leer_datos("data2.txt")
    
    # Configurar paralelismo
    params = [(k, x1, y1, z1, fval_e1, x2, y2, z2, fval_e2) 
              for k in range(30)]
    
    print("Calculando integrales...")
    with Pool(cpu_count()) as p:
        resultados = p.map(procesar_k, params)
    
    # Ordenar y guardar resultados
    resultados.sort()
    xb = [(k+1)*0.1 for k in range(30)]
    totales = [res[1] for res in resultados]
    
    np.savetxt("resultados_python.dat", 
               np.column_stack([xb, totales]), 
               header="# xb\tTotal(meV)", 
               fmt="%.1f\t%.6e")
    
    print(f"\n[T] Tiempo total: {time.time()-inicio_total:.2f}s")