#include <stdio.h>
#include <stdlib.h>
#include "fila.h"

// Definições e constantes
#define DISCO 2
#define FITA 3
#define IMPRESSORA 4
#define NONE -1

// Funções auxiliares
void carregar_processos_iniciais(Fila* fila, Processo* processos, int num_processos, int tempo_atual);
void executar_processo(Fila* fila_alta, Fila* fila_baixa, Fila* fila_io, Processo processo, int* processos_concluidos, int tempo);
void gerenciar_io(Fila* fila_io, Fila* fila_alta, Fila* fila_baixa, int tempo);
int ler_processos(const char* nome_arquivo, Processo** processos);

// Função principal
int main(int argc, char* argv[]) {
    int tempo = 0, processos_concluidos = 0;

    // Criar filas
    Fila* fila_alta_prioridade = criar_fila();
    Fila* fila_baixa_prioridade = criar_fila();
    Fila* fila_io = criar_fila();

    // Carregar processos de arquivo
    Processo* processos = NULL;
    int num_processos = ler_processos("processos.txt", &processos);

    if (num_processos == 0) {
        printf("Nenhum processo carregado. Encerrando...\n");
        return 1;
    }

    // Loop principal do escalonador
    while (processos_concluidos < num_processos) {
        carregar_processos_iniciais(fila_alta_prioridade, processos, num_processos, tempo);

        if (!fila_vazia(fila_alta_prioridade)) {
            Processo processo = remover(fila_alta_prioridade);
            executar_processo(fila_alta_prioridade, fila_baixa_prioridade, fila_io, processo, &processos_concluidos, tempo);
        } else if (!fila_vazia(fila_baixa_prioridade)) {
            Processo processo = remover(fila_baixa_prioridade);
            executar_processo(fila_alta_prioridade, fila_baixa_prioridade, fila_io, processo, &processos_concluidos, tempo);
        }

        gerenciar_io(fila_io, fila_alta_prioridade, fila_baixa_prioridade, tempo);
        tempo++;
    }

    printf("Todos os processos foram concluídos!\n");
    return 0;
}

// Implementação das funções auxiliares
void carregar_processos_iniciais(Fila* fila, Processo* processos, int num_processos, int tempo_atual) {
    for (int i = 0; i < num_processos; i++) {
        if (processos[i].tempo_chegada == tempo_atual) {
            inserir(fila, processos[i]);
            printf("Processo %d chegou e foi adicionado à fila de alta prioridade em t=%d\n", processos[i].id, tempo_atual);
        }
    }
}

void executar_processo(Fila* fila_alta, Fila* fila_baixa, Fila* fila_io, Processo processo, int* processos_concluidos, int tempo) {
    int fez_io = 0;

    for (int i = 0; i < 3; i++) {
        if (processo.tempo_executado == processo.tempo_inicio_io[i]) {
            fez_io = 1;
            processo.tempo_retorno_io = tempo + (i == 0 ? DISCO : (i == 1 ? FITA : IMPRESSORA));
            processo.atual_io = i;
            inserir(fila_io, processo);
            printf("Processo ID %d movido para a fila de I/O em t=%d\n", processo.id, tempo);
            return;
        }
    }

    if (!fez_io) {
        processo.tempo_executado++;
        if (processo.tempo_executado >= processo.tempo_servico) {
            (*processos_concluidos)++;
            printf("Processo ID %d concluído em t=%d\n", processo.id, tempo);
        } else {
            inserir(fila_alta, processo);
        }
    }
}

void gerenciar_io(Fila* fila_io, Fila* fila_alta, Fila* fila_baixa, int tempo) {
    if (!fila_vazia(fila_io)) {
        Processo processo = remover(fila_io);
        if (tempo == processo.tempo_retorno_io) {
            if (processo.atual_io == 0) {
                inserir(fila_baixa, processo);
            } else {
                inserir(fila_alta, processo);
            }
            printf("Processo ID %d voltou do I/O em t=%d\n", processo.id, tempo);
        } else {
            inserir(fila_io, processo);
        }
    }
}
// Funcao para ler os processos de um arquivo txt
int ler_processos(const char* nome_arquivo, Processo** processos) {
    FILE* arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 0;  // Retorna 0 se nao for possivel abrir o arquivo
    }

    int num_processos = 0;
    Processo p;
    while (fscanf(arquivo, "%d %d %d %d %d %d",
                &p.id, &p.tempo_chegada, &p.tempo_servico,
                &p.tempo_inicio_io[0], &p.tempo_inicio_io[1], &p.tempo_inicio_io[2]) == 6) {
        // p.tempo_restante = p.tempo_servico;  // Inicializa o tempo restante com o tempo_servico
        p.tempo_executado = 0;
        p.tempo_retorno_io = 0;
        p.atual_io = NONE;
        (*processos) = realloc(*processos, (num_processos + 1) * sizeof(Processo));
        (*processos)[num_processos++] = p;
    }

    fclose(arquivo);
    return num_processos;
}// (teste? v : f)