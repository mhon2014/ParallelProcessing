#!/bin/bash
#
## SLURM submission script for OpenMP job Assignment3
## change cpus-per-task to change number of threads
#
#SBATCH --job-name=Assignment3B_omp
#SBATCH --output=Assignment3Bomp.%J.out
#SBATCH --error=Assignment3Bomp.%J.err
#
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --time=5:00
#SBATCH --partition=class

export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK
./Assignment3B