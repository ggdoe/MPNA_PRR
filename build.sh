#!/bin/sh

mpicc -g -flto -Wall main.c prr.c multi_prr.c calcul_residu.c projection.c resolution_sev.c retour_espace_depart.c tools.c -lblas -llapack -lm 

