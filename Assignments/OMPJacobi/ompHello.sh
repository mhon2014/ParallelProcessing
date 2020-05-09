#!/bin/bash
#
## SLURM submission script for OpenMP job helloWorld
## change cpus-per-task to change number of threads
#
#SBATCH --job-name=hello_omp
#SBATCH --output=HELLOomp.%J.out
#SBATCH --error=HELLOomp.%J.err
#
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --time=5:00
#SBATCH --partition=class

export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK
./helloWorld
