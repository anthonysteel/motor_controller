import numpy as np
import matplotlib.pyplot as plt

plt.rcParams['text.latex.preamble'] = r'\usepackage{amsmath}'
plt.rcParams['text.usetex'] = True
plt.rcParams['font.family'] = 'serif'

fig = plt.figure(figsize=(6, 6))
ax = plt.subplot(111, projection='3d')

e1 = np.array([1.0, 0.0, 0.0])
e2 = np.array([0.0, 1.0, 0.0])
e3 = np.array([0.0, 0.0, 1.0])

ax.quiver(0, 0, 0, e1[0], e1[1], e1[2], color='C0', linewidth=2)
ax.quiver(0, 0, 0, e2[0], e2[1], e2[2], color='C0', linewidth=2)
ax.quiver(0, 0, 0, e3[0], e3[1], e3[2], color='C0', linewidth=2)

ax.text(*(1.1*e1), r'$e_1$', color='C0')
ax.text(*(1.1*e2), r'$e_2$', color='C0')
ax.text(*(1.1*e3), r'$e_3$', color='C0')

v1 = np.array([1.0, 1.0, 1.0])
u1 = v1 / np.linalg.norm(v1)
v2 = np.array([1.0, -1.0, 0.0])
v2 = v2 - np.dot(v2, u1) * u1
u2 = v2 / np.linalg.norm(v2)
u3 = np.cross(u1, u2)

ax.quiver(0, 0, 0, u1[0], u1[1], u1[2], color='C3', linewidth=2)
ax.quiver(0, 0, 0, u2[0], u2[1], u2[2], color='C3', linewidth=2)
ax.quiver(0, 0, 0, u3[0], u3[1], u3[2], color='C3', linewidth=2)

ax.text(*(1.1*u1), r'$u_1$', color='C3')
ax.text(*(1.1*u2), r'$u_2$', color='C3')
ax.text(*(1.1*u3), r'$u_3$', color='C3')

ax.set_xlabel(r'$v_a$', labelpad=-5)
ax.set_ylabel(r'$v_b$', labelpad=-5)
ax.set_zlabel(r'$v_c$', labelpad=-5)

ax.set_xlim(-1.5, 1.5)
ax.set_ylim(-1.5, 1.5)
ax.set_zlim(-1.5, 1.5)

ax.set_box_aspect([1, 1, 1])
ax.grid(False)
ax.view_init(elev=20, azim=30)

ax.set_xticks([])
ax.set_yticks([])
ax.set_zticks([])

plt.tight_layout()
plt.savefig("rotated_basis_vectors.png", dpi=300)
plt.show()