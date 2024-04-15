#ifndef AUXILIAR_H
#define AUXILIAR_H

#include "io.h"
#define MAX 1025
#define TAM_PAL 2

// Separa a linha em palavras
char *separa(char *string);

// Conta quantidade de digitos
unsigned short contar_digitos(unsigned long numero);

// Verifica se a coluna foi ou não selecionado
int test_tamanho(unsigned long a, unsigned long b, char *aux, unsigned long *indice);

// Volta normal as colunas e linhas validas
void reset_CL(char *vetor, unsigned long tam);

// Volta normal a prioridade das colunas
void reset_P(unsigned long *vetor, unsigned long tam);

// Copia de forma crescente
void copiar_a(unsigned long *indices, unsigned long *aux, unsigned long max);

// Copia de forma decrescente
void copiar_d(unsigned long *indices, unsigned long *aux, unsigned long max);

// Volta as linhas do jeito que estava o arquivo
void reset_linhas(unsigned long *indices, unsigned long *aux, unsigned long tamL);

// Imprime quantidade certa de espacos
void imprime_espaco(unsigned short tam);

// Conta quantidade de colunas
unsigned long conta_coluna(FILE *arquivo);

// Insere os elementos na matriz
void insere_matriz(arq_csv *csv, unsigned long tamL, unsigned long tamColuna);

// Verifica o tamanho das palavras
void loop_tamanho(arq_csv *csv, unsigned long linha, unsigned short *tam);

// Verifica o tamanho das palavras
unsigned short *qtd_espaco(arq_csv *csv, unsigned long tamL, unsigned long tamanho);

// Calcula media
long double calcula_media(arq_csv *csv, unsigned long coluna);

// Calcula desvio
long double calcula_desvio(arq_csv *csv, unsigned long coluna, long double media, unsigned long quantidade);

// Devolve a posicao do proximo valor valido
unsigned long devolve_proximo(arq_csv *csv, unsigned long coluna, unsigned long posicao);

// Coloca as linhas para um vetor
void insere_ptr(arq_csv *csv, unsigned long linhas_validas);

#endif