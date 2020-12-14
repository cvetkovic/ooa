#!/usr/bin/env bash

g++ -std=c++17 main.cpp -o 2bp_solver -DLOGARITHMIC_COOLING_SCHEDULE
./2bp_solver
python3 visualize.py