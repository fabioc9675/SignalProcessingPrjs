import matplotlib.pyplot as plt
import numpy as np
import time
import csv
import sys
import os

# Variables globales
raw_signature = []
drawn_line = None
fig, ax = None, None
base_filename = 'firma_t'  # Valor por defecto

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
    print(f"\nFirma capturada con {len(raw_signature)} puntos.")
    save_signature_image(base_filename + '.png')
    plt.close(fig)
    process_and_save_signature(raw_signature, base_filename + '.csv')

def capture_signature():
    global fig, ax
    fig, ax = plt.subplots()
    ax.set_title("Dibuja tu firma aquí")
    ax.set_xlim(0, 1)
    ax.set_ylim(0, 1)
    ax.set_aspect('equal')
    ax.grid(True)

    fig.canvas.mpl_connect('button_press_event', on_press)
    fig.canvas.mpl_connect('motion_notify_event', on_motion)
    fig.canvas.mpl_connect('button_release_event', on_release)

    plt.show()

def save_signature_image(filename):
    ax.set_title("Firma capturada")
    ax.axis('off')
    fig.savefig(filename, bbox_inches='tight')
    print(f"Imagen de la firma guardada como '{filename}'")

def process_and_save_signature(signature_data, filename):
    if len(signature_data) < 2:
        print("Firma demasiado corta para analizar.")
        return

    signature_data = np.array(signature_data)

    x = signature_data[:, 0]
    y = signature_data[:, 1]
    t = signature_data[:, 2] - signature_data[0, 2]

    dt = np.diff(t)
    dt[dt == 0] = 1e-6

    vx = np.diff(x) / dt
    vy = np.diff(y) / dt

    # Recortar
    t = t[1:]
    x = x[1:]
    y = y[1:]

    # Graficar curvas
    fig2, axs = plt.subplots(4, 1, figsize=(8, 8), sharex=True)
    axs[0].plot(t, x, label='x(t)', color='blue')
    axs[1].plot(t, y, label='y(t)', color='green')
    axs[2].plot(t, vx, label='vx(t)', color='red')
    axs[3].plot(t, vy, label='vy(t)', color='orange')
    for axx in axs:
        axx.legend()
        axx.grid()
    axs[-1].set_xlabel("Tiempo (s)")
    plt.tight_layout()
    plt.show()

    # Guardar CSV
    with open(filename, 'w', newline='') as f:
        writer = csv.writer(f)
        writer.writerow(['t', 'x', 'y', 'vx', 'vy'])
        for i in range(len(t)):
            writer.writerow([t[i], x[i], y[i], vx[i], vy[i]])

    print(f"Datos de la firma guardados como '{filename}'")

if __name__ == "__main__":
    if len(sys.argv) > 1:
        base_filename = os.path.splitext(sys.argv[1])[0]
    capture_signature()

