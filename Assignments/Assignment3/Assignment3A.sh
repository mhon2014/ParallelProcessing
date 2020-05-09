#!/bin/bash
#
## SLURM submission script for OpenMP job Assignment3
## change cpus-per-task to change number of threads
#
#SBATCH --job-name=Assignment3A_omp
#SBATCH --output=Aomp.%J.out
#SBATCH --error=Aomp.%J.err
#
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=16
#SBATCH --time=5:00
#SBATCH --partition=class

export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK
./Assignment3A 1600