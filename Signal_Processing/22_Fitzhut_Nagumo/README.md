# FitzHugh-Nagumo Cardiac Electrophysiology Simulator

A computational toolkit for simulating neuronal/cardiac action potentials and synthesizing ECG signals using a modified FitzHugh-Nagumo (FHN) model. Designed for researchers studying excitable systems and cardiac arrhythmias.
**Keywords**: Cardiac electrophysiology, Action potentials, Finite element method, ECG synthesis, Parameter sensitivity.

---

## Installation

### Prerequisites

- **Python 3.10+** (tested on Python 3.10.12)
- **Gmsh 4.9+** (for finite element mesh generation - [download here](https://gmsh.info/))

### 1. Clone the Repository

```bash
git clone -b devFabianFitzhutNaguno https://github.com/fabioc9675/SignalProcessingPrjs.git
cd SignalProcessingPrjs/Signal_Processing/22_Fitzhut_Nagumo
```

### Install Python Dependencies

```bash
pip install -r requirements.txt
```

### Usage

The core of the project is located in the `FHNmod` folder, where the Jupyter notebook `FHN2` allows for the exploration of the modified FitzHugh-Nagumo equations and the adjustment of parameters such as $a$, $b$, $c_1$, $c_2$, $d$, $k$, $A$, and $B$ to study their impact on action potential waveforms. The notebook automatically generates plots that describe the behavior of the model with the modification of the parameters.

To model the heart, finite element methods were used to simulate how electrical activity propagates through heterogeneous tissues. Meanwhile, synthetic ECG signals were obtained by combining the specific patterns of each tissue, such as the sinoatrial node or the ventricles.

The code is written in `Python 3.10`, using libraries such as `NumPy`, `SciPy`, and `Matplotlib`, ensuring easy replication of results. Feel free to modify the parameters, run the simulations, and observe how cardiac signals change it's like having a virtual heart on your computer.

### Repository Structure

```
22_Fitzhut_Nagumo/
├── FHN_Model/                    # Core simulation code
│   ├── FHN_model.ipynb           # Interactive FHN parameter exploration
│   ├── FHN2_model.ipynb          # Interactive FHN parameter exploration
│   ├── ode3py.ipynb              # Solution of ODE equations
│   ├── output/
├── FHNmod_Model/  
│   └── FHN2_model.ipynb          # ECG lead configurations
│   ├── output/
├── HH_Model/                     # HH model sim
│   ├── FN_firstSteep.ipynb  
│   ├── FN_Model.ipynb   
│   ├── FN_secondSteep.ipynb  
│   ├── hh_equations.py  
│   ├── HH_Model.ipynb   
│   ├── output/
├── Nullclines/                    # Nullclines evaluation
│   ├── Nullclines_FHN.ipynb   
├── SweepParamet                   # Sweep Parameters
│   ├── SweepParameters_a.ipynb   
│   ├── SweepParameters_ab.ipynb  
│   ├── SweepParameters_Acap.ipynb  
│   ├── SweepParameters_an.ipynb  
│   ├── SweepParameters_b.ipynb   
│   ├── SweepParameters_Bcap.ipynb  
│   ├── SweepParameters_c1.ipynb  
│   ├── SweepParameters_c2.ipynb  
│   ├── SweepParameters_d.ipynb   
│   ├── SweepParameters_e.ipynb   
│   ├── SweepParameters_k.ipynb   
│   ├── output/
├── Spatial_2D_Model                # 2D implementation un COMSOL
│   ├── Heart_2D_FHN.mph   
├── requirements.txt                # Python dependencies
└── LICENSE                         # GPL-3.0 License
└── README.md                       # README.md
```
