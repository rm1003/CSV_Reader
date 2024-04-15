// Arquivo io.h
#ifndef __IO__
#define __IO__

#include <stdio.h>
#define MAX 1025
#define TAM_PAL 2

typedef int(*filtros)(char*, char*);

typedef struct memoria {
    char *col_valida; // tamC + 1 , tem a coluna de indices
    char *lin_valida; // tamL - 1
    unsigned long *prioridades; // tamC
    unsigned long *linhas; // tamL - 1
    unsigned long ptr[10];
    unsigned long linhas_memoria;
} memoria;

// Matriz
typedef struct coluna {
    char **word;
} coluna;

typedef struct matriz {
    char *rotulo;
    struct coluna *dados;
} matriz;

// Struct principal
typedef struct arq_csv {
    FILE *arquivo; // Arquivo
    char *file; // Nome do arquivo
    struct matriz *infos; // Matriz com as informacoes
    struct memoria *vetor_auxiliar; // Struct com as informacoes para manipulacao
    char *tipos; // Tipo numeric ou string
    unsigned long tamanho_colunas; // Quantidade de colunas
    unsigned long tamanho_linhas; // Quantidade de linhas
} arq_csv;

// Abre o arquivo e chama verifica para obter todos os dados do arquivo
arq_csv *abre_arquivo(char *entrada);

// verifica as linhas
void verifica(arq_csv *csv);

// Exibe informacoes mostrando para usuario quais escolhass
int exibe_solicitacoes(arq_csv *csv, unsigned long tam_linha, unsigned long tam_coluna);
 

// Sumario
void sumario(arq_csv *csv);


// Mostrar
void exibe(arq_csv *csv);
void mostrar(arq_csv *csv, unsigned long tamL);
// Imprime as linhas
void imprime_linha(arq_csv *csv, unsigned short *tamanho, unsigned long indice);


// Filtro
void filtrar(arq_csv *csv);
// Funcao que retorna o filtro correto
filtros retornaFiltro(char *filt);


// Descricao
void descricao(arq_csv *csv);
// Coluna numeric
void numerico(arq_csv *csv, unsigned long coluna);
// Coluna string
void strings(arq_csv *csv, unsigned long coluna);
void imprime_valor_unico(arq_csv *csv, unsigned long coluna, char tipo, unsigned long qtd);

// Ordenacao
void ordenacao(arq_csv *csv);


// Selecao
void selecao(arq_csv *csv);


// Dados faltantes
void dados_faltantes(arq_csv *csv);
// Listar as linhas nan
void listar_nan(arq_csv *csv);
// Substitui pela media
void substitui_media(arq_csv *csv);
// Substitui pelo proximo
void substitui_proximo(arq_csv *csv);
// Remove linhas com nan
void remove_nan(arq_csv *csv);


// Guarda dados
int insere_dados(arq_csv *csv, char *file_name, unsigned long size);
void guardar_dados(arq_csv *csv, unsigned long tamanho_coluna);


// libera memoria
void libera_matriz(arq_csv *csv);
void fecha_arquivo(arq_csv *csv);

#endif