import matplotlib.pyplot as plt
import numpy as np
import time
import csv
from fastdtw import fastdtw
from scipy.spatial.distance import euclidean

raw_signature = []
drawn_line = None
fig, ax = None, None

def on_press(event):
    global raw_signature, drawn_line
    raw_signature = []
    if event.xdata is not None and event.ydata is not None:
        raw_signature.append((event.xdata, event.ydata, time.time()))
        drawn_line, = ax.plot([event.xdata], [event.ydata], color='black')

def on_motion(event):
    global drawn_line
    if event.xdata is not None and event.ydata is not None:
        raw_signature.append((event.xdata, event.ydata, time.time()))
        x_vals = [p[0] for p in raw_signature]
        y_vals = [p[1] for p in raw_signature]
        drawn_line.set_data(x_vals, y_vals)
        fig.canvas.draw()

def on_release(event):
    print(f"\nNueva firma capturada con {len(raw_signature)} puntos.")
    plt.close(fig)
    nueva_firma = procesar_firma(raw_signature, 'firma_nueva.csv')
    comparar_con_referencia(nueva_firma, 'firma_1.csv')

def capturar_nueva_firma():
    global fig, ax
    fig, ax = plt.subplots()
    ax.set_title("Repite tu firma para validación")
    ax.set_xlim(0, 1)
    ax.set_ylim(0, 1)
    ax.set_aspect('equal')
    ax.grid(True)

    fig.canvas.mpl_connect('button_press_event', on_press)
    fig.canvas.mpl_connect('motion_notify_event', on_motion)
    fig.canvas.mpl_connect('button_release_event', on_release)

    plt.show()

def procesar_firma(firma_raw, csv_filename):
    if len(firma_raw) < 2:
        print("Firma demasiado corta.")
        return None

    firma_raw = np.array(firma_raw)

    x = firma_raw[:, 0]
    y = firma_raw[:, 1]
    t = firma_raw[:, 2] - firma_raw[0, 2]

    dt = np.diff(t)
    dt[dt == 0] = 1e-6

    vx = np.diff(x) / dt
    vy = np.diff(y) / dt

    t = t[1:]
    x = x[1:]
    y = y[1:]

    data = np.stack([t, x, y, vx, vy], axis=1)

    with open(csv_filename, 'w', newline='') as f:
        writer = csv.writer(f)
        writer.writerow(['t', 'x', 'y', 'vx', 'vy'])
        for row in data:
            writer.writerow(row)

    return data

def leer_firma_csv(csv_filename):
    with open(csv_filename, 'r') as f:
        reader = csv.reader(f)
        next(reader)  # saltar encabezado
        data = np.array([[float(x) for x in row] for row in reader])
    return data

def comparar_con_referencia(firma_nueva, archivo_referencia):
    firma_ref = leer_firma_csv(archivo_referencia)

    componentes = ['x', 'y', 'vx', 'vy']
    indices = [1, 2, 3, 4]

    distancias = []

    print("\nComparación DTW por componente:")
    for nombre, idx in zip(componentes, indices):
        serie_nueva = firma_nueva[:, idx]
        serie_ref = firma_ref[:, idx]
        distancia, _ = fastdtw(serie_nueva, serie_ref, dist=euclidean)
        print(f"- {nombre}(t): distancia DTW = {distancia:.2f}")
        distancias.append(distancia)

    dtw_total = sum(distancias)
    print(f"\n✅ Distancia DTW total: {dtw_total:.2f}")

    # Puedes poner un umbral empírico si quieres tomar una decisión
    if dtw_total < 100:  # ajustar según pruebas
        print("✅ Firma autenticada con éxito.")
    else:
        print("❌ Firma no coincide suficientemente con la referencia.")

if __name__ == "__main__":
    capturar_nueva_firma()
