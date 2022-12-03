#!/bin/sh

gcc -g -flto -O3 -Wall main.c prr.c calcul_residu.c projection.c resolution_sev.c retour_espace_depart.c tools.c -lblas -llapack -lm 

