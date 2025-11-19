import numpy as np
import matplotlib.pyplot as plt
from itertools import product

plt.rcParams['text.usetex'] = True
plt.rcParams['font.family'] = 'serif'
plt.rcParams['text.latex.preamble'] = r'\usepackage{amsmath}'

Vdc = 1.0
pts = np.array(list(product([0, Vdc], repeat=3)))

fig = plt.figure(figsize=(6,6))
ax = fig.add_subplot(projection='3d')

ax.scatter(pts[:,0], pts[:,1], pts[:,2], s=60, color='black')

ax.set_xlabel(r'$v_a$', fontsize=14, labelpad=-5)
ax.set_ylabel(r'$v_b$', fontsize=14, labelpad=-5)
ax.set_zlabel(r'$v_c$', fontsize=14, labelpad=-5)

ax.set_xlim(-0.2, 1.2)
ax.set_ylim(-0.2, 1.2)
ax.set_zlim(-0.2, 1.2)

ax.set_box_aspect([1,1,1])
ax.grid(False)
ax.view_init(elev=20, azim=30)

ax.set_xticks([])
ax.set_yticks([])
ax.set_zticks([])

plt.tight_layout()

plt.savefig("img/cube_vertices.png", format="png", dpi=600, bbox_inches="tight")

plt.show()