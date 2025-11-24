import numpy as np
import matplotlib.pyplot as plt
import magpylib as mag

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
    r_hat_vec = np.array([np.cos(theta), np.sin(theta), 0.0])
    sign = 1.0 if k % 2 == 0 else -1.0
    M = sign * M_mag * r_hat_vec
    m = mag.magnet.Cuboid(
        magnetization=M,
        dimension=(dr, tangential_width, axial_length),
        position=(cx, cy, 0.0),
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

step_r = 2
step_t = 2

R_e, ThetaE = np.meshgrid(r_vals, theta_e, indexing="ij")

R_sub = R_e[::step_r, ::step_t]
ThetaE_sub = ThetaE[::step_r, ::step_t]
Br_sub = B_r_e[::step_r, ::step_t]
Bt_sub = B_t_e[::step_r, ::step_t]

X_plot = ThetaE_sub
Y_plot = R_sub

magB = np.sqrt(Br_sub**2 + Bt_sub**2)
Bmax = np.max(magB)
magB_norm = magB / Bmax

norm_mag = np.maximum(magB, 1e-12)
Br_dir = Br_sub / norm_mag
Bt_dir = Bt_sub / norm_mag

U_rotor = Bt_dir
V_rotor = Br_dir

U_stator = -U_rotor
V_stator = -V_rotor

fig, (ax_rotor, ax_stator) = plt.subplots(
    2, 1, figsize=(8, 7), sharex=True
)

Q_rotor = ax_rotor.quiver(
    X_plot,
    Y_plot,
    U_rotor,
    V_rotor,
    magB_norm,
    angles="uv",
    scale=80,
    cmap="viridis",
    width=0.0012,
)

ax_stator.quiver(
    X_plot,
    Y_plot,
    U_stator,
    V_stator,
    color="black",
    angles="uv",
    scale=80,
    width=0.0015,
)

ax_rotor.set_ylim(r_min, r_max)
ax_stator.set_ylim(r_min, r_max)

ax_rotor.set_ylabel(r"radius $r$", labelpad=10)
ax_stator.set_ylabel(r"radius $r$", labelpad=10)

xticks = [0, np.pi/2, np.pi, 3*np.pi/2, 2*np.pi]
xtick_labels = [
    r"$0$", r"$\frac{\pi}{2}$", r"$\pi$",
    r"$\frac{3\pi}{2}$", r"$2\pi$"
]

ax_stator.set_xticks(xticks)
ax_stator.set_xticklabels(xtick_labels)
ax_stator.set_xlabel(r"electrical angle $\theta_e$ (rad)")

ax_rotor.set_title("Rotor Airgap Field (coloured)")
ax_stator.set_title("Ideal Stator Field (black, antiparallel)")

plt.tight_layout()
plt.savefig("img/rotor_and_stator_magnetization.png", dpi=600, bbox_inches="tight")
plt.show()