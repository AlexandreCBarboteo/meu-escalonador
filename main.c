#include <stdio.h>
#include <stdlib.h>
#include "fila.h"

#define DISCO 0 // Identifica Tipos de I/O. Usado nos Switch Cases
#define FITA 1
#define IMPRESSORA 2

#define TEMPO_DISCO 1 // Usado para saber quando o processo deve retornar do I/O
#define TEMPO_FITA 1
#define TEMPO_IMPRESSORA 1

#define FALSE 0
#define TRUE 1
#define NONE -1

int ler_processos(const char* nome_arquivo, Processo** processos);

int main(int argc, char *argv[]) {
    int tempo = 0;
    int processos_concluidos = 0;
    //int num_processos = 3;
    int fez_io = FALSE;

    // Criacao das filas
    Fila *fila_alta_prioridade = criar_fila();
    Fila *fila_baixa_prioridade = criar_fila();
    Fila *fila_io = criar_fila();

   /* TEM QUE SAIR
    // Criar alguns processos /////////// Tem que sumir. Precisa vir de leitura de arquivo
    Processo processo1 = {};
    Processo processo2 = {};
    Processo processo3 = {};

    // Inserir os processos na fila
    inserir(fila_alta_prioridade, processo1);
    inserir(fila_alta_prioridade, processo2);
    inserir(fila_alta_prioridade, processo3);
   */
    
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
        printf("INSTANTE %d\n", tempo);

        // Carrega os processos NOVOS
        for (int i = 0; i < num_processos; i++) {
            if (processos[i].tempo_chegada == tempo /*&& processos[i].tempo_restante > 0*/) {
                inserir(fila_alta_prioridade, processos[i]);
                printf("Processo %d chegou e foi adicionado à fila de alta prioridade em t=%d\n", processos[i].id, tempo);
            }
        }
            
        if (!fila_vazia(fila_alta_prioridade)) {
            // Remove o primeiro processo da fila para simular a execução
            Processo processo_executado = remover(fila_alta_prioridade);

            fez_io=FALSE;
            int teste_alta=FALSE; // teste
            for(int i=0;i<3;i++){
                if (teste_alta==TRUE){
                    //printf("Saí do for\n"); // teste
                    break;
                }
                if(processo_executado.tempo_executado == processo_executado.tempo_inicio_io[i]){
                    //printf("TEMPO EXECUTADO = %d", processo_executado.tempo_executado);
                    fez_io =TRUE;
                    switch(i) { //direcionar tipo de IO
                        case DISCO: // A
                                // Move o processo para a fila de I/O
                                processo_executado.tempo_retorno_io = tempo + TEMPO_DISCO;
                                processo_executado.atual_io = 0; 
                                processo_executado.tempo_inicio_io[i] = NONE; // Isso impede loop infinito
                                inserir(fila_io, processo_executado);
                                printf("Processo ID %d movido da ALTA  para a Fila de I/O após executar por %d ciclos\n", 
                                            processo_executado.id, processo_executado.tempo_executado);
                                // teste:
                                //printf("Valor em .tempo_retorno_io: %d\n",processo_executado.tempo_retorno_io); 
                                teste_alta=TRUE;
                                break;                

                        case FITA: // B
                                // Move o processo para a fila de I/O
                                processo_executado.tempo_retorno_io = tempo + TEMPO_FITA;
                                processo_executado.atual_io = 1;
                                processo_executado.tempo_inicio_io[i] = NONE; // Isso impede loop infinito
                                inserir(fila_io, processo_executado);
                                printf("Processo ID %d movido da ALTA  para a Fila de I/O após executar por %d ciclos\n", 
                                            processo_executado.id, processo_executado.tempo_executado);
                               // printf("Valor em .tempo_retorno_io: %d\n",processo_executado.tempo_retorno_io);
                                teste_alta=TRUE;
                                break;
                        case IMPRESSORA: // C
                                // Move o processo para a fila de I/O
                                processo_executado.tempo_retorno_io = tempo + TEMPO_IMPRESSORA;
                                processo_executado.atual_io = 2;
                                processo_executado.tempo_inicio_io[i] = NONE; // Isso impede loop infinito
                                inserir(fila_io, processo_executado);
                                printf("Processo ID %d movido da ALTA  para a Fila de I/O após executar por %d ciclos\n", 
                                            processo_executado.id, processo_executado.tempo_executado);
                                //printf("Valor em .tempo_retorno_io: %d\n",processo_executado.tempo_retorno_io);
                                teste_alta=TRUE;
                                break;
                    }
                }   
            }
            
            
            if(fez_io == FALSE){
                // Se não precisar de I/O, continua a execução normal
                printf("Processo ID %d está sendo executado na Fila de Alta Prioridade em t=%d\n", processo_executado.id, tempo);

                // Incrementa o tempo executado do processo
                processo_executado.tempo_executado++;

                // Verifica se o processo foi concluído
                if (processo_executado.tempo_executado >= processo_executado.tempo_servico) {
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

             
            fez_io=FALSE;
            int teste_baixa=FALSE; // teste
            for(int i=0;i<3;i++){ // talvez tirar?
                if (teste_baixa==TRUE){
                    break;
                }
                if(processo_executado.tempo_executado == processo_executado.tempo_inicio_io[i]){
                   // printf("TEMPO EXECUTADO = %d\n", processo_executado.tempo_executado);
                    fez_io =TRUE;
                    switch(i) { //direcionar tipo de IO
                        case DISCO: // A
                                // Move o processo para a fila de I/O
                                processo_executado.tempo_retorno_io = tempo + TEMPO_DISCO;
                                processo_executado.atual_io = 0;
                                processo_executado.tempo_inicio_io[i] = NONE; // Isso impede loop infinito
                                inserir(fila_io, processo_executado);
                                printf("Processo ID %d movido da BAIXA para a Fila de I/O após executar por %d ciclos\n", 
                                            processo_executado.id, processo_executado.tempo_executado);
                                // teste:
                              //  printf("Valor em .tempo_retorno_io: %d\n",processo_executado.tempo_retorno_io);
                                teste_baixa=TRUE;
                                break;                

                        case FITA: // B
                                // Move o processo para a fila de I/O
                                processo_executado.tempo_retorno_io = tempo + TEMPO_FITA;
                                processo_executado.atual_io = 1;
                                processo_executado.tempo_inicio_io[i] = NONE; // Isso impede loop infinito
                                inserir(fila_io, processo_executado);
                                printf("Processo ID %d movido da BAIXA para a Fila de I/O após executar por %d ciclos\n", 
                                            processo_executado.id, processo_executado.tempo_executado);
                                teste_baixa=TRUE;
                                break;
                        case IMPRESSORA: // C
                                // Move o processo para a fila de I/O
                                processo_executado.tempo_retorno_io = tempo + TEMPO_IMPRESSORA;
                                processo_executado.atual_io = 2;
                                processo_executado.tempo_inicio_io[i] = NONE; // Isso impede loop infinito
                                inserir(fila_io, processo_executado);
                                printf("Processo ID %d movido da BAIXA para a Fila de I/O após executar por %d ciclos\n", 
                                            processo_executado.id, processo_executado.tempo_executado);
                                teste_baixa=TRUE;
                                break;
                    }
                }   
            }
            

            if(fez_io == FALSE){
                // Se não precisar de I/O, continua a execução normal
                printf("Processo ID %d está sendo executado na Fila de Baixa Prioridade em t=%d\n", processo_executado.id, tempo);

                // Incrementa o tempo executado do processo
                processo_executado.tempo_executado++;

                // Verifica se o processo foi concluído
                if (processo_executado.tempo_executado >= processo_executado.tempo_servico) {
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
            int tamanho_io = tamanho_fila(fila_io);
            int n = 0;
            while(n++ < tamanho_io) {
                Processo processo = remover(fila_io);
                if (tempo == processo.tempo_retorno_io) {
                   // printf("tempo igual ao retorno do i/o\n");
                    if (processo.atual_io == 0) {
                        inserir(fila_baixa_prioridade, processo);
                    } else {
                        inserir(fila_alta_prioridade, processo);
                    }
                    printf("Processo ID %d voltou do I/O em t=%d\n", processo.id, tempo);
                } else {
                    inserir(fila_io, processo);
                }
            }
            
        }
        tempo++;
    }
    printf("Escalonador Encerrou depois de %d ut\n", --tempo);

    /* ESCALONADOR ACIMA */

  return 0;
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
}
