#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "auxiliar.h"
#include "funcFiltros.h"
#include "io.h"

void merge(arq_csv *csv, unsigned long *indices, unsigned long m, unsigned long max, unsigned long col) {
    unsigned long i, j, k, *aux;
    aux = malloc(max * sizeof(unsigned long));
    if (!aux) return;

    i = 0;
    j = m;
    k = 0;
    while (k < max) {
        if (j >= max || (i < m && compara_menor(csv->infos[col].dados->word[indices[i]], csv->infos[col].dados->word[indices[j]]))) {
            aux[k] = indices[i];
            i++;
        } else {
            aux[k] = indices[j];
            j++;
        }
        k++;
    }
    copiar_a(indices, aux, max);
    free(aux);
}

void mergeSort(arq_csv *csv, unsigned long *indices, unsigned long max, unsigned long col) {
    unsigned long m;
    if (max <= 1) return;
    m = max / 2;  
    mergeSort(csv, indices, m, col);
    mergeSort(csv, indices + m, max - m, col);
    merge(csv, indices, m, max, col);
}
