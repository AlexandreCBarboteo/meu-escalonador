#include <stdio.h>
#include <stdlib.h>
#include "fila.h"

#define DISCO 0 // Identifica Tipos de I/O. Usado nos Switch Cases e ternários
#define FITA 1
#define IMPRESSORA 2

#define TEMPO_DISCO 1 // Usado para saber quando o processo deve retornar do I/O
#define TEMPO_FITA 1
#define TEMPO_IMPRESSORA 1

#define FALSE 0 
#define TRUE 1
#define NONE -1

#define ALTA 0    // ADICIONAR COMENTARIO DEPOIS
#define BAIXA 1

#define ALTA_BAIXA 1
#define ALTA_ALTA 2

int ler_processos(const char* nome_arquivo, Processo** processos);
void carregar_processos_iniciais(Fila* fila_alta_prioridade, Processo* processos, int num_processos, int tempo);
void executar_processo(Fila* fila_alta_prioridade, Fila* fila_baixa_prioridade, Fila* fila_io, int tempo, int* processos_concluidos,
                        /**/ Processo* processos, int num_processos);
void tratar_io(Fila* fila_io, Fila* fila_alta_prioridade, Fila* fila_baixa_prioridade, int tempo);
int verificar_processos_iniciais(Processo* processos, int num_processos, int tempo);

int main(int argc, char *argv[]) {
    int tempo = 0;
    int processos_concluidos = 0;
    int fez_io = FALSE;
    int num_processos;
    int sentido;

    // Criacao das filas
    Fila *fila_alta_prioridade = criar_fila();
    Fila *fila_baixa_prioridade = criar_fila();
    Fila *fila_io = criar_fila();
    
    // Carregar processos de um arquivo txt
    Processo* processos = NULL;
    num_processos = ler_processos("processos.txt", &processos);
    
    // Verifica se pelo menos um processo foi carregado
    if (num_processos == 0) {
        printf("Nenhum processo carregado. Encerrando...\n");
        return 1;
    }
    
    /* ESCALONADOR */
    while(processos_concluidos<num_processos)
    {
        printf("INSTANTE %d:\n", tempo);
        carregar_processos_iniciais(fila_alta_prioridade, processos, num_processos, tempo);
        
        // Executar processo de alta prioridade, se disponível
        if (!fila_vazia(fila_alta_prioridade)) {
            // Remove o primeiro processo da fila para simular a execução
            //Processo processo_executado = remover(fila_alta_prioridade);
            //processo_executado.fila_origem = ALTA;           
            executar_processo_alta( fila_alta_prioridade, fila_baixa_prioridade, fila_io, tempo, 
                            &processos_concluidos,/**/processos, num_processos);
            
        } else if (!fila_vazia(fila_baixa_prioridade)) {
            // Executar processo de baixa prioridade, se não houver de alta
            //Processo processo_executado = remover(fila_baixa_prioridade);
            //processo_executado.fila_origem = BAIXA;
            executar_processo( fila_baixa_prioridade, fila_baixa_prioridade, fila_io, tempo, &processos_concluidos,
                        /**/processos, num_processos);
        }

        // Tratar processos na fila de I/O
        tratar_io(fila_io, fila_alta_prioridade, fila_baixa_prioridade, tempo);
        //printf("\n");
        tempo++;
    }
    printf("Escalonador Encerrou depois de %d ut\n", --tempo);
    /* FIM ESCALONADOR */
  return 0;
}

void carregar_processos_iniciais(Fila* fila_alta_prioridade, Processo* processos, int num_processos, int tempo) {
    for (int i = 0; i < num_processos; i++) {
        if (processos[i].tempo_chegada == tempo ) {
            inserir(fila_alta_prioridade, processos[i]);
            printf("Processo %d chegou e foi adicionado à fila de alta prioridade\n", processos[i].id);
        }
    }
}

int verificar_processos_iniciais(Processo* processos, int num_processos, int tempo){
    for (int i = 0; i < num_processos; i++) {
        if (processos[i].tempo_chegada == tempo ) {
            return TRUE;
        }
    }
    return FALSE;
}

void executar_processo(Fila* fila_alta_prioridade, Fila* fila_baixa_prioridade, Fila* fila_io, int tempo, int* processos_concluidos,
                        /**/ Processo* processos, int num_processos) {
    // Cria uma cópia local do processo para eliminar o uso de "->"
    int fez_io = FALSE;
Processo processo_executado = remover(fila_alta_prioridade);
    if (verificar_processos_iniciais(processos, num_processos, tempo)){
                
    }

    for (int i = 0; i < 3; i++) {
        if (processo_executado.tempo_executado == processo_executado.tempo_inicio_io[i]) {
            switch (i) {
            case DISCO:
                processo_executado.tempo_retorno_io = tempo + TEMPO_DISCO;
                break;
            case FITA:
                processo_executado.tempo_retorno_io = tempo + TEMPO_FITA;
                break;
            case IMPRESSORA:
                processo_executado.tempo_retorno_io = tempo + TEMPO_IMPRESSORA;
                break;
            }
            processo_executado.atual_io = i;
            processo_executado.tempo_inicio_io[i] = NONE; // Prevenir loops infinitos
            inserir(fila_io, processo_executado);
            printf("Processo ID %d movido da fila de %s para I/O (tipo %s) após executar por %d ciclos.\n",
                processo_executado.id,
                (processo_executado.fila_origem == ALTA ? "ALTA" : "BAIXA"),
                (i == ALTA ? "DISCO" : (i == BAIXA ? "FITA" : "IMPRESSORA")),
                processo_executado.tempo_executado);
            fez_io = TRUE;
            break;
        }
    }

    if (!fez_io) {
        Processo processo_executado = remover(fila_alta_prioridade);
        processo_executado.tempo_executado++;
        printf("Processo ID %d está sendo executado na Fila de %s prioridade\n", 
            processo_executado.id, 
            (processo_executado.fila_origem == ALTA ? "ALTA" : "BAIXA"));
        
        if (processo_executado.tempo_executado >= processo_executado.tempo_servico) {
            (*processos_concluidos)++;
            printf("Processo ID %d concluído após %d ciclos de execução.\n",
                processo_executado.id,
                processo_executado.tempo_executado);
        } else {
            /*
            if (verificar_processos_iniciais(processos, num_processos, tempo)){
                
            }
            */
            
            //inserir(fila_destino, processo_executado);
        }
    }
    // Atualiza o processo original (de volta para o ponteiro)
}

// Função para tratar I/O
void tratar_io(Fila* fila_io, Fila* fila_alta_prioridade, Fila* fila_baixa_prioridade, int tempo) {
    int tamanho_io = tamanho_fila(fila_io);
    int n = 0;
    while(n++ < tamanho_io) {
        Processo processo_io = remover(fila_io);
        if (tempo == processo_io.tempo_retorno_io) {
            //printf("tempo igual ao retorno do i/o\n");
            if (processo_io.atual_io == DISCO) {
                    inserir(fila_baixa_prioridade, processo_io);
            } else {
                inserir(fila_alta_prioridade, processo_io);
            }
                printf("Processo ID %d voltou do I/O (tipo %s)\n", 
                    processo_io.id, 
                    (processo_io.atual_io == DISCO ? "DISCO" : (processo_io.atual_io == FITA ? "FITA" : "IMPRESSORA")));
            } else {
                inserir(fila_io, processo_io);
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
        p.fila_origem = 0;
        (*processos) = realloc(*processos, (num_processos + 1) * sizeof(Processo));
        (*processos)[num_processos++] = p;
    }

    fclose(arquivo);
    return num_processos;
}
