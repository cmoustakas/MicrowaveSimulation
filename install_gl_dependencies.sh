#!/bin/bash

#Exit immediately in silent errors
set -euo pipefail

# Update and upgrade the system
sudo apt update && sudo apt upgrade -y

# Install essential build tools
sudo apt install -y build-essential cmake git

# Install OpenGL libraries
sudo apt install -y libgl1-mesa-dev libglu1-mesa-dev freeglut3-dev

# Install GLFW
sudo apt install -y libglfw3 libglfw3-dev

# Install Assimp
sudo apt install -y libassimp-dev

# Install GLM (math library for OpenGL)
sudo apt install -y libglm-dev

# Install GLEW (OpenGL extension wrangler)
sudo apt install -y libglew-dev

# Install additional tools for debugging and profiling OpenGL apps
sudo apt install -y gdb valgrind

# Clean up
sudo apt autoremove -y

echo ""
echo "[+] All prerequisites for OpenGL development are installed."
echo ""
echo "[+]This project also uses CUDA for GPGPU calculations make sure the appropriate toolkit is installed"