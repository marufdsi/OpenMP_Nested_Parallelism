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

X=100000
Y=10
for L in 2 3 4 6 8 12 24 48 10000 100000 10 20 100 1000
do
    for ot in 2 6 12 24 48
    do
	for inn in 1
	do
            ./NestedOMP $L $X $Y $ot $inn
	done
     done
     for ot in 2 6 12 24
     do
	 for inn in 2
	 do
	     ./NestedOMP $L $X $Y $ot $inn
	 done
     done
     for ot in 2 6 12
     do
	 for inn in 3 4
	 do
	     ./NestedOMP $L $X $Y $ot $inn
	 done
     done
     for ot in 2 6
     do
         for inn in 6 8
         do
             ./NestedOMP $L $X $Y $ot $inn
         done
     done
     for ot in 2
     do
         for inn in 12 24
         do
             ./NestedOMP $L $X $Y $ot $inn
         done
     done
done

