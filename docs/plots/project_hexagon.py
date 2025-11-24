import numpy as np
import matplotlib.pyplot as plt
from itertools import product

plt.rcParams['text.latex.preamble'] = r'\usepackage{amsmath}'
plt.rcParams['text.usetex'] = True
plt.rcParams['font.family'] = 'serif'

Vdc = 1.0

states = np.array([
    [1.0, 0.0, 0.0],
    [1.0, 1.0, 0.0],
    [0.0, 1.0, 0.0],
    [0.0, 1.0, 1.0],
    [0.0, 0.0, 1.0],
    [1.0, 0.0, 1.0],
])

va = states[:, 0] * Vdc
vb = states[:, 1] * Vdc
vc = states[:, 2] * Vdc
v_n = (va + vb + vc) / 3.0
va_n = va - v_n
vb_n = vb - v_n
vc_n = vc - v_n

hex_pts = np.stack([va_n, vb_n, vc_n], axis=1)

fig = plt.figure(figsize=(6, 6))
ax = fig.add_subplot(111, projection='3d')

ax.scatter(hex_pts[:, 0], hex_pts[:, 1], hex_pts[:, 2],
           color='#e67e22', s=40)
ax.scatter(0.0, 0.0, 0.0,
           color='#e67e22', s=40)

va_grid = np.linspace(-1.0, 1.0, 10)
vb_grid = np.linspace(-1.0, 1.0, 10)
VA, VB = np.meshgrid(va_grid, vb_grid)
VC = - (VA + VB)
ax.plot_surface(VA, VB, VC, alpha=0.2, color='#e67e22', edgecolor='none')

ax.set_xlabel(r'$v_a$', fontsize=14)
ax.set_ylabel(r'$v_b$', fontsize=14)
ax.set_zlabel(r'$v_c$', fontsize=14)

all_pts = np.vstack([hex_pts, np.array([[0.0, 0.0, 0.0]])])
xmin, ymin, zmin = all_pts.min(axis=0) - 0.2
xmax, ymax, zmax = all_pts.max(axis=0) + 0.2
ax.set_xlim(xmin, xmax)
ax.set_ylim(ymin, ymax)
ax.set_zlim(zmin, zmax)

ax.set_box_aspect([1, 1, 1])
ax.grid(False)
ax.view_init(elev=20, azim=20)
ax.set_xticks([])
ax.set_yticks([])
ax.set_zticks([])

plt.tight_layout()
plt.savefig("img/project_hexagon.png", format="png", dpi=600, bbox_inches="tight")
plt.show()