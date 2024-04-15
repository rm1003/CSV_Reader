#ifndef ORDENA_H
#define ORDENA_H

#include "io.h"
#include "auxiliar.h"
#include "funcFiltros.h"

void merge(arq_csv *csv, unsigned long *indices, unsigned long m, unsigned long max, unsigned long col);

void mergeSort(arq_csv *csv, unsigned long *indices, unsigned long max, unsigned long col);

#endif