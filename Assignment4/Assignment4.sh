#!/bin/bash
#
## SLURM submission script for OpenMP job Assignment3
## change cpus-per-task to change number of threads
#
#SBATCH --job-name=Assignment4_omp
#SBATCH --output=4omp.%J.out
#SBATCH --error=4omp.%J.err
#
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --time=5:00
#SBATCH --partition=class

export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK
./Assignment4 2000
