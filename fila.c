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

/*
Node* remover_io(Node* head) {
    // Caso a lista esteja vazia
    if (head == NULL) {
        return NULL;
    }
    
    // Caso o nó a ser removido seja o primeiro (cabeça da lista)
    if (head->processo->tempo_retorno_io == tempo) {
        Node* temp = head;
        head = head->proximo;  // Atualiza a cabeça para o próximo nó
        return temp; // Retorna o nó que foi apagado
    }
    
    // Caso o nó a ser removido não seja o primeiro
    Node* atual = head;
    while (atual->proximo != NULL) {
        if (atual->proximo->processo->tempo_retorno_io == tempo) {
            Node* temp = atual->proximo;
            atual->proximo = atual->proximo->proximo;  // Atualiza o ponteiro do nó anterior
            return temp; // Retorna o nó que foi apagado
        }
        atual = atual->proximo;
    }
    
    return NULL;  // Se o valor não foi encontrado, retorna a lista original
}

*/

// Função auxiliar para calcular o tamanho da fila (opcional)
int tamanho_fila(Fila* fila) {
    int contador = 0;
    No* atual = fila->frente;
    while (atual != NULL) {
        contador++;
        atual = atual->prox;
    }
    return contador;
}