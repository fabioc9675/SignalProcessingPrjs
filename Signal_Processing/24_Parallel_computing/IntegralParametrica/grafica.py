import plotly.graph_objects as go
import numpy as np

def interactive_3d_plot(archivo, k=0):
    x, y, z, fval_e = leer_datos(archivo)
    funcion_onda = fval_e[:, k]
    
    fig = go.Figure(data=go.Scatter3d(
        x=x, y=y, z=z,
        mode='markers',
        marker=dict(
            size=4,
            color=funcion_onda,
            colorscale='Viridis',
            opacity=0.8
        )
    ))
    
    fig.update_layout(scene=dict(
        xaxis_title='X (nm)',
        yaxis_title='Y (nm)',
        zaxis_title='Z (nm)'))
    
    fig.show()


def leer_datos(archivo):
    """Lee archivo de datos y devuelve coordenadas y valores de función."""
    datos = np.loadtxt(archivo)
    x, y, z = datos[:,0], datos[:,1], datos[:,2]
    fval_e = datos[:,3:33]  # 30 valores por punto
    return x, y, z, fval_e

interactive_3d_plot('data2.txt', k=20)