import numpy as np
import matplotlib.pyplot as plt
from itertools import product
from mpl_toolkits.mplot3d.art3d import Poly3DCollection, Line3DCollection

plt.rcParams['text.usetex'] = True
plt.rcParams['font.family'] = 'serif'
plt.rcParams['text.latex.preamble'] = r'\usepackage{amsmath}'

Vdc = 1.0

verts = np.array(list(product([0.0, Vdc], repeat=3)))

n = np.array([1.0, 1.0, 1.0])
u = n / np.linalg.norm(n)

center = np.array([Vdc/2, Vdc/2, Vdc/2])

def project_to_plane(v):
    return v - np.dot(v - center, u) * u

proj_verts = np.array([project_to_plane(v) for v in verts])

plane_pts = np.array([
    project_to_plane(np.array([ 1.0,  0.0,  0.0])),
    project_to_plane(np.array([ 0.0,  1.0,  0.0])),
    project_to_plane(np.array([ 0.0,  0.0,  1.0])),
    project_to_plane(np.array([-1.0,  0.0,  0.0])),
    project_to_plane(np.array([ 0.0, -1.0,  0.0])),
    project_to_plane(np.array([ 0.0,  0.0, -1.0]))
])

# compute scale factor so rays from center hit cube faces
rel = proj_verts - center
s_candidates = []
for r in rel:
    for k in range(3):
        if r[k] > 0:
            s_candidates.append((Vdc/2) / r[k])
        elif r[k] < 0:
            s_candidates.append((-Vdc/2) / r[k])
s = min(s_candidates)

proj_verts = center + s * rel
plane_pts  = center + s * (plane_pts - center)

cube_faces = [
    [(0,0,0), (Vdc,0,0), (Vdc,Vdc,0), (0,Vdc,0)],
    [(0,0,Vdc), (Vdc,0,Vdc), (Vdc,Vdc,Vdc), (0,Vdc,Vdc)],
    [(0,0,0), (Vdc,0,0), (Vdc,0,Vdc), (0,0,Vdc)],
    [(0,Vdc,0), (Vdc,Vdc,0), (Vdc,Vdc,Vdc), (0,Vdc,Vdc)],
    [(0,0,0), (0,Vdc,0), (0,Vdc,Vdc), (0,0,Vdc)],
    [(Vdc,0,0), (Vdc,Vdc,0), (Vdc,Vdc,Vdc), (Vdc,0,Vdc)]
]

unique = np.unique(np.round(proj_verts, 6), axis=0)
nonzero = np.array([p for p in unique if not np.allclose(p, center)])

e1 = np.array([1.0, -1.0, 0.0])
e1 /= np.linalg.norm(e1)
e2 = np.cross(u, e1)

angles = []
for p in nonzero:
    p_rel = p - center
    c1 = np.dot(p_rel, e1)
    c2 = np.dot(p_rel, e2)
    angles.append(np.arctan2(c2, c1))
order = np.argsort(angles)
hex_pts = nonzero[order]

fig = plt.figure(figsize=(7,7))
ax = fig.add_subplot(projection='3d')

cube_poly = Poly3DCollection([[np.array(p) for p in f] for f in cube_faces],
                             facecolors='#a8c6ff',
                             edgecolors='none',
                             alpha=0.35)
ax.add_collection3d(cube_poly)

plane_poly = Poly3DCollection([plane_pts],
                              facecolors='#dddddd',
                              edgecolors='none',
                              alpha=0.25)
ax.add_collection3d(plane_poly)

hex_face = Poly3DCollection([hex_pts],
                            facecolors='#ffb347',
                            edgecolors='black',
                            linewidths=1.2,
                            alpha=0.8)
ax.add_collection3d(hex_face)

ax.scatter(verts[:,0], verts[:,1], verts[:,2],
           color='#003f7f', s=45, zorder=5)

ax.scatter(proj_verts[:,0], proj_verts[:,1], proj_verts[:,2],
           color="#ff9500", s=45, zorder=6)

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

def project_direction(v):
    return v - np.dot(v, u) * u

e_aN_dir = project_direction(e_a)
e_bN_dir = project_direction(e_b)
e_cN_dir = project_direction(e_c)

def normalize(v, L=0.7):
    nrm = np.linalg.norm(v)
    return (L / nrm) * v if nrm > 1e-9 else v

e_aN_plot = normalize(e_aN_dir)*0.75*s
e_bN_plot = normalize(e_bN_dir)*0.75*s
e_cN_plot = normalize(e_cN_dir)*0.75*s

ax.quiver(center[0], center[1], center[2],
          e_aN_plot[0], e_aN_plot[1], e_aN_plot[2],
          color='red', arrow_length_ratio=0.1)
ax.quiver(center[0], center[1], center[2],
          e_bN_plot[0], e_bN_plot[1], e_bN_plot[2],
          color='red', arrow_length_ratio=0.1)
ax.quiver(center[0], center[1], center[2],
          e_cN_plot[0], e_cN_plot[1], e_cN_plot[2],
          color='red', arrow_length_ratio=0.1)

ax.text(*(center + 1.1*e_aN_plot), r'$\hat v_{aN}$', color='red', fontsize=11)
ax.text(*(center + 1.1*e_bN_plot), r'$\hat v_{bN}$', color='red', fontsize=11)
ax.text(*(center + 1.1*e_cN_plot), r'$\hat v_{cN}$', color='red', fontsize=11)

ax.set_xlabel(r'$v_a$', fontsize=14, labelpad=-5)
ax.set_ylabel(r'$v_b$', fontsize=14, labelpad=-5)
ax.set_zlabel(r'$v_c$', fontsize=14, labelpad=-5)

ax.set_xlim(-0.2, 1.2)
ax.set_ylim(-0.2, 1.2)
ax.set_zlim(-0.2, 1.2)
ax.set_box_aspect([1,1,1])
ax.grid(False)
ax.view_init(elev=15, azim=-5)
ax.set_xticks([]); ax.set_yticks([]); ax.set_zticks([])

plt.tight_layout()
plt.savefig("img/cube_projection_with_bases.png", dpi=600)
plt.show()