import numpy as np
import matplotlib.pyplot as plt
from matplotlib.patches import Wedge

plt.rcParams['text.latex.preamble'] = r'\usepackage{amsmath}'
plt.rcParams['text.usetex'] = True
plt.rcParams['font.family'] = 'serif'

fig, ax = plt.subplots(figsize=(6,6))

rotor_radius = 1.0
n_poles = 24
mag_inner = 0.65 * rotor_radius
mag_outer = 0.95 * rotor_radius

for k in range(n_poles):
    theta1 = 360.0 * k / n_poles - 360.0/(2*n_poles)
    theta2 = 360.0 * (k+1) / n_poles - 360.0/(2*n_poles)
    color = "#e74c3c" if k % 2 == 0 else "#3498db"
    mag = Wedge((0,0), mag_outer, theta1, theta2,
                width=mag_outer-mag_inner,
                facecolor=color, edgecolor='black', linewidth=1.0)
    ax.add_patch(mag)

ax.set_xlim(-1.2*rotor_radius, 1.2*rotor_radius)
ax.set_ylim(-1.2*rotor_radius, 1.2*rotor_radius)
ax.set_aspect('equal')
ax.axis('off')

plt.tight_layout()
plt.savefig("img/rotor.png", format="png", dpi=600, bbox_inches="tight")
plt.show()