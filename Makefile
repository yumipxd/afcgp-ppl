DIR_nonbonded=src/C-Ener-nonbonded
DIR_readdcd=src/R-Write-Dcd

CC = g++
CF = gfortran
FFLAGS = -c
SYSLIB = -fopenmp
EXE_CNE = bin/Calc-nonbonded-energy
EXE_RDW = bin/R-write-dcd

OBJ_readdcd = \
	$(DIR_readdcd)/R-write-dcd.o \
	$(DIR_readdcd)/Load_dcd_head.o \
	$(DIR_readdcd)/Load_dcd_coor.o 

OBJ_nonbonded = \
	$(DIR_nonbonded)/Calc-Ener-nonbonded.o 


.c.o:
	$(CC) -o $(EXE_CNE) $(SYSLIB) $*.cpp
.f.o:
	$(CF)  $(FFLAGS) $*.f90
