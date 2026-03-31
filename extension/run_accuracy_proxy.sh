#!/bin/sh
set -e

cd /home/data/mlk/code/index-new2

# Build
/usr/bin/g++ -O3 -std=c++11 -pthread -o main main.cpp Graph.cpp BitMap.cpp HashSet.cpp

# Reviewer-requested settings
K_LIST="2 4 10 20 50 100 200 500 1000"

for K in $K_LIST; do
  ./main /home/data/mlk/data/wiki/wiki "$K" wiki 0.5 0.5
  ./main /home/data/mlk/data/web-Google/web-Google "$K" web-Google 0.5 0.5
done
