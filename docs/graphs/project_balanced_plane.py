import numpy as np
import matplotlib.pyplot as plt
from itertools import product
from mpl_toolkits.mplot3d.art3d import Poly3DCollection, Line3DCollection

plt.rcParams['text.usetex'] = True
plt.rcParams['font.family'] = 'serif'
plt.rcParams['text.latex.preamble'] = r'\usepackage{amsmath}'

Vdc = 1.0

# Raw cube vertices in (v_a, v_b, v_c)
verts = np.array(list(product([0.0, Vdc], repeat=3)))

# Projection onto balanced plane (normal n = (1,1,1))
n = np.array([1.0, 1.0, 1.0])
u = n / np.linalg.norm(n)

def project_to_plane(v):
    return v - np.dot(v, u) * u

proj_verts = np.array([project_to_plane(v) for v in verts])

# Balanced plane patch
plane_pts = np.array([
    project_to_plane(np.array([ 1.0,  0.0,  0.0])),
    project_to_plane(np.array([ 0.0,  1.0,  0.0])),
    project_to_plane(np.array([ 0.0,  0.0,  1.0])),
    project_to_plane(np.array([-1.0,  0.0,  0.0])),
    project_to_plane(np.array([ 0.0, -1.0,  0.0])),
    project_to_plane(np.array([ 0.0,  0.0, -1.0]))
])

# Cube faces
cube_faces = [
    [(0,0,0), (Vdc,0,0), (Vdc,Vdc,0), (0,Vdc,0)],
    [(0,0,Vdc), (Vdc,0,Vdc), (Vdc,Vdc,Vdc), (0,Vdc,Vdc)],
    [(0,0,0), (Vdc,0,0), (Vdc,0,Vdc), (0,0,Vdc)],
    [(0,Vdc,0), (Vdc,Vdc,0), (Vdc,Vdc,Vdc), (0,Vdc,Vdc)],
    [(0,0,0), (0,Vdc,0), (0,Vdc,Vdc), (0,0,Vdc)],
    [(Vdc,0,0), (Vdc,Vdc,0), (Vdc,Vdc,Vdc), (Vdc,0,Vdc)]
]

# Unique projected vertices and hexagon ordering
unique = np.unique(np.round(proj_verts, 6), axis=0)
nonzero = np.array([p for p in unique if not np.allclose(p, 0.0)])

# Choose basis in plane for ordering
e1 = np.array([1.0, -1.0, 0.0])
e1 /= np.linalg.norm(e1)
e2 = np.cross(u, e1)

angles = []
for p in nonzero:
    c1 = np.dot(p, e1)
    c2 = np.dot(p, e2)
    angles.append(np.arctan2(c2, c1))
order = np.argsort(angles)
hex_pts = nonzero[order]

fig = plt.figure(figsize=(7,7))
ax = fig.add_subplot(projection='3d')

# Cube faces (light blue)
cube_poly = Poly3DCollection([[np.array(p) for p in f] for f in cube_faces],
                             facecolors='#a8c6ff',
                             edgecolors='none',
                             alpha=0.35)
ax.add_collection3d(cube_poly)

# Hexagon face (orange) on plane
hex_face = Poly3DCollection([hex_pts],
                            facecolors='#ffb347',
                            edgecolors='black',
                            linewidths=1.2,
                            alpha=0.8)
ax.add_collection3d(hex_face)

# Original cube vertices (dark blue)
ax.scatter(verts[:,0], verts[:,1], verts[:,2],
           color='#003f7f', s=45, zorder=5)

# Projected vertices (black)
ax.scatter(proj_verts[:,0], proj_verts[:,1], proj_verts[:,2],
           color='black', s=45, zorder=6)

# Ambient basis vectors e_a, e_b, e_c
e_a = np.array([1.0, 0.0, 0.0])*0.75
e_b = np.array([0.0, 1.0, 0.0])*0.75
e_c = np.array([0.0, 0.0, 1.0])*0.75

ax.quiver(0, 0, 0, e_a[0], e_a[1], e_a[2],
          color='black', arrow_length_ratio=0.1)
ax.quiver(0, 0, 0, e_b[0], e_b[1], e_b[2],
          color='black', arrow_length_ratio=0.1)
ax.quiver(0, 0, 0, e_c[0], e_c[1], e_c[2],
          color='black', arrow_length_ratio=0.1)

ax.text(1.05, 0.0, 0.0, r'$\hat v_a$', fontsize=11)
ax.text(0.0, 1.05, 0.0, r'$\hat v_b$', fontsize=11)
ax.text(0.0, 0.0, 1.05, r'$\hat v_c$', fontsize=11)

# Projected basis vectors (v_aN, v_bN, v_cN)
e_aN = project_to_plane(e_a)
e_bN = project_to_plane(e_b)
e_cN = project_to_plane(e_c)

# Optionally normalize to a nice length
def normalize(v, L=0.7):
    n = np.linalg.norm(v)
    return (L / n) * v if n > 1e-9 else v

e_aN_plot = normalize(e_aN)*0.75
e_bN_plot = normalize(e_bN)*0.75
e_cN_plot = normalize(e_cN)*0.75

ax.quiver(0, 0, 0, e_aN_plot[0], e_aN_plot[1], e_aN_plot[2],
          color='red', arrow_length_ratio=0.1)
ax.quiver(0, 0, 0, e_bN_plot[0], e_bN_plot[1], e_bN_plot[2],
          color='red', arrow_length_ratio=0.1)
ax.quiver(0, 0, 0, e_cN_plot[0], e_cN_plot[1], e_cN_plot[2],
          color='red', arrow_length_ratio=0.1)

ax.text(e_aN_plot[0]*1.1, e_aN_plot[1]*1.1, e_aN_plot[2]*1.1,
        r'$\hat v_{aN}$', color='red', fontsize=11)
ax.text(e_bN_plot[0]*1.1, e_bN_plot[1]*1.1, e_bN_plot[2]*1.1,
        r'$\hat v_{bN}$', color='red', fontsize=11)
ax.text(e_cN_plot[0]*1.1, e_cN_plot[1]*1.1, e_cN_plot[2]*1.1,
        r'$\hat v_{cN}$', color='red', fontsize=11)

# Axes styling
ax.set_xlabel(r'$v_a$', fontsize=14, labelpad=-5)
ax.set_ylabel(r'$v_b$', fontsize=14, labelpad=-5)
ax.set_zlabel(r'$v_c$', fontsize=14, labelpad=-5)

ax.set_xlim(-0.8, 1.2)
ax.set_ylim(-0.8, 1.2)
ax.set_zlim(-0.8, 1.2)
ax.set_box_aspect([1,1,1])
ax.grid(False)
ax.view_init(elev=25, azim=-25)
ax.set_xticks([]); ax.set_yticks([]); ax.set_zticks([])

plt.tight_layout()
plt.savefig("img/cube_projection_with_bases.png", dpi=600)
plt.show()