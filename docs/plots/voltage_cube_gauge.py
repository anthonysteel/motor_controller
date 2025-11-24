import numpy as np
import matplotlib.pyplot as plt
from itertools import product

plt.rcParams['text.latex.preamble'] = r'\usepackage{amsmath}'
plt.rcParams['text.usetex'] = True
plt.rcParams['font.family'] = 'serif'

Vdc = 1.0

pts = np.array(list(product([0, Vdc], repeat=3)))

rng = np.random.default_rng(123)
dir_vec = np.array([1.0, 1.0, 1.0])
dir_unit = dir_vec / np.linalg.norm(dir_vec)

mag = rng.uniform(0.2, 0.6, size=pts.shape[0])
sign = rng.choice([-1.0, 1.0], size=pts.shape[0])
t = mag * sign

pts_rand = pts + t[:, None] * dir_unit[None, :]

fig = plt.figure(figsize=(6,6))
ax = fig.add_subplot(projection='3d')

ax.scatter(pts[:,0], pts[:,1], pts[:,2],
           s=60, color='#003f7f', label=r'original cube')

ax.scatter(pts_rand[:,0], pts_rand[:,1], pts_rand[:,2],
           s=60, color='#e67e22', label=r'randomly shifted along $(1,1,1)$')

all_pts = np.vstack([pts, pts_rand])
xmin, ymin, zmin = all_pts.min(axis=0) - 0.2
xmax, ymax, zmax = all_pts.max(axis=0) + 0.2

ax.set_xlabel(r'$v_a$', fontsize=14, labelpad=-5)
ax.set_ylabel(r'$v_b$', fontsize=14, labelpad=-5)
ax.set_zlabel(r'$v_c$', fontsize=14, labelpad=-5)

ax.set_xlim(xmin, xmax)
ax.set_ylim(ymin, ymax)
ax.set_zlim(zmin, zmax)

ax.set_box_aspect([1,1,1])
ax.grid(False)
ax.view_init(elev=20, azim=20)

ax.set_xticks([])
ax.set_yticks([])
ax.set_zticks([])

#ax.legend(loc='center left', bbox_to_anchor=(1.02, 0.5))

plt.tight_layout()
plt.savefig("img/full_gauge_transform.png",
            format="png", dpi=600, bbox_inches="tight")
plt.show()