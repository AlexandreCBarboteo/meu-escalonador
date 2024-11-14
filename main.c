#include <stdio.h>
#include <stdlib.h>
#include "fila.h"

#define TEMPO_A 2
#define TEMPO_B 3
#define TEMPO_C 4

int ler_processos(const char* nome_arquivo, Processo** processos);

/* PARA LEMBRAR:
// Definição da struct Processo
    typedef struct Processo {
        int id;
        int tempo_chegada;
        int burst_time;
        int tempo_restante;
        int prioridade;
    } Processo;
    
 */


int main(int argc, char *argv[]) {
    int tempo = 0;
    int processos_concluidos = 0;
    int num_processos = 3;


    // Criação das filas
    Fila *fila_alta_prioridade = criar_fila();
    Fila *fila_baixa_prioridade = criar_fila();
    Fila *fila_io = criar_fila();

    // Criar alguns processos
    Processo processo1 = {1, 0, 5, 0, 1,2,0};
    Processo processo2 = {2, 2, 3, 0, 2,2,1};
    Processo processo3 = {3, 4, 8, 0, 3,5,2};

    // Inserir os processos na fila
    inserir(fila_alta_prioridade, processo1);
    inserir(fila_alta_prioridade, processo2);
    inserir(fila_alta_prioridade, processo3);
    
    // Carregar processos de um arquivo txt
    Processo* processos = NULL;
    int num_processos = ler_processos("processos.txt", &processos);
    
    // Verifica se pelo menos um processo foi carregado
    if (num_processos == 0) {
        printf("Nenhum processo carregado. Encerrando...\n");
        return 1;
    }
    
    /* ESCALONADOR ABAIXO */
    while(processos_concluidos<num_processos)
    {
        /*
        for (int i = 0; i < num_processos; i++) {
            if (processos[i].tempo_chegada == tempo && processos[i].tempo_restante > 0) {
                inserir(&filas[], processos[i]);
                printf("Processo %d chegou e foi adicionado à fila de alta prioridade em t=%d\n", processos[i].id, tempo);
            }
        }
        */        

        if (!fila_vazia(fila_alta_prioridade)) {
            // Remove o primeiro processo da fila para simular a execução
            Processo processo_executado = remover(fila_alta_prioridade);

            // Verifica se o processo precisa realizar I/O
            if (processo_executado.tempo_executado == processo_executado.tempo_io) {
                // Move o processo para a fila de I/O
                inserir(fila_io, processo_executado);
                printf("Processo ID %d movido para a Fila de I/O após executar por %d ciclos\n", processo_executado.id, processo_executado.tempo_executado);
            } else {
                // Se não precisar de I/O, continua a execução normal
                printf("Processo ID %d está sendo executado na Fila de Alta Prioridade em t=%d\n", processo_executado.id, tempo);

                // Incrementa o tempo executado do processo
                processo_executado.tempo_executado++;

                // Verifica se o processo foi concluído
                if (processo_executado.tempo_executado >= processo_executado.burst_time) {
                    processos_concluidos++;
                    printf("Processo ID %d concluído após %d ciclos de execução.\n", processo_executado.id, processo_executado.tempo_executado);
                } else {
                    // Insere o processo na fila de Baixa Prioridade se ainda não foi concluído
                    inserir(fila_baixa_prioridade, processo_executado);
                }
            }
        }

        // Verifica se há processos na fila de baixa prioridade
        if (!fila_vazia(fila_baixa_prioridade)) {
            // Remove o primeiro processo da fila de baixa prioridade
            Processo processo_executado = remover(fila_baixa_prioridade);

            // Verifica se o processo precisa realizar I/O
            if (processo_executado.tempo_executado == processo_executado.tempo_io) {
                // Move o processo para a fila de I/O
                inserir(fila_io, processo_executado);
                printf("Processo ID %d movido para a Fila de I/O após executar por %d ciclos\n", processo_executado.id, processo_executado.tempo_executado);
            } else {
                // Se não precisar de I/O, continua a execução normal
                printf("Processo ID %d está sendo executado na Fila de Baixa Prioridade em t=%d\n", processo_executado.id, tempo);

                // Incrementa o tempo executado do processo
                processo_executado.tempo_executado++;

                // Verifica se o processo foi concluído
                if (processo_executado.tempo_executado >= processo_executado.burst_time) {
                    processos_concluidos++;
                    printf("Processo ID %d concluído após %d ciclos de execução.\n", processo_executado.id, processo_executado.tempo_executado);
                } else {
                    // Reinsere o processo na fila de baixa prioridade se ainda não foi concluído
                    inserir(fila_baixa_prioridade, processo_executado);
                }
            }
        }

        // Verifica se há processos na fila de I/O
        if (!fila_vazia(fila_io)) {
            // Remove o primeiro processo da fila de I/O
            Processo processo_executado = remover(fila_io);

            // Checa o tipo de I/O
            if (processo_executado.tipo_io == 'A') {
                // Verifica se o tempo de execução ainda não ultrapassou o tempo necessário para I/O
                if (processo_executado.tempo_executado < processo_executado.tempo_io + TEMPO_A) {
                    // Avança o tempo de execução
                    processo_executado.tempo_executado++;

                    // Verifica se o I/O foi concluído
                    if (processo_executado.tempo_executado == processo_executado.tempo_io + TEMPO_A) {
                        inserir(fila_alta_prioridade, processo_executado);
                        printf("Processo ID %d completou I/O (Tipo A) e retornou para a Fila de Alta Prioridade.\n", processo_executado.id);
                    }
                }
            }
            else if (processo_executado.tipo_io == 'B') {
                // Verifica se o tempo de execução ainda não ultrapassou o tempo necessário para I/O
                if (processo_executado.tempo_executado < processo_executado.tempo_io + TEMPO_B) {
                    // Avança o tempo de execução
                    processo_executado.tempo_executado++;

                    // Verifica se o I/O foi concluído
                    if (processo_executado.tempo_executado == processo_executado.tempo_io + TEMPO_B) {
                        inserir(fila_alta_prioridade, processo_executado);
                        printf("Processo ID %d completou I/O (Tipo B) e retornou para a Fila de Alta Prioridade.\n", processo_executado.id);
                    }
                }
            }
            else if (processo_executado.tipo_io == 'C') {
                // Verifica se o tempo de execução ainda não ultrapassou o tempo necessário para I/O
                if (processo_executado.tempo_executado < processo_executado.tempo_io + TEMPO_C) {
                    // Avança o tempo de execução
                    processo_executado.tempo_executado++;

                    // Verifica se o I/O foi concluído
                    if (processo_executado.tempo_executado == processo_executado.tempo_io + TEMPO_C) {
                        inserir(fila_baixa_prioridade, processo_executado);
                        printf("Processo ID %d completou I/O (Tipo C) e retornou para a Fila de Baixa Prioridade.\n", processo_executado.id);
                    }
                }
            }
        }

    }

    /* ESCALONADOR ACIMA */

  return 0;
}

// Função para ler os processos de um arquivo txt
/*
int ler_processos(const char* nome_arquivo, Processo** processos) {
    FILE* arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 0;  // Retorna 0 se não for possível abrir o arquivo
    }

    int num_processos = 0;
    Processo p;
    while (fscanf(arquivo, "%d %d %d", &p.id, &p.tempo_chegada, &p.tempo_servico) == 3) {
        p.tempo_restante = p.tempo_servico;  // Inicializa o tempo restante com o tempo_servico
        p.prioridade = 0;  // Inicializa a prioridade (0 para a fila de alta prioridade)
        
        (*processos) = realloc(*processos, (num_processos + 1) * sizeof(Processo));
        (*processos)[num_processos++] = p;
    }

    fclose(arquivo);
    return num_processos;
}
*/