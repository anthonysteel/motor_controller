import numpy as np
import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle

plt.rcParams['text.latex.preamble'] = r'\usepackage{amsmath}'
plt.rcParams['text.usetex'] = True
plt.rcParams['font.family'] = 'serif'

fig, ax = plt.subplots(figsize=(8, 2))

n_poles_mech = 24
pole_pairs = 12

y0 = 0.25
thickness = 0.5

theta_mech_per_pole = 2 * np.pi / n_poles_mech
theta_elec_per_pole = pole_pairs * theta_mech_per_pole

n_poles_elec_period = n_poles_mech // pole_pairs

for k in range(n_poles_elec_period):
    x_elec = k * theta_elec_per_pole
    color = '#e74c3c' if k % 2 == 0 else '#3498db'
    rect = Rectangle((x_elec, y0), theta_elec_per_pole, thickness,
                     facecolor=color, edgecolor='black', linewidth=0.5)
    ax.add_patch(rect)

ax.set_xlim(0, 2 * np.pi)
ax.set_ylim(0, 1.0)
ax.set_aspect('auto')

xticks = [0, np.pi/2, np.pi, 3*np.pi/2, 2*np.pi]
xtick_labels = [r'$0$', r'$\frac{\pi}{2}$', r'$\pi$',
                r'$\frac{3\pi}{2}$', r'$2\pi$']
ax.set_xticks(xticks)
ax.set_xticklabels(xtick_labels)

ax.set_yticks([])

ax.set_xlabel(r'electrical angle $\theta_e$ (rad)')

plt.tight_layout()
plt.savefig("img/rotor_unrolled_electrical_angle.png", format="png", dpi=600, bbox_inches="tight")
plt.show()