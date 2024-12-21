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

Where:
- $P$: Power absorbed per unit volume (W/m³).
- $\sigma$: Electrical conductivity of the material (S/m).
- $E$: Electric field amplitude (V/m).

---

### 3. **Temperature Change ($\Delta T$)**
The temperature rise in the material over time is given by:

$$
\Delta T = \frac{\sigma E^2}{4 \rho h c} \\
$$
$$
\Delta T = \frac{\sigma E_0^2  \cos(\omega t)^2 }{4 \rho h c r^2}
$$

Where:
- $\Delta T$: Temperature change (K).
- $\sigma$: Electrical conductivity (S/m).
- $E_0$: Electric field amplitude (V/m).
- $t$: Time of exposure (s).
- $\rho$: Material density (kg/m³).
- $h$: Thickness of the material (m).
- $c$: Specific heat capacity (J/kg·K).
- $d$: Distance between the material and the microwave source (m).



This decay factor ensures the field weakens as distance increases, influencing the absorbed heat accordingly.

---

## **Steps for Simulation**

### 1. **Material Properties**
Gather material-specific properties:
- Electrical conductivity ($\sigma$)
- Specific heat capacity ($c$)
- Density ($\rho$)
- Thickness ($h$)

Refer to the provided CSV file for common materials' properties.

### 2. **Mesh Division for Complex Shapes**
For irregular objects, divide the object into smaller elements (e.g., triangles or voxels). Compute $\Delta T$ for each element based on its exposure to the microwave source.

### 3. **Iterative Simulation**
Repeat the calculations over time to simulate dynamic temperature changes:
- Update $T(t) = T(t-1) + \Delta T$.
- Adjust $E_0$ for moving or rotating objects if applicable.

---

## **Numerical Examples**

### Example 1: Thin Aluminum Foil
- $\sigma = 3.77 \times 10^7 \; \text{S/m}$
- $\rho = 2.7 \times 10^3 \; \text{kg/m}^3$
- $c = 900 \; \text{J/kg·K}$
- $h = 10^{-4} \; \text{m}$
- $E_0 = 1 \; \text{V/m}$
- $d = 0.1 \; \text{m}$
- $t = 1 \; \text{s}$

$$
\Delta T = \frac{(3.77 \times 10^7) (1)^2 (1)}{4 (2.7 \times 10^3) (10^{-4}) (900) (0.1)^2} \approx 3.88 \times 10^3 \; \text{K}.
$$

### Example 2: Water Layer
- $\sigma = 5.5 \times 10^{-6} \; \text{S/m}$
- $\rho = 1 \times 10^3 \; \text{kg/m}^3$
- $c = 4186 \; \text{J/kg·K}$
- $h = 0.01 \; \text{m}$
- $E_0 = 10 \; \text{V/m}$
- $d = 0.2 \; \text{m}$
- $t = 1 \; \text{s}$

$$
\Delta T = \frac{(5.5 \times 10^{-6}) (10)^2 (1)}{4 (1 \times 10^3) (0.01) (4186) (0.2)^2} \approx 8.21 \times 10^{-5} \; \text{K}.
$$

---

## **Applications**

- Microwave heating simulations for materials science.
- Thermal analysis of biological tissues.
- Optimizing industrial microwave processes.

---

## **How to Use**

1. Clone this repository.
2. Use the provided CSV file to input material properties.
3. Run the simulation script with desired parameters.

---

Feel free to contribute or suggest improvements!

\[
\Delta T = \frac{\sigma E_0^2 t}{4 \rho h c d^2}
\]

Where:
- \( \Delta T \): Temperature change (K).
- \( \sigma \): Electrical conductivity (S/m).
- \( E_0 \): Electric field amplitude (V/m).
- \( t \): Time of exposure (s).
- \( \rho \): Material density (kg/m³).
- \( h \): Thickness of the material (m).
- \( c \): Specific heat capacity (J/kg·K).
- \( d \): Distance between the material and the microwave source (m).

### 3. **Electric Field Decay with Distance**
The electric field amplitude decays with distance \(d\) from the source:

\[
E_0(d) = \frac{E_{\text{source}}}{d}
\]

This decay factor ensures the field weakens as distance increases, influencing the absorbed heat accordingly.

---

## **Numerical Examples**

### Example 1: Thin Aluminum Foil
- \( \sigma = 3.77 \times 10^7 \; \text{S/m} \)
- \( \rho = 2.7 \times 10^3 \; \text{kg/m}^3 \)
- \( c = 900 \; \text{J/kg·K} \)
- \( h = 10^{-4} \; \text{m} \)
- \( E_0 = 1 \; \text{V/m} \)
- \( d = 0.1 \; \text{m} \)
- \( t = 1 \; \text{s} \)

\[
\Delta T = \frac{(3.77 \times 10^7) (1)^2 (1)}{4 (2.7 \times 10^3) (10^{-4}) (900) (0.1)^2} \approx 3.88 \times 10^3 \; \text{K}.
\]

### Example 2: Water Layer
- \( \sigma = 5.5 \times 10^{-6} \; \text{S/m} \)
- \( \rho = 1 \times 10^3 \; \text{kg/m}^3 \)
- \( c = 4186 \; \text{J/kg·K} \)
- \( h = 0.01 \; \text{m} \)
- \( E_0 = 10 \; \text{V/m} \)
- \( d = 0.2 \; \text{m} \)
- \( t = 1 \; \text{s} \)

\[
\Delta T = \frac{(5.5 \times 10^{-6}) (10)^2 (1)}{4 (1 \times 10^3) (0.01) (4186) (0.2)^2} \approx 8.21 \times 10^{-5} \; \text{K}.
\]

---

Feel free to contribute or suggest improvements!
