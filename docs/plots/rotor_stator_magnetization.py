import numpy as np
import matplotlib.pyplot as plt
import magpylib as mag

plt.rcParams['text.latex.preamble'] = r'\usepackage{amsmath}'
plt.rcParams['text.usetex'] = True
plt.rcParams['font.family'] = 'serif'

rotor_radius = 1.0
n_poles = 24
pole_pairs = n_poles // 2

stator_radius = 1.35 * rotor_radius
axial_length = 1.0
I0 = 1e5
n_slots = 6 * pole_pairs * 2

alpha_elec = 0.0

stator_currents = []
for k in range(n_slots):
    theta = 2 * np.pi * k / n_slots
    I_k = I0 * np.cos(pole_pairs * theta - alpha_elec)
    x = stator_radius * np.cos(theta)
    y = stator_radius * np.sin(theta)
    line = mag.current.Line(
        current=I_k,
        vertices=[
            (x, y, -axial_length / 2),
            (x, y, axial_length / 2),
        ],
    )
    stator_currents.append(line)

coll = mag.Collection(stator_currents)

r_min = rotor_radius
r_max = stator_radius

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
V = 0.0 * Bt_dir

fig, ax1 = plt.subplots(1, 1, figsize=(8, 3.6))

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

xticks = [0, np.pi / 2, np.pi, 3 * np.pi / 2, 2 * np.pi]
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
cbar.set_label(r"normalized $|B_{t,\mathrm{stator}}|$")

plt.savefig("img/stator_tangential_field.png", dpi=600, bbox_inches="tight")
plt.show()