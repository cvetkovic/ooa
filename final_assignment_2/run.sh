#!/usr/bin/env bash

g++ main.cpp -o final
$(./final)

python3 visualize.py

#rm ./final