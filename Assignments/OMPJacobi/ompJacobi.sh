#!/bin/bash
#
## SLURM submission script for OpenMP job jacobi
## change cpus-per-task to change number of threads
#SBATCH --job-name=jacobi_omp
#SBATCH --output=JACOBIomp.%J.out
#SBATCH --error=JACOBIomp.%J.err
#
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=24
#SBATCH --time=5:00
#SBATCH --partition=class

export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK
## change n to modify number gridpoints in the rod
## change s to modify number of outer jacobi sweeps or iterations
./jacobi -n 1600000 -s 100
