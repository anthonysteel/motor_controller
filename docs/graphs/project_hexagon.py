import numpy as np
import matplotlib.pyplot as plt
from itertools import product
from mpl_toolkits.mplot3d.art3d import Poly3DCollection, Line3DCollection

plt.rcParams['text.usetex'] = True
plt.rcParams['font.family'] = 'serif'
plt.rcParams['text.latex.preamble'] = r'\usepackage{amsmath}'

Vdc = 1.0
verts = np.array(list(product([0, Vdc], repeat=3)))

faces = [
    [(0,0,0), (Vdc,0,0), (Vdc,Vdc,0), (0,Vdc,0)],
    [(0,0,Vdc), (Vdc,0,Vdc), (Vdc,Vdc,Vdc), (0,Vdc,Vdc)],
    [(0,0,0), (Vdc,0,0), (Vdc,0,Vdc), (0,0,Vdc)],
    [(0,Vdc,0), (Vdc,Vdc,0), (Vdc,Vdc,Vdc), (0,Vdc,Vdc)],
    [(0,0,0), (0,Vdc,0), (0,Vdc,Vdc), (0,0,Vdc)],
    [(Vdc,0,0), (Vdc,Vdc,0), (Vdc,Vdc,Vdc), (Vdc,0,Vdc)]
]

edges = []
for f in faces:
    for i in range(4):
        p1 = np.array(f[i])
        p2 = np.array(f[(i+1)%4])
        edges.append((p1, p2))

fig = plt.figure(figsize=(6,6))
ax = fig.add_subplot(projection='3d')

poly3d = [[np.array(p) for p in face] for face in faces]
collection = Poly3DCollection(poly3d, facecolors='#999999', edgecolors='none', alpha=0.35)
ax.add_collection3d(collection)

edge_collection = Line3DCollection(edges, colors='#999999', linewidths=1.2)
ax.add_collection3d(edge_collection)

ax.set_xlabel(r'$v_a$', fontsize=14, labelpad=-5)
ax.set_ylabel(r'$v_b$', fontsize=14, labelpad=-5)
ax.set_zlabel(r'$v_c$', fontsize=14, labelpad=-5)

ax.set_xlim(-0.2, 1.2)
ax.set_ylim(-0.2, 1.2)
ax.set_zlim(-0.2, 1.2)

ax.set_box_aspect([1,1,1])
ax.grid(False)

ax.view_init(elev=35.264, azim=45)

ax.set_xticks([])
ax.set_yticks([])
ax.set_zticks([])

plt.tight_layout()
plt.savefig("img/project_hexagon.png", format="png", dpi=600)
plt.show()