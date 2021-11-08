#!/bin/bash


#SBATCH --job-name=TestNested # Job name
#SBATCH --partition=Orion
#SBATCH --nodes=1                    # Run all processes on a single node	
#SBATCH --ntasks=1                   # Run a single task	
#SBATCH --constraint=caslake    # for Skylake:skylake, for cascade lake: caslake
#SBATCH --cpus-per-task=48            # Number of CPU cores per task
#SBATCH --mem=300gb                  # Total memory limit
#SBATCH --time=60:00:00              # Time limit hrs:min:sec
#SBATCH --output=nested_loop_%j.log     # Standard output and error log
#date;hostname;pwd

export OMP_NUM_THREADS=48

module load intel
#export OMP_NESTED=true

for ot in 2
do
	for inn in 1 12 24
	do
		./TestNested.o $ot $inn
	done
done
