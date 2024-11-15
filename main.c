#include <stdio.h>
#include <stdlib.h>
#include "fila.h"

#define TEMPO_A 2
#define TEMPO_B 3
#define TEMPO_C 4
#define A 0 // identificador Switch case. Depois mudar para nomes IO: impressora...
#define B 1
#define C 2

int ler_processos(const char* nome_arquivo, Processo** processos);

int main(int argc, char *argv[]) {
    int tempo = 0;
    int processos_concluidos = 0;
    int num_processos = 3;
    int fez_io = 0;

    // Criação das filas
    Fila *fila_alta_prioridade = criar_fila();
    Fila *fila_baixa_prioridade = criar_fila();
    Fila *fila_io = criar_fila();

    // Criar alguns processos /////////// Tem que sumir. Precisa vir de leitura de arquivo
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
        /* Carrega os processos NOVOS
        for (int i = 0; i < num_processos; i++) {
            if (processos[i].tempo_chegada == tempo && processos[i].tempo_restante > 0) {
                inserir(fila_alta_prioridade, processos[i]);
                printf("Processo %d chegou e foi adicionado à fila de alta prioridade em t=%d\n", processos[i].id, tempo);
            }
        }
        */    
        if (!fila_vazia(fila_alta_prioridade)) {
            // Remove o primeiro processo da fila para simular a execução
            Processo processo_executado = remover(fila_alta_prioridade);

            fez_io=0;
            for(int i=0;i<3;i++){
                if(processo_executado.tempo_executado == processo_executado.tempo_inicio_io[i]){
                    fez_io =1;
                    switch(i) { //direcionar tipo de IO
                        case 0: // A
                                // Move o processo para a fila de I/O
                                processo_executado.tempo_retorno_io = tempo + TEMPO_A;
                                processo_executado.atual_io = 0; 
                                inserir(fila_io, processo_executado);
                                printf("Processo ID %d movido para a Fila de I/O após executar por %d ciclos\n", 
                                            processo_executado.id, processo_executado.tempo_executado);
                                break;                

                        case 1: // B
                                // Move o processo para a fila de I/O
                                processo_executado.tempo_retorno_io = tempo + TEMPO_B;
                                processo_executado.atual_io = 1;
                                inserir(fila_io, processo_executado);
                                printf("Processo ID %d movido para a Fila de I/O após executar por %d ciclos\n", 
                                            processo_executado.id, processo_executado.tempo_executado);
                                break;
                        case 2: // C
                                // Move o processo para a fila de I/O
                                processo_executado.tempo_retorno_io = tempo + TEMPO_C;
                                processo_executado.atual_io = 2;
                                inserir(fila_io, processo_executado);
                                printf("Processo ID %d movido para a Fila de I/O após executar por %d ciclos\n", 
                                            processo_executado.id, processo_executado.tempo_executado);
                                break;
                    }
                }   
            }
            
            if(fez_io == 0){
                // Se não precisar de I/O, continua a execução normal
                printf("Processo ID %d está sendo executado na Fila de Alta Prioridade em t=%d\n", processo_executado.id, tempo);

                // Incrementa o tempo executado do processo
                processo_executado.tempo_executado++;

                // Verifica se o processo foi concluído
                if (processo_executado.tempo_executado >= processo_executado.burst_time) {
                    processos_concluidos++;
                    printf("Processo ID %d concluído após %d ciclos de execução.\n", processo_executado.id, processo_executado.tempo_executado);
                } else if (fila_vazia(fila_alta_prioridade)) { // retorna true se estiver vazia
                    // Mantém o processo na fila de Alta Prioridade se ainda não foi concluído
                    inserir(fila_alta_prioridade, processo_executado);
                } else {
                    // Insere o processo na fila de Baixa Prioridade se ainda não foi concluído
                    inserir(fila_baixa_prioridade, processo_executado);
                }
            }
        }

        // Verifica se há processos na fila de baixa prioridade
        else if (!fila_vazia(fila_baixa_prioridade)) {
            // Remove o primeiro processo da fila de baixa prioridade
            Processo processo_executado = remover(fila_baixa_prioridade);
            
            fez_io=0;
            for(int i=0;i<3;i++){
                if(processo_executado.tempo_executado == processo_executado.tempo_inicio_io[i]){
                    fez_io =1;
                    switch(i) { //direcionar tipo de IO
                        case 0: // A
                                // Move o processo para a fila de I/O
                                processo_executado.tempo_retorno_io = tempo + TEMPO_A;
                                processo_executado.atual_io = 0;
                                inserir(fila_io, processo_executado);
                                printf("Processo ID %d movido para a Fila de I/O após executar por %d ciclos\n", 
                                            processo_executado.id, processo_executado.tempo_executado);
                                break;                

                        case 1: // B
                                // Move o processo para a fila de I/O
                                processo_executado.tempo_retorno_io = tempo + TEMPO_B;
                                processo_executado.atual_io = 1;
                                inserir(fila_io, processo_executado);
                                printf("Processo ID %d movido para a Fila de I/O após executar por %d ciclos\n", 
                                            processo_executado.id, processo_executado.tempo_executado);
                                break;
                        case 2: // C
                                // Move o processo para a fila de I/O
                                processo_executado.tempo_retorno_io = tempo + TEMPO_C;
                                processo_executado.atual_io = 2;
                                inserir(fila_io, processo_executado);
                                printf("Processo ID %d movido para a Fila de I/O após executar por %d ciclos\n", 
                                            processo_executado.id, processo_executado.tempo_executado);
                                break;
                    }
                }   
            }

            if(fez_io == 0){
                // Se não precisar de I/O, continua a execução normal
                printf("Processo ID %d está sendo executado na Fila de Baixa Prioridade em t=%d\n", processo_executado.id, tempo);

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

        // Verifica se há processos na fila de I/O
        if (!fila_vazia(fila_io)) {
            // Remove o primeiro processo da fila de I/O
            Processo processo_executado = remover(fila_io);

            /*
            if (tempo == processo_executado.tempo_retorno_io){
                    switch (atual_io){
                    case 0:
                        inserir(fila_baixa_prioridade, processo_executado);
                        break;
                    
                    case 1: //as duas opções voltam p/ alta prioridade                
                    case 2:
                        inserir(fila_alta_prioridade, processo_executado);
                        break;                
                    }                    
                }
            */

            No * no_atual = fila_io->frente;

            do {
                if (tempo == no_atual->processo.tempo_retorno_io){
                    No * no_atual = no_atual->prox;

                    Processo processo_executado = remover(fila_io); // USAR remover do AMAI
                    switch (processo_executado.atual_io){
                    case 0:
                        inserir(fila_baixa_prioridade, processo_executado);
                        break;
                    
                    case 1: //as duas opções voltam p/ alta prioridade                
                    case 2:
                        inserir(fila_alta_prioridade, processo_executado);
                        break;                
                    }                    
                }
            } while((no_atual = no_atual->prox) != NULL);
        }
    }

    /* ESCALONADOR ACIMA */

  return 0;
}


// ARQUIVO ARQUIVO ARQUIVO ARQUIVO ARQUIVO ARQUIVO
//
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