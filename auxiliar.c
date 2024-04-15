#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "auxiliar.h"
#include "io.h"

char *separa(char *string){
	char *pos_virgula;
	if (!string) return 0;
	pos_virgula = strchr(string, ',');
	if (!pos_virgula) return 0;
	*pos_virgula = '\0';
	return pos_virgula + 1;
}

unsigned short contar_digitos(unsigned long numero) {
    unsigned short digitos = 0;
    // Caso especial para número zero
    if (numero == 0) return 1;
    while (numero != 0) {
        numero /= 10;
        digitos++;
    }
    return digitos;
}

int test_tamanho(unsigned long a, unsigned long b, char *aux, unsigned long *indice) {
    if (a == b) {
        printf("\nNao possui colunas selecionadas.\n");
        printf("Selecione as opcoes novamente.\n");
        free(aux);
        free(indice);
        return 1;
    }
    return 0;
}

void reset_CL(char *vetor, unsigned long tam) {
    unsigned long i;
    for (i = 0; i < tam; i++) {
        vetor[i] = 1;
    }
    return;
}

void reset_P(unsigned long *vetor, unsigned long tam) {
    unsigned long i;
    for (i = 0; i < tam; i++) {
        vetor[i] = i;
    }
    return;
}

void copiar_a(unsigned long *indices, unsigned long *aux, unsigned long max) {
    for (unsigned long i = 0; i < max; i++) {
        indices[i] = aux[i];
    }
    return;
}

void copiar_d(unsigned long *indices, unsigned long *aux, unsigned long max) {
    unsigned long tmp, ultimo;
    ultimo = max - 1;
    for (unsigned long i = 0; i < (max / 2); i++) {
        // Faz a troca dos elementos
        tmp = indices[i];
        indices[i] = aux[ultimo];
        aux[ultimo] = tmp;
        ultimo--;
    }
    return;
}

void reset_linhas(unsigned long *indices, unsigned long *aux, unsigned long tamL) {
    for (unsigned long i = 0; i < tamL; i++) {
        indices[i] = aux[i];
    }
    return;
}

void imprime_espaco(unsigned short tam) {
    for (unsigned short i = 0; i < tam; i++) {
        printf(" ");
    }
    return;
}

unsigned long conta_coluna(FILE *arquivo) {
    unsigned long c = 0;
    char string[MAX], *token, *line, *ptr;
    if (fgets(string, MAX, arquivo) == NULL) return 0;

    line = strdup(string);
    ptr = line; // aqui guarda o ponteiro para não dar free em NULL
    token = strsep(&line, ",");
    while (token != NULL) {
        c++;
        token = strsep(&line, ",");
    }
    free(ptr);
    rewind(arquivo);
    return c;
}

void insere_matriz(arq_csv *csv, unsigned long tamL, unsigned long tamColuna) {
    char buffer[MAX], *atual, *proximo;
    unsigned long i, j, contador, tamanho;

    matriz *m = malloc(tamColuna * sizeof(matriz));
    if (!m) {
        perror("Erro ao alocar memória.\n");
        return;
    }
    memoria *aux = malloc(sizeof(memoria));
    if (!aux) {
        free(m);
        perror("Erro ao alocar memória.\n");
        return;
    }
    aux->linhas = malloc(tamL * sizeof(unsigned long));
    if (!aux->linhas) {
        free(m);
        free(aux);
        perror("Erro ao alocar memória.\n");
        return;
    }
    aux->col_valida = malloc(tamColuna + 1);
    if (!aux->col_valida) {
        free(m);
        free(aux);
        free(aux->linhas);
        perror("Erro ao alocar memória.\n");
        return;
    }
    aux->lin_valida = malloc(tamL - 1);
    if (!aux->lin_valida) {
        free(m);
        free(aux);
        free(aux->linhas);
        free(aux->col_valida);
        perror("Erro ao alocar memória.\n");
        return;
    }
    aux->prioridades = malloc(tamColuna * sizeof(unsigned long));
    if (!aux->prioridades) {
        free(m);
        free(aux);
        free(aux->linhas);
        free(aux->col_valida);
        free(aux->lin_valida);
        perror("Erro ao alocar memória.\n");
        return;
    }

    fgets(buffer, MAX, csv->arquivo);
    tamanho = strlen(buffer);
    buffer[tamanho - 1] = '\0'; // Tira '\n'
    atual = buffer;

    aux->col_valida[0] = 1; // Coluna dos indices
    for (i = 0; i < tamColuna; i++) {
        m[i].dados = malloc(sizeof(coluna));
        if (!m[i].dados) {
            perror("Erro ao alocar memória.\n");
            return;
        }
        m[i].dados->word = malloc((tamL - 1) * sizeof(char*));
        if (!m[i].dados->word) {
            perror("Erro ao alocar memória.\n");
            free(m[i].dados);
            return;
        }
        aux->prioridades[i] = i; // Ordem das colunas
        aux->col_valida[i + 1] = 1; // Colunas validas
        proximo = separa(atual);
        if (strcmp(atual, "") != 0) {
            m[i].rotulo = strdup(atual);
        } else {
            m[i].rotulo = strdup("NaN");
        }
        atual = proximo;
    }

    contador = 0;
    while (fgets(buffer, MAX, csv->arquivo)) {
        if (strcmp(buffer, "\n") == 0) continue;
        tamanho = strlen(buffer);
        if (buffer[tamanho - 1] == '\n') buffer[tamanho - 1] = '\0';
        atual = buffer;
        for (j = 0; j < tamColuna; j++) {
            proximo = separa(atual);
            if (strcmp(atual, "") == 0) {
                m[j].dados->word[contador] = strdup("NaN");
            } else {
                m[j].dados->word[contador] = strdup(atual);
            }
            atual = proximo;
        }
        aux->linhas[contador] = contador; // Indice das linhas
        aux->lin_valida[contador] = 1; // Linhas validas recebe 1
        contador++;
    }

    aux->linhas_memoria = csv->tamanho_linhas - 1;
    csv->infos = m;
    csv->vetor_auxiliar = aux;
    rewind(csv->arquivo);
    return;
}

void loop_tamanho(arq_csv *csv, unsigned long linha, unsigned short *tam) {
    char *word;
    unsigned long j;
    for (j = 0; j < csv->tamanho_colunas; j++) {
        word = csv->infos[j].dados->word[linha];
        if (tam[j + 1] < strlen(word) + TAM_PAL) {
            tam[j + 1] = strlen(word) + TAM_PAL; // Constante para adicionar espaço extra
        }
    }
    return;
}

unsigned short *qtd_espaco(arq_csv *csv, unsigned long tamL, unsigned long tamanho) {
    char *word;
    unsigned long contador, i;
    unsigned short *tam_palavra, digitos;

    tam_palavra = malloc((tamanho + 1) * sizeof(unsigned short));
    if (tam_palavra == NULL) {
        printf("Erro de alocação de memória.\n");
        return NULL;
    }
    for (contador = 0; contador < tamanho + 1; contador++) {
        tam_palavra[contador] = TAM_PAL;
    }

    i = 0;
    digitos = contar_digitos(csv->tamanho_linhas - 1);
    if (digitos > 4) tam_palavra[i] = digitos;

    for (i = 1; i < tamanho + 1; i++) {
        word = csv->infos[i - 1].rotulo;
        if (tam_palavra[i] < strlen(word) + TAM_PAL) {
            tam_palavra[i] = strlen(word) + TAM_PAL;
        }
    }

    if ((tamL) <= 10) {
        for (i = 0; i < tamL; i++) {
            loop_tamanho(csv, csv->vetor_auxiliar->ptr[i], tam_palavra);
        }
    } else {
        for (i = 0; i < 10; i++) {
            loop_tamanho(csv, csv->vetor_auxiliar->ptr[i], tam_palavra);
        }
    }
    return tam_palavra;
}

long double calcula_desvio(arq_csv *csv, unsigned long coluna, long double media, unsigned long quantidade) {
    unsigned long i, numero;
    long double desvio = 0;
    if (quantidade == 0) return desvio;
    for (i = 0; i < csv->tamanho_linhas - 1; i++) {
        if (csv->vetor_auxiliar->lin_valida[i] == 1) { // Faz verificacao da linha
            if (strcmp(csv->infos[coluna].dados->word[i], "NaN") == 0) continue;
            numero = atof(csv->infos[coluna].dados->word[i]);
            desvio += ((numero - media) * (numero - media));
        }
    }
    desvio = sqrt(desvio / quantidade);
    return desvio;
}

long double calcula_media(arq_csv *csv, unsigned long coluna) {
    unsigned long i;
    long double media, contador;
    media = 0;
    contador = 0;
    for (i = 0; i < csv->tamanho_linhas - 1; i++) {
        if(csv->vetor_auxiliar->lin_valida[i] == 1) { // Faz verificacao da linha
            if (strcmp(csv->infos[coluna].dados->word[i], "NaN") != 0) {
                media += atof(csv->infos[coluna].dados->word[i]);
                contador++;
            }
        }
    }
    if (contador == 0) return 0;
    return media / contador;
}

unsigned long devolve_proximo(arq_csv *csv, unsigned long coluna, unsigned long posicao) {
    for (; posicao < csv->tamanho_linhas - 1; posicao++) {
        if (csv->vetor_auxiliar->lin_valida[csv->vetor_auxiliar->linhas[posicao]] == 1) {
            if (strcmp(csv->infos[coluna].dados->word[csv->vetor_auxiliar->linhas[posicao]], "NaN") != 0) {
                return posicao;
            }
        }
    }
    return posicao + 1;
}

void insere_ptr(arq_csv *csv, unsigned long linhas_validas) {
    unsigned long i, j, aux, tamanho;
    aux = 0;
    if (linhas_validas == 0) {
        printf("Nao possui dados!!\n");
        return;
    }

    if (linhas_validas <= 10) {
        for (i = 0; i < csv->tamanho_linhas - 1; i++) {
            if (csv->vetor_auxiliar->lin_valida[csv->vetor_auxiliar->linhas[i]] == 1) {
                csv->vetor_auxiliar->ptr[aux] = csv->vetor_auxiliar->linhas[i];
                aux++;
            }
        }
    } else {
        tamanho = 10;
        for (i = 0; tamanho > 5 && i < csv->tamanho_linhas - 1; i++) {
            if (csv->vetor_auxiliar->lin_valida[csv->vetor_auxiliar->linhas[i]] == 1) {
                csv->vetor_auxiliar->ptr[aux] = csv->vetor_auxiliar->linhas[i];
                aux++;
                tamanho--;
            }
        }
        aux = 9;
        for (j = csv->tamanho_linhas - 2; tamanho > 0; j--) {
            if (csv->vetor_auxiliar->lin_valida[csv->vetor_auxiliar->linhas[j]] == 1) {
                csv->vetor_auxiliar->ptr[aux] = csv->vetor_auxiliar->linhas[j];
                aux--;
                tamanho--;
            }
        }

    }
}
