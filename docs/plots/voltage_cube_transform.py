import numpy as np
import matplotlib.pyplot as plt
from itertools import product

plt.rcParams['text.latex.preamble'] = r'\usepackage{amsmath}'
plt.rcParams['text.usetex'] = True
plt.rcParams['font.family'] = 'serif'

Vdc = 1.0

pts = np.array(list(product([0, Vdc], repeat=3)))
special = np.array([1.0, 1.0, 0.0])

mask = ~np.all(pts == special, axis=1)
pts_plot = pts[mask]

fig = plt.figure(figsize=(6,6))
ax = fig.add_subplot(projection='3d')

ax.scatter(pts_plot[:,0], pts_plot[:,1], pts_plot[:,2],
           s=60, color='#003f7f')

ax.scatter(special[0], special[1], special[2],
           s=80, color="#f7d5b7")

dir_vec = np.array([1.0, 1.0, 1.0])
dir_unit = dir_vec / np.linalg.norm(dir_vec)

t = 0.5
proj = special + t * dir_unit

ax.scatter(proj[0], proj[1], proj[2],
           s=80, color='#e67e22')

disp = proj - special
arrow_vec = 0.25 * disp
midpoint = 0.5 * (special + proj)

ax.quiver(midpoint[0], midpoint[1], midpoint[2],
          arrow_vec[0], arrow_vec[1], arrow_vec[2],
          color='black', arrow_length_ratio=0.5)

ax.set_xlabel(r'$v_a$', fontsize=14, labelpad=-5)
ax.set_ylabel(r'$v_b$', fontsize=14, labelpad=-5)
ax.set_zlabel(r'$v_c$', fontsize=14, labelpad=-5)

ax.set_xlim(-0.2, 1.2)
ax.set_ylim(-0.2, 1.2)
ax.set_zlim(-0.2, 1.2)

ax.set_box_aspect([1,1,1])
ax.grid(False)
ax.view_init(elev=20, azim=20)

ax.set_xticks([])
ax.set_yticks([])
ax.set_zticks([])

plt.tight_layout()
plt.savefig("img/common_mode_transform.png",
            format="png", dpi=600, bbox_inches="tight")
plt.show()