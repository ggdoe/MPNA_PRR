#!/bin/sh

gcc -g -Wall main.c calcul_residu.c projection.c resolution_sev.c retour_espace_depart.c tools.c -lblas -llapack -lm 

