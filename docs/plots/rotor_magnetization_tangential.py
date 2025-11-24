import numpy as np
import matplotlib.pyplot as plt
import magpylib as mag
from matplotlib.colors import ListedColormap

plt.rcParams['text.latex.preamble'] = r'\usepackage{amsmath}'
plt.rcParams['text.usetex'] = True
plt.rcParams['font.family'] = 'serif'

rotor_radius = 1.0
n_poles = 24
pole_pairs = n_poles // 2

mag_inner = 0.65 * rotor_radius
mag_outer = 0.95 * rotor_radius
r_mid = 0.5 * (mag_inner + mag_outer)
dr = mag_outer - mag_inner

pole_arc = 2 * np.pi / n_poles
tangential_width = rotor_radius * pole_arc
axial_length = 1.0
M_mag = 1e6

magnets = []
for k in range(n_poles):
    theta = k * pole_arc
    cx = r_mid * np.cos(theta)
    cy = r_mid * np.sin(theta)
    cz = 0.0
    r_hat_vec = np.array([np.cos(theta), np.sin(theta), 0.0])
    sign = 1.0 if k % 2 == 0 else -1.0
    M = sign * M_mag * r_hat_vec
    m = mag.magnet.Cuboid(
        magnetization=M,
        dimension=(dr, tangential_width, axial_length),
        position=(cx, cy, cz),
    )
    magnets.append(m)

coll = mag.Collection(magnets)

r_min = rotor_radius
r_max = 1.35 * rotor_radius

n_r = 40
n_th = 960

r_vals = np.linspace(r_min, r_max, n_r)
theta_m = np.linspace(0.0, 2 * np.pi, n_th, endpoint=False)

R, TH = np.meshgrid(r_vals, theta_m, indexing="ij")
X = R * np.cos(TH)
Y = R * np.sin(TH)
Z = np.zeros_like(X)

pos = np.stack([X, Y, Z], axis=-1).reshape(-1, 3)
B = coll.getB(pos).reshape(n_r, n_th, 3)

r_hat = np.stack([np.cos(TH), np.sin(TH), np.zeros_like(TH)], axis=-1)
t_hat = np.stack([-np.sin(TH), np.cos(TH), np.zeros_like(TH)], axis=-1)

B_r = np.einsum("ijk,ijk->ij", B, r_hat)
B_t = np.einsum("ijk,ijk->ij", B, t_hat)

n_th_elec = n_th // pole_pairs
theta_e = np.linspace(0.0, 2 * np.pi, n_th_elec, endpoint=False)

B_r_e = B_r[:, :n_th_elec]
B_t_e = B_t[:, :n_th_elec]

R_e, ThetaE = np.meshgrid(r_vals, theta_e, indexing="ij")

step_r = 2
step_t = 2

R_sub = R_e[::step_r, ::step_t]
ThetaE_sub = ThetaE[::step_r, ::step_t]
Bt_sub = B_t_e[::step_r, ::step_t]

X_plot = ThetaE_sub
Y_plot = R_sub

magBt = np.abs(Bt_sub)
Bt_max = np.max(magBt)
magBt_norm = magBt / Bt_max

norm_t = np.maximum(magBt, 1e-12)
Bt_dir = Bt_sub / norm_t

U = Bt_dir
V = 0.0 * Bt_dir   # no radial component

# -----------------------------
#   Single-subplot figure
# -----------------------------

fig, ax1 = plt.subplots(
    1, 1, figsize=(8, 3.6)
)

Q = ax1.quiver(
    X_plot,
    Y_plot,
    U,
    V,
    magBt_norm,
    angles="uv",
    scale=80,
    cmap="viridis",
    width=0.0012,
)

ax1.set_xlim(0, 2 * np.pi)
ax1.set_ylim(r_min, r_max)

xticks = [0, np.pi/2, np.pi, 3*np.pi/2, 2*np.pi]
xtick_labels = [
    r"$0$", r"$\frac{\pi}{2}$", r"$\pi$",
    r"$\frac{3\pi}{2}$", r"$2\pi$"
]
ax1.set_xticks(xticks)
ax1.set_xticklabels(xtick_labels)
ax1.set_xlabel(r"electrical angle $\theta_e$ (rad)")
ax1.set_ylabel(r"normalized radius $r/r_{\mathrm{rotor}}$", labelpad=10)

plt.tight_layout(rect=[0.0, 0.0, 0.9, 1.0])

cbar_ax = fig.add_axes([0.92, 0.15, 0.02, 0.7])
cbar = fig.colorbar(Q, cax=cbar_ax)
cbar.set_label(r"normalized $|B_t|$")

plt.savefig("img/rotor_tangential.png", dpi=600, bbox_inches="tight")
plt.show()