# MicrowaveSimulation
Graphical simulation of heat transmition through microwaves
# Heat Transmission Simulation in Microwaves

This repository provides a detailed explanation and implementation of equations used to model heat transmission in materials exposed to microwave radiation. The equations are derived from Maxwell's equations and material properties, such as electrical conductivity ($\sigma$), specific heat capacity ($c$), and thickness ($h$). This document explains the theoretical foundation and offers an example framework for simulating heat transmission.

---

## **Core Equations**
### 3. **Electric Field Decay with Distance**
The electric field amplitude decays with distance $d$ from the source:

$$
E(t, r) = \hat{r} \frac{E_0 \cos(\omega t)}{r}
$$

### 2. **Power Absorption from Electric Field**
The heat absorbed by a material is related to the electric field intensity of the microwave radiation. The power per unit volume absorbed is given by:

$$
P = \frac{1}{2} \sigma E^2
$$


### 3. **Temperature Change ($\Delta T$)**
The temperature rise in the material over time is given by:

$$
\Delta T = \frac{\sigma E^2}{4 \rho h c} \\
$$
$$
\Delta T = \frac{\sigma E_0^2  \cos(\omega t)^2 }{4 \rho h c r^2}
$$

Where:
- $P$: Power absorbed per unit volume (W/m³).
- $\Delta T$: Temperature change (K).
- $\sigma$: Electrical conductivity (S/m).
- $E_0$: Electric field amplitude (V/m).
- $t$: Time of exposure (s).
- $\rho$: Material density (kg/m³).
- $h$: Thickness of the material (m).
- $c$: Specific heat capacity (J/kg·K).
- $d$: Distance between the material and the microwave source (m).

---


Feel free to contribute or suggest improvements!
