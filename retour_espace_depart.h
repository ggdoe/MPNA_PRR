#ifndef RETOUR_ESPACE_DEPART_H
#define RETOUR_ESPACE_DEPART_H

#include "tools.h"

// Vm : taille m*m
// u : taille 1*m
// q : taille m*m (tableau retourné)
void* ret_esp_dep(int m, const double* Vm, const double* u);

#endif
