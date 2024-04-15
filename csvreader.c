#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "auxiliar.h"
#include "funcFiltros.h"
#include "io.h"

int main(int argc, char **argv) {
    char *entrada;
    int n;
    arq_csv *dados;

    if (argc != 2) {
        printf("\nUse:  %s <arquivo.csv>\n\n", argv[0]);
        return 1;
    }

    entrada = strdup(argv[1]);

    if (strstr(entrada, ".csv") == NULL) {
        printf("\nO arquivo não é .csv\n");
        printf("Use:  %s <arquivo.csv>\n\n", argv[0]);
        free(entrada);
        return 1;
    }


    dados = abre_arquivo(entrada);
    if (dados == NULL) {
        free(entrada);
        printf("Arquivo não encontrado!!\n\n");
        return 1;
    }

    do {
        printf("\n");
        printf("1) Sumario do Arqivo\n");
        printf("2) Mostrar\n");
        printf("3) Filtros\n");
        printf("4) Descrição dos Dados\n");
        printf("5) Ordenacao\n");
        printf("6) Selecao\n");
        printf("7) Dados Faltantes\n");
        printf("8) Salvar Dados\n");
        printf("9) Fim\n");
        printf("Digite sua opcao: ");
        scanf("%d", &n);
        getchar();
        printf("\n");

        if (n == 1) {
            // Aqui vai sumario
            sumario(dados);
        } else if (n == 2) {
            // Aqui vai mostrar
            exibe(dados);
        } else if (n == 3) {
            // Aqui vai filtro
            filtrar(dados);
        } else if (n == 4) {
            // Aqui vai descricao dos dados
            descricao(dados);
        } else if (n == 5) {
            // Aqui vai ordenacao
            ordenacao(dados);
        } else if (n == 6) {
            // Aqui vai selecao
            selecao(dados);
        } else if (n == 7) {
            // Aqui vai dados faltantes
            dados_faltantes(dados);
        } else if (n == 8) {
            // Aqui vai salvar dados
            guardar_dados(dados, dados->tamanho_colunas);
        } else if (n == 9) {
            // Aqui é fim
            free(entrada);
            fecha_arquivo(dados);
        } else {
            printf("--> [Opção inválida, entre com opção válida!!] <--\n");
        }
    } while (n != 9);

    return 0;
}