import numpy as np
import matplotlib.pyplot as plt
import magpylib as mag

plt.rcParams['text.latex.preamble'] = r'\usepackage{amsmath}'
plt.rcParams['text.usetex'] = True
plt.rcParams['font.family'] = 'serif'

rotor_radius = 1.0
n_poles = 24
pole_pairs = n_poles // 2  # p = 12

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

B_t = np.einsum("ijk,ijk->ij", B, t_hat)

n_th_elec = n_th // pole_pairs
theta_e = np.linspace(0.0, 2 * np.pi, n_th_elec, endpoint=False)

B_t_e = B_t[:, :n_th_elec]

# -----------------------------------------------------
# Choose slice at θe = π/2
# -----------------------------------------------------
target_angle = np.pi / 2
idx_theta = np.argmin(np.abs(theta_e - target_angle))

Bt_slice = B_t_e[:, idx_theta].copy()

# Make it positive at rotor surface
if Bt_slice[0] < 0:
    Bt_slice = -Bt_slice

# Normalize
Bt_norm = Bt_slice / np.max(Bt_slice)
r_norm = r_vals / rotor_radius

# -----------------------------------------------------
# Theoretical decay: (r/r0)^-(p+1)
# -----------------------------------------------------
p = pole_pairs
Bt_theory = r_norm ** (-(p + 1))
Bt_theory_norm = Bt_theory / Bt_theory[0]

# -----------------------------------------------------
# Compute radius where 90% of area under Bt_norm is inside
# -----------------------------------------------------
x = r_norm
y = Bt_norm

# total area under curve
area_total = np.trapz(y, x)

# cumulative area using trapezoids
segment_areas = 0.5 * (y[1:] + y[:-1]) * (x[1:] - x[:-1])
cum_area = np.concatenate(([0.0], np.cumsum(segment_areas)))

target_area = 0.9 * area_total

idx = np.searchsorted(cum_area, target_area)

if idx == 0:
    r90_norm = x[0]
elif idx >= len(x):
    r90_norm = x[-1]
else:
    # linear interpolation within segment
    a0 = cum_area[idx - 1]
    a1 = cum_area[idx]
    x0 = x[idx - 1]
    x1 = x[idx]
    frac = (target_area - a0) / (a1 - a0)
    r90_norm = x0 + frac * (x1 - x0)

# -----------------------------------------------------
# Plot
# -----------------------------------------------------
fig, ax = plt.subplots(1, 1, figsize=(6, 3.5))

ax.plot(r_norm, Bt_norm, label=r"simulated")
ax.plot(r_norm, Bt_theory_norm, "--",
        label=fr"theoretical")

print(r90_norm)
#ax.axvline(r90_norm, linestyle=":", label=r"90\% flux")

ax.set_xlabel(r"normalized radius $r/r_{\mathrm{rotor}}$")
ax.set_ylabel(r"normalized $B_t$")
ax.set_title(r"Radial decay of $B_t$ at $\theta_e=\frac{\pi}{2}$")
ax.grid(True, linestyle=":", linewidth=0.5)
ax.legend()

plt.tight_layout()
plt.savefig("img/rotor_radial_decay.png", dpi=600)
plt.show()