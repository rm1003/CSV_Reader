#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "ordena.h"
#include "auxiliar.h"
#include "funcFiltros.h"
#include "io.h"

// Funções auxiliares
// =============================================================================
// O retorno 1 ou 0, indica se o usuario descartou ou nao as informacoes
int exibe_solicitacoes(arq_csv *csv, unsigned long tam_linha, unsigned long tam_coluna) {
    char string[MAX], file_name[MAX], escolha[MAX], ok;
    fgets(string, sizeof(string), stdin);
    string[strlen(string) - 1] = '\0';
    if (strcmp(string, "S") == 0 || strcmp(string, "s") == 0) {
        printf("Entre com o nome do arquivo: ");
        fgets(file_name, sizeof(file_name), stdin);
        file_name[strlen(file_name) - 1] = '\0';

        ok = insere_dados(csv, file_name, tam_coluna);
        if (ok == 1) printf("Arquivo gravado com sucesso.\n\n");
        else printf("\nFalha ao colocar dados no arquivo.\n\n");
    } else if (strcmp(string, "N") != 0 && strcmp(string, "n") != 0){
        printf("\nEntrada Invalida.\n");
        return 1;
    }
    // Descarte
    printf("Deseja descartar os dados originais? [S|N]: ");
    scanf("%s", escolha);
    getchar();
    // Aqui se o usuario deseja descartar
    if (strcmp(escolha, "S") == 0 || strcmp(escolha, "s") == 0) {
        csv->vetor_auxiliar->linhas_memoria = tam_linha;
        printf("Pressione ENTER para continuar\n");
        getchar();
        return 0;
    }
    printf("Pressione ENTER para continuar\n");
    getchar();
    return 1;
}
// =============================================================================

// Verifica arquivo
// =============================================================================
void verifica(arq_csv *csv) {
    unsigned long i, j, contador;
    char buffer[MAX], *atual, *proximo;

    // Alocação de memória
    csv->tipos = malloc(csv->tamanho_colunas * sizeof(char));
    if (csv->tipos == NULL) {
        perror("Erro em alocação de memória.\n");
        return;
    }
    // Inicia os valores em 1
    for (i = 0; i < csv->tamanho_colunas; i++) {
        csv->tipos[i] = 1;
    }
 
    // todo numero pode ser string, mas nem toda string é um numero
    // 0 para Numeric e 1 para String
    contador = 0;
    while (fgets(buffer, MAX, csv->arquivo)) {
        if (strcmp(buffer, "\n") == 0) continue;
        atual = buffer; // Aqui guarda o ponteiro para não dar free em NULL
        for (j = 0; j < csv->tamanho_colunas; j++){
            proximo = separa(atual);

            // Verifica o tipo
            if (atoi(atual) != 0 || atof(atual) != 0) {
                csv->tipos[j] = 0;
            }
            atual = proximo;
        }
        contador++;
    }

    csv->tamanho_linhas = contador;
    rewind(csv->arquivo);
    return;
}

arq_csv *abre_arquivo(char *entrada) {

    arq_csv *csv = malloc(sizeof(arq_csv));
    if (!csv) {
        perror("Erro ao alocar memória.\n");
        return NULL;
    }

    // Abre arquivo
    csv->arquivo = fopen(entrada, "r+");
    if (csv->arquivo == NULL) {
        free(csv);
        perror("\nO arquivo não foi aberto corretamente\n");
        return NULL;
    }
    csv->file = entrada;
    // Conta linha e coluna
    csv->tamanho_colunas = conta_coluna(csv->arquivo);

    // Verifica tipo
    verifica(csv);

    if (csv->tamanho_linhas == 0) {
        printf("\nO arquivo passado não tem nada!!\n");
        free(csv->tipos);
        fclose(csv->arquivo);
        free(csv);
        free(entrada);
        exit(0);
    }

    // Insere os dados na matriz
    insere_matriz(csv, csv->tamanho_linhas, csv->tamanho_colunas);

    rewind(csv->arquivo);
    return csv;
}
// =============================================================================

// Função 1 Sumário
// =============================================================================
void sumario(arq_csv *csv) {
    unsigned long i;

    for (i = 0; i < csv->tamanho_colunas; i++) {
        printf("%s", csv->infos[i].rotulo);
        if (csv->tipos[i] == 0) {
            printf(" [N]\n");
        } else {
            printf(" [S]\n");
        }
    }

    printf("\n%ld variaveis encontradas\n", csv->tamanho_colunas);
    printf("Pressione ENTER para continuar\n\n");
    getchar();
    return;
}
// =============================================================================

// Função 2 Mostrar
// =============================================================================
void imprime_linha(arq_csv *csv, unsigned short *tamanho, unsigned long indice) {
    char *word;
    unsigned short tam_numero;
    unsigned long j;

    tam_numero = contar_digitos(indice); // Tamanho do indice
    for (j = 0; j < csv->tamanho_colunas + 1; j++) {
        if (csv->vetor_auxiliar->col_valida[j] == 1) {
            if (j == 0) {
                // Imprime o indice
                printf("%ld", indice);
                imprime_espaco(tamanho[j] - tam_numero);
            } else {
                // Imprime as informacoes
                word = csv->infos[csv->vetor_auxiliar->prioridades[j - 1]].dados->word[indice];
                imprime_espaco(tamanho[csv->vetor_auxiliar->prioridades[j - 1] + 1] - strlen(word));
                printf("%s", word);
            }
        }
        if ((j + 1) == (csv->tamanho_colunas + 1)) printf("\n");
    }
    return;
}

void mostrar(arq_csv *csv, unsigned long tamL) {
    char *word;
    unsigned short *tamanho;
    unsigned long i, j, indice;

    if (csv->tamanho_linhas == 1) {
        printf("\nO arquivo nao possui dados!!\n");
        return;
    }

    tamanho = qtd_espaco(csv, tamL, csv->tamanho_colunas);
    // Imprime os rotulos
    for (i = 0; i < csv->tamanho_colunas + 1; i++) {
        if (csv->vetor_auxiliar->col_valida[i] == 1) {
            if (i == 0) {
                imprime_espaco(tamanho[i]);
            } else {
                word = csv->infos[csv->vetor_auxiliar->prioridades[i - 1]].rotulo;
                imprime_espaco(tamanho[csv->vetor_auxiliar->prioridades[i - 1] + 1] - strlen(word));
                printf("%s", word);
            }
        }
        if ((i + 1) == (csv->tamanho_colunas + 1)) printf("\n");
    }

    if ((tamL) <= 10) {
        for (j = 0; j < tamL; j++)
            imprime_linha(csv, tamanho, csv->vetor_auxiliar->ptr[j]);
    } else {
        for (j = 0; j < 5; j++) {
            imprime_linha(csv, tamanho, csv->vetor_auxiliar->ptr[j]);
        }
        indice = j;

        for (j = 0; j < csv->tamanho_colunas + 1; j++) {
            if (csv->vetor_auxiliar->col_valida[j] == 1) {
                if (j == 0) {
                    printf("...");
                    imprime_espaco(tamanho[j] - 3);
                } else {
                    imprime_espaco((tamanho[csv->vetor_auxiliar->prioridades[j - 1] + 1] - 3));
                    printf("...");
                }
            }
        }
        printf("\n");

        for (j = 0; j < 5; j++) {
            imprime_linha(csv, tamanho, csv->vetor_auxiliar->ptr[indice]);
            indice++;
        }
    }
    free(tamanho);
    return; 
}

void exibe(arq_csv *csv) {
    insere_ptr(csv, csv->vetor_auxiliar->linhas_memoria);
    mostrar(csv, csv->vetor_auxiliar->linhas_memoria);
    printf("\n[%ld rows x %ld columns]\n", csv->vetor_auxiliar->linhas_memoria, csv->tamanho_colunas);
    printf("\nPressione ENTER para continuar\n");
    getchar();
    return;
}
// =============================================================================

// Função 3 Filtro
// =============================================================================
filtros retornaFiltro(char *filt) {
    if (strcmp(filt, "==") == 0) return igual;
    else if (strcmp(filt, "!=") == 0) return diferente;
    else if (strcmp(filt, "<") == 0) return compara_menor;
    else if (strcmp(filt, "<=") == 0) return compara_menor_igual;
    else if (strcmp(filt, ">") == 0) return compara_maior;
    else if (strcmp(filt, ">=") == 0) return compara_maior_igual;
    else return NULL;
}

void filtrar(arq_csv *csv) {
    char word[MAX], filt[MAX], valor[MAX], *aux;
    unsigned long i, j, coluna, contador;
    filtros ptrOperacao;
    
    printf("Entre com a variavel: ");
    fgets(word, sizeof(word), stdin);
    word[strlen(word) - 1] = '\0';
    if (atoi(word) != 0) {
        printf("\nEntrada invalida.\n");
        return;
    }
    for (i = 0; i < csv->tamanho_colunas; i++) {
        if (strcmp(word, csv->infos[i].rotulo) == 0) {
            coluna = i;
            break;
        }
    }
    if (i == csv->tamanho_colunas) {
        printf("\nNenhuma coluna selecionada.\n");
        return;
    }
    
    printf("Escolha um filtro ( == > >= < <= != ): ");
    fgets(filt, sizeof(filt), stdin);
    filt[strlen(filt) - 1] = '\0';
    if (atoi(filt) != 0) {
        printf("\nEntrada invalida.");
        return;
    }

    ptrOperacao = retornaFiltro(filt);
    if (!ptrOperacao) {
        printf("\nFiltro não encontrado.\n");
        return;
    }

    printf("Digite um valor: ");
    fgets(valor, sizeof(valor), stdin);
    valor[strlen(valor) - 1] = '\0';
    printf("\n");

    aux = malloc(csv->tamanho_linhas - 1);
    if (!aux) return;
    for (j = 0; j < csv->tamanho_linhas - 1; j++) {
        // Guarda posicao inicial das linhas validas
        aux[j] = csv->vetor_auxiliar->lin_valida[csv->vetor_auxiliar->linhas[j]];
        // Zera todas linhas validas
        csv->vetor_auxiliar->lin_valida[csv->vetor_auxiliar->linhas[j]] = 0;
    }

    contador = 0;
    for (j = 0; j < csv->tamanho_linhas - 1; j++) {
        if (aux[j] == 1) {
            if (ptrOperacao(csv->infos[coluna].dados->word[csv->vetor_auxiliar->linhas[j]], valor)) {
                // Se for valido, o vetor linha valida recebe 1 para imprimir
                csv->vetor_auxiliar->lin_valida[csv->vetor_auxiliar->linhas[j]] = 1;
                contador++;
            }
        }
    }

    if (contador == 0) {
        printf("Nao possui dados filtrados.\n");
        for (i = 0; i < csv->tamanho_linhas - 1; i++) {
            csv->vetor_auxiliar->lin_valida[i] = aux[i];
        }
        free(aux);
        return;
    }

    insere_ptr(csv, contador);
    mostrar(csv, contador);
    printf("\n[%ld rows x %ld columns]\n\n", contador, csv->tamanho_colunas);
    printf("Deseja gravar um arquivo com as variaveis filtrados? [S|N]: ");
    if (exibe_solicitacoes(csv, contador, csv->tamanho_colunas)) {
        for (i = 0; i < csv->tamanho_linhas - 1; i++) {
            csv->vetor_auxiliar->lin_valida[i] = aux[i];
        }
    }
    free(aux);
    return;
}
// =============================================================================

// Função 4 descricao
// =============================================================================
void imprime_valor_unico(arq_csv *csv, unsigned long coluna, char tipo, unsigned long qtd) {
    char word[MAX], word_memoria[MAX];
    unsigned long j;
    double atual, num_memoria;
    // Os if serve para verificar se a linha é valida ou nao e se nessa linha tem dados validos
    if (tipo == 0) {
        num_memoria = 0;
        printf("Valores unicos: ");
        if (qtd != 0 || qtd == 0) printf("[");
        for (j = 0; j < csv->tamanho_linhas - 1; j++) {
            if (csv->vetor_auxiliar->lin_valida[csv->vetor_auxiliar->linhas[j]] == 1) {
                if (strcmp(csv->infos[coluna].dados->word[csv->vetor_auxiliar->linhas[j]], "NaN") != 0) {
                    atual = atof(csv->infos[coluna].dados->word[csv->vetor_auxiliar->linhas[j]]);
                    if (atual != num_memoria) {
                        num_memoria = atual;
                        printf("%.2f", atual); // Mostra o numero
                        if (qtd > 0) {
                            printf(", ");
                            qtd--;
                        } else if (qtd == 0) {
                            printf("]\n\n");
                            return;
                        }
                    }
                }
            }
        }
    } else {
        strcpy(word_memoria, "");
        printf("Valores unicos: ");
        if (qtd != 0 || qtd == 0) printf("[");
        for (j = 0; j < csv->tamanho_linhas - 1; j++) {
            if (csv->vetor_auxiliar->lin_valida[csv->vetor_auxiliar->linhas[j]] == 1) {
                if (strcmp(csv->infos[coluna].dados->word[csv->vetor_auxiliar->linhas[j]], "NaN") != 0) {
                    strcpy(word, csv->infos[coluna].dados->word[csv->vetor_auxiliar->linhas[j]]);
                    if (strcmp(word, word_memoria) != 0) {
                        strcpy(word_memoria, word);
                        printf("'%s'", word); // Mostra a palavra
                        if (qtd > 0) {
                            printf(", ");
                            qtd--;
                        } else if (qtd == 0) {
                            printf("]\n");
                            return;
                        }
                    }
                }
            }
        }
    }
    return;
}

void numerico(arq_csv *csv, unsigned long coluna) {
    unsigned long j, contador, aux_mediana, qtd_numeros;
    unsigned long num1, num2, nao_validos, freq_memoria, freq_atual;
    double min, max, mediana, moda, num_atual, num_memoria;
    long double media, desvio;
    contador = 0;
    nao_validos = 0;
    media = 0;
    moda = 0;
    freq_atual = 0;
    freq_memoria = 0;
    qtd_numeros = 0;
    max = 0;
    media = 0;
    // A verificacao da moda eh de acordo com o anterior
    for (j = 0; j < csv->tamanho_linhas - 1; j++) {
        if (csv->vetor_auxiliar->lin_valida[csv->vetor_auxiliar->linhas[j]] == 1) {
            if (strcmp(csv->infos[coluna].dados->word[csv->vetor_auxiliar->linhas[j]], "NaN") != 0) {
                if (contador == 0) {
                    min = atof(csv->infos[coluna].dados->word[csv->vetor_auxiliar->linhas[j]]);
                    num_memoria = atof(csv->infos[coluna].dados->word[csv->vetor_auxiliar->linhas[j]]);
                } else {
                    // Aqui acha a moda
                    num_atual = atof(csv->infos[coluna].dados->word[csv->vetor_auxiliar->linhas[j]]);
                    freq_atual++;
                    if (freq_memoria < freq_atual) {
                        freq_memoria = freq_atual;
                        moda = num_memoria;
                    }
                    if ((num_memoria != num_atual) && (contador != 0)) {
                        num_memoria = num_atual;
                        freq_atual = 0;
                        qtd_numeros++;
                    }
                }
                contador++;
                if (atof(csv->infos[coluna].dados->word[csv->vetor_auxiliar->linhas[j]]) > max) {
                    max = atof(csv->infos[coluna].dados->word[csv->vetor_auxiliar->linhas[j]]);
                }
            } else {
                nao_validos++;
            }
        }
    }

    // Verificacao a mais
    freq_atual++;
    if (freq_memoria < freq_atual) {
        freq_memoria = freq_atual;
        moda = num_memoria;
    }

    if (contador == 0) {
        printf("\nNao possui valores validos.\n");
        return;
    }
    // Acha a Mediana 
    aux_mediana = contador / 2;
    if ((contador % 2) == 0) {
        num1 = atof(csv->infos[coluna].dados->word[csv->vetor_auxiliar->linhas[aux_mediana + nao_validos]]);
        num2 = atof(csv->infos[coluna].dados->word[csv->vetor_auxiliar->linhas[aux_mediana - 1 + nao_validos]]);
        mediana = (num1 + num2) / 2;
    } else {
        mediana = atof(csv->infos[coluna].dados->word[csv->vetor_auxiliar->linhas[aux_mediana + nao_validos]]);
    }
    
    media = calcula_media(csv, coluna);
    desvio = calcula_desvio(csv, coluna, media, contador);
    printf("Contador: %ld\n", contador);
    printf("Media: %.2Lf\n", media);
    printf("Desvio: %.2Lf\n", desvio);
    printf("Mediana: %.2f\n", mediana);
    printf("Moda: %.2f %ld vezes\n", moda, freq_memoria);
    printf("Min.: %.2f\n", min);
    printf("Max.: %.2f\n", max);
    imprime_valor_unico(csv, coluna, csv->tipos[coluna], qtd_numeros);
    return;
}

void strings(arq_csv *csv, unsigned long coluna) {
    char word_atual[MAX], moda[MAX], word_memoria[MAX];
    unsigned long j, contador, freq_atual, freq_memoria, qtd_palavras;
    contador = 0;
    freq_atual = 0;
    freq_memoria = 0;
    qtd_palavras = 0;
    strcpy(word_memoria, "NaN");
    strcpy(moda, "NaN");
    // A verificacao da moda eh de acordo com o anterior
    for (j = 0; j < csv->tamanho_linhas - 1; j++) {
        if (csv->vetor_auxiliar->lin_valida[csv->vetor_auxiliar->linhas[j]] == 1) {
            if (strcmp(csv->infos[coluna].dados->word[csv->vetor_auxiliar->linhas[csv->vetor_auxiliar->linhas[j]]], "NaN") != 0) {
                if (contador == 0) {
                    strcpy(word_memoria, csv->infos[coluna].dados->word[csv->vetor_auxiliar->linhas[j]]);
                } else {
                    // Aqui acha a moda
                    strcpy(word_atual, csv->infos[coluna].dados->word[csv->vetor_auxiliar->linhas[j]]);
                    freq_atual++;
                    if (freq_memoria < freq_atual) {
                        freq_memoria = freq_atual;
                        strcpy(moda, word_memoria);
                    }
                    if (strcmp(word_atual, word_memoria) != 0) {
                        strcpy(word_memoria, word_atual);
                        freq_atual = 0;
                        qtd_palavras++;
                    }
                }
                contador++;
            }
        }
    }
    freq_atual++;
    if (freq_memoria < freq_atual) {
        freq_memoria = freq_atual;
        strcpy(moda, word_memoria);
    }
    if (contador == 0) {
        printf("\nNao possui valores validos.\n");
        return;
    }

    printf("Contador: %ld\n", contador);
    printf("Moda: %s %ld vezes\n", moda, freq_memoria);
    // printf("\n\n%ld\n\n", qtd_palavras - 1);
    imprime_valor_unico(csv, coluna, csv->tipos[coluna], qtd_palavras);
}

void descricao(arq_csv *csv) {
    char word[MAX];
    unsigned long i, coluna, *aux;

    if ((csv->tamanho_linhas - 1) == 0) {
        printf("\nO arquivo nao possui dados.\n");
        return;
    }

    printf("Entre com a variavel: ");
    fgets(word, sizeof(word), stdin);
    word[strlen(word) - 1] = '\0';
    if (atoi(word) != 0) {
        printf("\nEntrada invalida.\n");
        return;
    }
    // Acha coluna
    for (i = 0; i < csv->tamanho_colunas; i++) {
        if (strcmp(word, csv->infos[i].rotulo) == 0) {
            coluna = i;
            break;
        }
    }
    if (i == csv->tamanho_colunas) {
        printf("\nNenhuma coluna selecionada.\n");
        return;
    }

    aux = malloc((csv->tamanho_linhas - 1) * sizeof(unsigned long));
    if (!aux) return;
    for (i = 0; i < csv->tamanho_linhas - 1; i++) {
        aux[i] = csv->vetor_auxiliar->linhas[i];
    }
    // Ordena primeiro
    mergeSort(csv, csv->vetor_auxiliar->linhas, csv->tamanho_linhas - 1, coluna);
    // Se numerico opu string
    if (csv->tipos[coluna] == 0) {
        numerico(csv, coluna);
    } else {
        strings(csv, coluna);
    }
    // Volta as linhas nas posicoes antes de ordenar
    reset_linhas(csv->vetor_auxiliar->linhas, aux, csv->tamanho_linhas - 1);
    free(aux);
    printf("Pressione ENTER para continuar\n");
    getchar();
    return;
}
// =============================================================================

// Funão 5 Ordenação ****
// =============================================================================
void ordenacao(arq_csv *csv) {
    char string[MAX], escolha[MAX], *token;
    int ok;
    unsigned long i, indice, *aux, *tmp;

    if (csv->tamanho_linhas == 1) {
        printf("O arquivo nao possui dados!!\n");
        return;
    }

    printf("Entre com a variavel: ");
    fgets(string, sizeof(string), stdin);
    if (strlen(string) == 1) return;
    string[strlen(string) - 1] = '\0';
    // Aqui verifica qual coluna quero imprimir
    token = strtok(string, " ");
    ok = 0;
    do {
        i = 0;
        while (i < csv->tamanho_colunas) {
            if (strcmp(token, csv->infos[i].rotulo) == 0) {
                indice = i;
                ok = 1;
            }
            i++;
        }
    } while ((token = strtok(NULL, " ")) != NULL);

    if (ok == 0) {
        printf("\nNenhuma coluna selecionada.\n");
        return;
    }
    tmp = malloc((csv->tamanho_linhas - 1) * sizeof(unsigned long));
    if (!tmp) return;
    for (i = 0; i < csv->tamanho_linhas - 1; i++) {
        tmp[i] = csv->vetor_auxiliar->linhas[i]; // Posicao inicial das linhas
    }

    printf("Selecione uma opcao [A]scendente ou [D]ecrescente: ");
    scanf("%s", escolha);
    getchar();
    if (strcmp(escolha, "A") != 0 && strcmp(escolha, "D") != 0) return;
    printf("\n");
    mergeSort(csv, csv->vetor_auxiliar->linhas, csv->tamanho_linhas - 1, indice);
    if (strcmp(escolha, "D") == 0) {
        aux = csv->vetor_auxiliar->linhas;
        copiar_d(csv->vetor_auxiliar->linhas, aux, csv->tamanho_linhas - 1);
    }
    insere_ptr(csv, csv->vetor_auxiliar->linhas_memoria); // Coloca as linhas para mostrar
    mostrar(csv, csv->vetor_auxiliar->linhas_memoria);
    printf("\n[%ld rows x %ld columns]\n\n", csv->vetor_auxiliar->linhas_memoria, csv->tamanho_colunas);
    printf("Deseja gravar um arquivo com as variaveis ordenados? [S|N]: ");
    // Se 1 significa que o usuario nao descartou os dados, entao volta as linhas como estava antes
    if (exibe_solicitacoes(csv, csv->vetor_auxiliar->linhas_memoria, csv->tamanho_colunas))
        reset_linhas(csv->vetor_auxiliar->linhas, tmp, csv->tamanho_linhas - 1);
    free(tmp);
    return;
}
// =============================================================================

// Função 6 Selecao
// =============================================================================
void selecao(arq_csv *csv) {
    char string[MAX], *token, *aux;
    unsigned long i, tamC, *indice, contador;

    if (csv->tamanho_linhas == 1) {
        printf("O arquivo nao possui dados!!\n");
        return;
    }

    aux = malloc(csv->tamanho_colunas);
    if (!aux) {
        perror("Erro ao alocar memória.\n");
        return;
    }
    indice = malloc(csv->tamanho_colunas * sizeof(unsigned long));
    if (!indice) {
        perror("Erro ao alocar memória.\n");
        free(aux);
        return;
    }
    for (i = 0; i < csv->tamanho_colunas; i++) {
        aux[i] = 0;
        indice[i] = i;
    }

    printf("Entre com a variaveis que deseja selecionar (separadas por espaços): ");
    fgets(string, sizeof(string), stdin);
    if (test_tamanho(strlen(string), 1, aux, indice)) return;
    string[strlen(string) - 1] = '\0';
    printf("\n");

    // Aqui verifica qual coluna quero imprimir
    token = strtok(string, " ");
    tamC = 0;
    do {
        i = 0;
        while (i < csv->tamanho_colunas) {
            if (strcmp(token, csv->infos[i].rotulo) == 0) {
                aux[i] = 1;
                indice[tamC] = i;
                tamC++;
            }
            i++;
        }
    } while ((token = strtok(NULL, " ")) != NULL);
    if (test_tamanho(tamC, 0, aux, indice)) {
        return;
    }

    // Aqui deixa a coluna valida do jeito que quero imprimir
    for (i = 1; i < csv->tamanho_colunas + 1; i++) {
        if ((aux[i - 1] != 1) && (csv->vetor_auxiliar->col_valida[i] == 1)) {
            csv->vetor_auxiliar->col_valida[i] = 0;
        }
    }
    // Aqui deixa a ordem de prioridade do jeito que quero imprimir
    contador = 0;
    for (i = 1; i < csv->tamanho_colunas + 1; i++) {
        if (csv->vetor_auxiliar->col_valida[i] == 1) {
            csv->vetor_auxiliar->prioridades[i - 1] = indice[contador];
            contador++;
        }
    }

    free(indice);
    free(aux);

    insere_ptr(csv, csv->tamanho_linhas - 1);
    mostrar(csv, csv->tamanho_linhas - 1);

    printf("\n[%ld rows x %ld columns]\n", csv->vetor_auxiliar->linhas_memoria, tamC);
    printf("\nDeseja gravar um arquivo com as variaveis selecionados? [S|N]: ");
    fgets(string, sizeof(string), stdin);
    string[strlen(string) - 1] = '\0';
    if (strcmp(string, "S") == 0 || strcmp(string, "s") == 0) {
        guardar_dados(csv, tamC);
    } else {
        printf("\nPressione ENTER para continuar\n");
        getchar();
    }
    // Reseta coluna valida e coluna de prioridade
    reset_CL(csv->vetor_auxiliar->col_valida, csv->tamanho_colunas + 1);
    reset_P(csv->vetor_auxiliar->prioridades, csv->tamanho_colunas);
    return;
}
// =============================================================================

// Função 7 Dados Faltantes ****
// =============================================================================
void remove_nan(arq_csv *csv) {
    char string[MAX];
    unsigned int i, j, indicador;
    indicador = 0;
    for (j = 0; j < csv->tamanho_linhas - 1; j++) {
        for (i = 0; i < csv->tamanho_colunas; i++) {
            // Verifica se eh NaN
            if (strcmp(csv->infos[i].dados->word[csv->vetor_auxiliar->linhas[j]], "NaN") == 0) {
                // Deixo a linha em invalido se eh NaN
                csv->vetor_auxiliar->lin_valida[csv->vetor_auxiliar->linhas[j]] = 0;
                indicador++;
                break;
            }
        }
    }
    if (indicador == 0) {
        printf("\nNenhuma linha invalida.\n\n");
        return;
    }
    // Quantidade de linhas recebe a nova quantidade
    csv->vetor_auxiliar->linhas_memoria = csv->vetor_auxiliar->linhas_memoria - indicador;
    insere_ptr(csv, csv->vetor_auxiliar->linhas_memoria);
    mostrar(csv, csv->vetor_auxiliar->linhas_memoria);
    printf("\n[%ld rows x %ld columns]\n", csv->vetor_auxiliar->linhas_memoria, csv->tamanho_colunas);
    printf("\nDeseja gravar um arquivo com as linhas selecionadas? [S|N]: ");
    fgets(string, sizeof(string), stdin);
    string[strlen(string) - 1] = '\0';
    if (strcmp(string, "S") == 0 || strcmp(string, "s") == 0) {
        guardar_dados(csv, csv->tamanho_colunas);
    } else {
        printf("\nPressione ENTER para continuar\n");
        getchar();
    }
    return;
}

void substitui_proximo(arq_csv *csv) {
    char word[MAX], string[MAX];
    unsigned long i, j, posicao;

    // Os if serve para verificacao das linhas validas
    for (i = 0; i < csv->tamanho_colunas; i++) {
        for (j = 0; j < csv->tamanho_linhas - 1; j++) {
            if (csv->vetor_auxiliar->lin_valida[csv->vetor_auxiliar->linhas[j]] == 1) {
                if (strcmp(csv->infos[i].dados->word[csv->vetor_auxiliar->linhas[j]], "NaN") == 0) {
                    posicao = devolve_proximo(csv, i, j);
                    // Se a posicao eh maior que tamanho de linhas significa que nao tem proximo valor valido
                    if (posicao > (csv->tamanho_linhas - 1)) {
                        break;
                    }
                    free(csv->infos[i].dados->word[csv->vetor_auxiliar->linhas[j]]);
                    strcpy(word, csv->infos[i].dados->word[csv->vetor_auxiliar->linhas[posicao]]);
                    csv->infos[i].dados->word[csv->vetor_auxiliar->linhas[j]] = strdup(word);
                }
            }
        }
    }
    insere_ptr(csv, csv->vetor_auxiliar->linhas_memoria);
    mostrar(csv, csv->vetor_auxiliar->linhas_memoria);
    printf("\n[%ld rows x %ld columns]\n", csv->vetor_auxiliar->linhas_memoria, csv->tamanho_colunas);
    printf("\nDeseja gravar um arquivo com as variaveis substituidas? [S|N]: ");
    fgets(string, sizeof(string), stdin);
    string[strlen(string) - 1] = '\0';
    if (strcmp(string, "S") == 0 || strcmp(string, "s") == 0) {
        guardar_dados(csv, csv->tamanho_colunas);
    } else {
        printf("\nPressione ENTER para continuar\n");
        getchar();
    }
    return;
}

void substitui_media(arq_csv *csv) {
    char string[MAX];
    unsigned long i, j, qtd_tipos, contador, indicador;
    long double *aux;

    qtd_tipos = 0;
    for (i = 0; i < csv->tamanho_colunas; i++) {
        if (csv->tipos[i] == 0)
            qtd_tipos++;
    }
    
    aux = malloc(qtd_tipos * sizeof(long double));
    if (!aux) {
        perror("Erro ao alocar memória.\n");
        return;
    }
    contador = 0;
    for (i = 0; i < csv->tamanho_colunas; i++) {
        // Tipo numeric
        if (csv->tipos[i] == 0) {
            aux[contador] = calcula_media(csv, i);
            contador++;
        }
    }
    // Coloca a media nos valores invalidos
    indicador = 0;
    for (j = 0; j < csv->tamanho_linhas - 1; j++) {
        if (csv->vetor_auxiliar->lin_valida[csv->vetor_auxiliar->linhas[j]] == 1) {
            contador = 0;
            for (i = 0; i < csv->tamanho_colunas; i++) {
                if (csv->tipos[i] == 0) {
                    if (strcmp(csv->infos[i].dados->word[csv->vetor_auxiliar->linhas[j]], "NaN") == 0) {
                        free(csv->infos[i].dados->word[csv->vetor_auxiliar->linhas[j]]);
                        sprintf(string, "%.2Lf", aux[contador]);
                        csv->infos[i].dados->word[csv->vetor_auxiliar->linhas[j]] = strdup(string);
                        bzero(string, sizeof(string));
                    }
                    contador++;
                }
            }
            indicador++;
        }
    }
    free(aux);
    insere_ptr(csv, indicador);
    mostrar(csv, indicador);
    printf("\n[%ld rows x %ld columns]\n", csv->vetor_auxiliar->linhas_memoria, csv->tamanho_colunas);
    printf("\nDeseja gravar um arquivo com as variaveis substituidas? [S|N]: ");
    fgets(string, sizeof(string), stdin);
    string[strlen(string) - 1] = '\0';
    if (strcmp(string, "S") == 0 || strcmp(string, "s") == 0) {
        guardar_dados(csv, csv->tamanho_colunas);
    } else {
        printf("\nPressione ENTER para continuar\n");
        getchar();
    }
    return;
}

void listar_nan(arq_csv *csv) {
    char *aux;
    unsigned long i, j, indicador;
    // Aqui define todas linhas invalidas
    aux = malloc(csv->tamanho_linhas - 1);
    if (!aux) {
        perror("Erro de alocacao de memoria.");
        return;
    }
    // Deixa todas linhas invalidas e salva com vetor auxiliar as linhas validas iniciais
    for (j = 0; j < csv->tamanho_linhas - 1; j++) {
        aux[j] = csv->vetor_auxiliar->lin_valida[csv->vetor_auxiliar->linhas[j]];
        csv->vetor_auxiliar->lin_valida[csv->vetor_auxiliar->linhas[j]] = 0;
    }

    indicador = 0;
    for (j = 0; j < csv->tamanho_linhas - 1; j++) {
        if (aux[j] == 1) {
            for (i = 0; i < csv->tamanho_colunas; i++) {
                if (strcmp(csv->infos[i].dados->word[csv->vetor_auxiliar->linhas[j]], "NaN") == 0) {
                    csv->vetor_auxiliar->lin_valida[csv->vetor_auxiliar->linhas[j]] = 1;
                    indicador++;
                    break;
                }
            }
        }
    }
    if (indicador == 0) {
        printf("\nNenhuma linha invalida.\n\n");
        for (j = 0; j < csv->tamanho_linhas - 1; j++) {
            csv->vetor_auxiliar->lin_valida[csv->vetor_auxiliar->linhas[j]] = aux[j];
        }
        free(aux);
        return;
    }
    insere_ptr(csv, indicador);
    mostrar(csv, indicador);
    printf("\n[%ld rows x %ld columns]\n\n", indicador, csv->tamanho_colunas);
    printf("Deseja gravar um arquivo com as linhas selecionadas? [S|N]: ");
    // Se o usuario nao descartar os dados originais volto normal as linhas validas
    if (exibe_solicitacoes(csv, indicador, csv->tamanho_colunas)) {
        for (j = 0; j < csv->tamanho_linhas - 1; j++) {
            csv->vetor_auxiliar->lin_valida[csv->vetor_auxiliar->linhas[j]] = aux[j];
        }
    }
    free(aux);
    return;
}

void dados_faltantes(arq_csv *csv) {
    char string[MAX];
    do {
        printf("\t1) Listar registros com NaN\n");
        printf("\t2) Substituir pela media\n");
        printf("\t3) Substituir pelo proximo valor valido\n");
        printf("\t4) Remover registros com NaN\n");
        printf("\t5) Voltar ao menu principal\n");
        printf("\tDigite sua opcao: ");
        fgets(string, sizeof(string), stdin);
        if (!atoi(string)) {
            printf("\nOpcao inválida.\n");
            return;
        }

        switch (atoi(string)) {
        case 1:
            listar_nan(csv);
            break;
        case 2:
            substitui_media(csv);
            break;
        case 3:
            substitui_proximo(csv);
            break;
        case 4:
            remove_nan(csv);
            break;
        case 5:
            break;
        default:
            printf("Opcao inválida. Escolha novamente.\n\n");
        }
    } while (atoi(string) != 5);
    printf("\nPressione ENTER para continuar\n");
    getchar();
    return;
}
// =============================================================================

// Função 8 guarda dados
// =============================================================================
int insere_dados(arq_csv *csv, char *file_name, unsigned long size) {
    unsigned long i, j, contador;
    FILE *new_file;

    new_file = fopen(file_name, "w");
    if (!new_file) {
        printf("\nArquivo nao aberto.\n");
        return 0;
    }

    contador = 0;
    // Aqui guarda os rotulos
    for (i = 0; i < csv->tamanho_colunas; i++) {
        if (csv->vetor_auxiliar->col_valida[i + 1] == 1) {
            fputs(csv->infos[csv->vetor_auxiliar->prioridades[i]].rotulo, new_file);
            if (contador < (size - 1)) fputs(",", new_file);
            contador++;
        }
    }
    fputs("\n", new_file);

    // Aqui guarda os dados
    for (j = 0; j < csv->tamanho_linhas - 1; j++) {
        contador = 0;
        if (csv->vetor_auxiliar->lin_valida[j] == 1) {
            for (i = 0; i < csv->tamanho_colunas; i++) {
                if (csv->vetor_auxiliar->col_valida[i + 1] == 1) {
                    if (strcmp(csv->infos[i].dados->word[csv->vetor_auxiliar->linhas[j]], "NaN") == 0) {
                        if (contador < (size - 1)) fputs(",", new_file);
                    } else {
                        fputs(csv->infos[csv->vetor_auxiliar->prioridades[i]].dados->word[csv->vetor_auxiliar->linhas[j]], new_file);
                        if (contador < (size - 1)) fputs(",", new_file);
                    }
                    contador++;
                }
            }
            fputs("\n", new_file);
        }
    }

    fclose(new_file);
    return 1;
}

void guardar_dados(arq_csv *csv, unsigned long tamanho_coluna) {
    char file_name[MAX];
    int ok;

    printf("Entre com o nome do arquivo: ");
    fgets(file_name, sizeof(file_name), stdin);
    file_name[strlen(file_name) - 1] = '\0';

    ok = insere_dados(csv, file_name, tamanho_coluna);

    if (ok == 1) printf("Arquivo gravado com sucesso.\n");
    else printf("\nFalha ao colocar dados no arquivo.\n");

    printf("Pressione ENTER para continuar\n");
    getchar();
    return;
}
// =============================================================================

// Libera memórias
// =============================================================================
void libera_matriz(arq_csv *csv) {
    unsigned long i, j;
    // Libera memoria dos dados da coluna
    for (i = 0; i < csv->tamanho_linhas - 1; i++) {
        for (j = 0; j < csv->tamanho_colunas; j++) {
            free(csv->infos[j].dados->word[i]);
        }
    }
    // Libera memoria da matriz
    for (i = 0; i < csv->tamanho_colunas; i++) {
        free(csv->infos[i].rotulo);
        free(csv->infos[i].dados->word);
        free(csv->infos[i].dados);
    }

    free(csv->infos);
    return;
}

void fecha_arquivo(arq_csv *csv) {
    free(csv->tipos);
    free(csv->vetor_auxiliar->col_valida);
    free(csv->vetor_auxiliar->lin_valida);
    free(csv->vetor_auxiliar->prioridades);
    free(csv->vetor_auxiliar->linhas);
    free(csv->vetor_auxiliar);
    libera_matriz(csv);
    fclose(csv->arquivo);
    free(csv);
    return;
}
// =============================================================================