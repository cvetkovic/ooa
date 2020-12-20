#!/bin/bash

# Compile program
g++ shapes.hpp shapes.cpp main.cpp -o fa_layout_miner

# Run
./fa_layout_miner

# Visualize
python3 visualize.py