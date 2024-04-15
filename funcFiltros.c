#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcFiltros.h"

// se a < b retorna 1
int compara_menor(char *a, char *b) {
    if (strcmp(a, "NaN") == 0 && strcmp(b, "NaN") == 0) return 0;
    if (strcmp(a, "NaN") == 0) return 1;
    if (strcmp(b, "NaN") == 0) return 0;
    if ((atoi(a) == 0 && atoi(b) == 0)) {
        return (strcmp(a, b) < 0);
    } else {
        double n, m;
        n = atof(a);
        m = atof(b);
        return (n < m);
    }
}

// se a <= b
int compara_menor_igual(char *a, char *b) {
    if (strcmp(a, "NaN") == 0 && strcmp(b, "NaN") == 0) return 1;
    if (strcmp(a, "NaN") == 0) return 1;
    if (strcmp(b, "NaN") == 0) return 0;
    if ((atoi(a) == 0 && atoi(b) == 0)) {
        return (strcmp(a, b) <= 0);
    } else {
        double n, m;
        n = atof(a);
        m = atof(b);
        return (n <= m);
    }
}

// se a > b retorna 1
int compara_maior(char *a, char *b) {
    if (strcmp(a, "NaN") == 0 && strcmp(b, "NaN") == 0) return 0;
    if (strcmp(a, "NaN") == 0) return 0;
    if (strcmp(b, "NaN") == 0) return 1;
    if ((atoi(a) == 0 && atoi(b) == 0)) {
        return (strcmp(a, b) > 0);
    } else {
        double n, m;
        n = atof(a);
        m = atof(b);
        return (n > m);
    }
}

// se a >= b 
int compara_maior_igual(char *a, char *b) {
    if (strcmp(a, "NaN") == 0 && strcmp(b, "NaN") == 0) return 1;
    if (strcmp(a, "NaN") == 0) return 0;
    if (strcmp(b, "NaN") == 0) return 1;
    if ((atoi(a) == 0 && atoi(b) == 0)) {
        return (strcmp(a, b) >= 0);
    } else {
        double n, m;
        n = atof(a);
        m = atof(b);
        return (n >= m);
    }
}

// se a == b
int igual(char *a, char *b) {
    if (strcmp(a, "NaN") == 0 && strcmp(b, "NaN") == 0) return 1;
    if ((atoi(a) == 0 && atoi(b) == 0)) {
        return (strcmp(a, b) == 0);
    } else {
        double n, m;
        n = atof(a);
        m = atof(b);
        return (n == m);
    }
}

// se a != b
int diferente(char *a, char *b) {
    if (strcmp(a, "NaN") == 0 && strcmp(b, "NaN") == 0) return 0;
    if ((atoi(a) == 0 && atoi(b) == 0)) {
        return (strcmp(a, b) != 0);
    } else {
        double n, m;
        n = atof(a);
        m = atof(b);
        return (n != m);
    }
}
