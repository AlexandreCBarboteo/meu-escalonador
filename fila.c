// fila.c
#include <stdio.h>
#include <stdlib.h>
#include "fila.h"  // Inclui o arquivo de cabeçalho

// Função para criar uma fila vazia
Fila* criar_fila() {
    Fila* fila = (Fila*)malloc(sizeof(Fila));
    fila->frente = fila->traseira = NULL;
    return fila;
}

// Função para verificar se a fila está vazia
int fila_vazia(Fila* fila) {
    return fila->frente == NULL;
}

// Função para inserir um processo na fila
void inserir(Fila* fila, Processo processo) {
    No* novo_no = (No*)malloc(sizeof(No));
    novo_no->processo = processo;
    novo_no->prox = NULL;

    if (fila->traseira == NULL) {
        fila->frente = fila->traseira = novo_no;
    } else {
        fila->traseira->prox = novo_no;
        fila->traseira = novo_no;
    }
}

// Função para remover um processo da fila
Processo remover(Fila* fila) {
    if (fila_vazia(fila)) {
        printf("Fila vazia! Não é possível remover.\n");
        exit(1);
    }

    No* temp = fila->frente;
    Processo processo_removido = temp->processo;

    fila->frente = fila->frente->prox;

    if (fila->frente == NULL) {
        fila->traseira = NULL;
    }

    free(temp);
    return processo_removido;
}

// Função para exibir os processos na fila
void exibir_fila(Fila* fila) {
    if (fila_vazia(fila)) {
        printf("Fila vazia!\n");
        return;
    }

    No* atual = fila->frente;
    while (atual != NULL) {
        printf("Processo ID: %d\n", atual->processo.id);
        atual = atual->prox;
    }
}
