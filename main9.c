#include <stdio.h>
#include <stdlib.h>
#include "fila.h"

#define DISCO 0 // Identifica Tipos de I/O. Usado nos Switch Cases e ternários
#define FITA 1
#define IMPRESSORA 2

#define TEMPO_DISCO 1 // Usado para saber quando o processo deve retornar do I/O
#define TEMPO_FITA 2
#define TEMPO_IMPRESSORA 1

#define FALSE 0 
#define TRUE 1
#define NONE -1

#define ALTA 0    // ADICIONAR COMENTARIO DEPOIS
#define BAIXA 1

int ler_processos(const char* nome_arquivo, Processo** processos);
void carregar_processos_iniciais(Fila* fila_alta_prioridade, Processo* processos, int num_processos, int tempo);
int verificar_processos_novos(Processo* processos, int num_processos, int tempo);
void carregar_processos_novos(Fila* fila_alta_prioridade, Processo* processos, int num_processos, int tempo);
void executar_processo_alta(Fila* fila_alta_prioridade, Fila* fila_baixa_prioridade, Fila* fila_io_alta, int tempo, int* processos_concluidos,
                        /**/ Processo* processos, int num_processos, Fila* fila_io_baixa);
void executar_processo_baixa(Fila* fila_alta_prioridade, Fila* fila_baixa_prioridade, Fila* fila_io_alta, int tempo, int* processos_concluidos,
                        /**/ Processo* processos, int num_processos, Fila* fila_io_baixa);
void tratar_io_alta(Fila* fila_io_alta, Fila* fila_alta_prioridade, int tempo);
int verificar_necessidade_io_alta(Fila* fila_io_alta, Fila* fila_alta_prioridade, int tempo);
void tratar_io_baixa(Fila* fila_io_baixa, Fila* fila_baixa_prioridade, int tempo);

int main(int argc, char *argv[]) {
    int tempo = 0;
    int processos_concluidos = 0;
    int fez_io = FALSE;
    int num_processos;

    // Criacao das filas
    Fila *fila_alta_prioridade = criar_fila();
    Fila *fila_baixa_prioridade = criar_fila();
    Fila *fila_io_alta = criar_fila();
    Fila *fila_io_baixa = criar_fila();
    
    // Carregar processos de um arquivo txt
    Processo* processos = NULL;
    num_processos = ler_processos("processos.txt", &processos);
    
    // Verifica se pelo menos um processo foi carregado
    if (num_processos == 0) {
        printf("Nenhum processo carregado. Encerrando...\n");
        return 1;
    }
    
    /* ESCALONADOR */
    while(/*processos_concluidos<num_processos*/ tempo<30)
    {
        printf("INSTANTE %d:\n", tempo);
        printf("processos_concluidos = %d\n", processos_concluidos);
        printf("num_processos = %d\n", num_processos);
        if (tempo==0) {
            printf("Carreguei processos iniciais\n");
            carregar_processos_iniciais(fila_alta_prioridade, processos, num_processos, tempo);
        }
        // Lida com a fila de alta prioridade, se disponível
        if (!fila_vazia(fila_alta_prioridade)) {
            printf("entrei execucao alta\n");
            executar_processo_alta(fila_alta_prioridade, fila_baixa_prioridade, fila_io_alta, tempo, &processos_concluidos,
                        /**/ processos, num_processos, fila_io_baixa);
        }
        /*
        else{
            printf("Não entrei na ALTA\n");
        }
        */ 
        // Lida com a fila de baixa prioridade, se disponível
        else if (!fila_vazia(fila_baixa_prioridade)) {
            printf("entrei execucao baixa\n");
            executar_processo_baixa(fila_alta_prioridade, fila_baixa_prioridade, fila_io_alta, tempo, &processos_concluidos,
                        /**/ processos, num_processos, fila_io_baixa);
        }
        /*
        else{
            printf("Não entrei na BAIXA\n");
        }
        */
        
        // Tratar processos na fila de I/O
        //tratar_io(fila_io, fila_alta_prioridade, fila_baixa_prioridade, tempo);
        //printf("\n");
        tempo++; // O TEMPO PRECISA SER INCREMENTADO AQUI? ****************
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

void carregar_processos_novos(Fila* fila_alta_prioridade, Processo* processos, int num_processos, int tempo) {
    for (int i = 0; i < num_processos; i++) {
        if (processos[i].tempo_chegada == tempo ) {
            furar_fila(fila_alta_prioridade, processos[i]);
            printf("Processo %d chegou e foi adicionado à fila de alta prioridade\n", processos[i].id);
        }
    }
}

int verificar_processos_novos(Processo* processos, int num_processos, int tempo){
    for (int i = 0; i < num_processos; i++) {
        if (processos[i].tempo_chegada == tempo ) {
            return TRUE;
        }
    }
    return FALSE;
}

void executar_processo_alta(Fila* fila_alta_prioridade, Fila* fila_baixa_prioridade, Fila* fila_io_alta, int tempo, int* processos_concluidos,
                        /**/ Processo* processos, int num_processos, Fila* fila_io_baixa) {
    
    int fez_io = FALSE;
    Processo processo_executado = remover(fila_alta_prioridade);
    for (int i = 0; i < 3; i++) {
        if (processo_executado.tempo_executado == processo_executado.tempo_inicio_io[i]) {
            switch (i) {
            case DISCO:
                processo_executado.tempo_retorno_io = tempo + TEMPO_DISCO;
                processo_executado.fila_origem = DISCO;
                break;
            case FITA:
                processo_executado.tempo_retorno_io = tempo + TEMPO_FITA;
                processo_executado.fila_origem = FITA;
                break;
            case IMPRESSORA:
                processo_executado.tempo_retorno_io = tempo + TEMPO_IMPRESSORA;
                processo_executado.fila_origem = IMPRESSORA;
                break;
            }
            processo_executado.atual_io = i;
            if (processo_executado.tempo_inicio_io[i]==DISCO){
                inserir(fila_io_baixa, processo_executado);
            } else{
                inserir(fila_io_alta, processo_executado);
            }
            processo_executado.tempo_inicio_io[i] = NONE; // Prevenir loops infinitos
            printf("Processo ID %d movido da fila de ALTA para I/O (tipo %s) após executar por %d ciclos.\n",
                processo_executado.id,
                (processo_executado.atual_io == ALTA ? "DISCO" : (i == BAIXA ? "FITA" : "IMPRESSORA")),
                processo_executado.tempo_executado);
            fez_io = TRUE;
            break;
        }
    }

    if (!fez_io) {
        // Verifica se há I/O, manda o processo autal para Baixa e trata I/O de alta - SE precisar voltar, já passa a frente
        /*
        if (verificar_necessidade_io_alta(fila_io_alta,fila_alta_prioridade, tempo)==TRUE && tempo!=0){
            printf("Processo %d indo à fila Baixa para dar lugar a outro", processo_executado.id);
            inserir(fila_baixa_prioridade, processo_executado);
            tratar_io_alta(fila_io_alta, fila_alta_prioridade, tempo);
            processo_executado = remover(fila_alta_prioridade);
            printf("Processo %d furou fila por voltar do I/O para Alta",processo_executado.id);
        }
        */

        //Coloca novos processos depois de tempo=0
        // Processos novos ficam na frente do I/O
        if (verificar_processos_novos(processos, num_processos, tempo) && tempo!=0){
            inserir(fila_baixa_prioridade, processo_executado);
            carregar_processos_novos(fila_alta_prioridade, processos, num_processos, tempo);
            processo_executado =  remover(fila_alta_prioridade); // Para executar o NOVO
            printf("Oi! NOVO Processo ID %d aqui\n", processo_executado.id);
        }
        /*
        if (verificar_processos_iniciais(processos, num_processos, tempo) && tempo!=0){
            inserir(fila_baixa_prioridade, processo_executado);
            processo_executado =  remover(fila_alta_prioridade); // Para executar o NOVO
            printf("Oi! NOVO Processo ID %d aqui\n", processo_executado.id);
        }
        */
        //tratar_io_baixa(fila_io_baixa, fila_baixa_prioridade, tempo);
        processo_executado.tempo_executado++;
        printf("TEMPO EXECUTADO do Processo %d = %d\n", processo_executado.id, processo_executado.tempo_executado);
        printf("Processo ID %d está sendo executado na Fila de ALTA prioridade\n", processo_executado.id);
        
        if (processo_executado.tempo_executado >= processo_executado.tempo_servico) {
            (*processos_concluidos)++;
            
            printf("Processo ID %d concluído após %d ciclos de execução.\n",
                processo_executado.id,
                processo_executado.tempo_executado);
        } else if (fila_vazia(fila_alta_prioridade)) { // retorna true se estiver vazia
            // Mantém o processo na fila de Alta Prioridade se ainda não foi concluído
            inserir(fila_alta_prioridade, processo_executado);
        } else {
            // Insere o processo na fila de Baixa Prioridade se ainda não foi concluído
            inserir(fila_baixa_prioridade, processo_executado);
        }
    } 
}

void executar_processo_baixa(Fila* fila_alta_prioridade, Fila* fila_baixa_prioridade, Fila* fila_io_alta, int tempo, int* processos_concluidos,
                        /**/ Processo* processos, int num_processoss, Fila* fila_io_baixa) {
    
    int fez_io = FALSE;
    Processo processo_executado = remover(fila_baixa_prioridade);
    for (int i = 0; i < 3; i++) {
        if (processo_executado.tempo_executado == processo_executado.tempo_inicio_io[i]) {
            switch (i) {
            case DISCO:
                processo_executado.tempo_retorno_io = tempo + TEMPO_DISCO;
                processo_executado.fila_origem = DISCO;
                break;
            case FITA:
                processo_executado.tempo_retorno_io = tempo + TEMPO_FITA;
                processo_executado.fila_origem = FITA;
                break;
            case IMPRESSORA:
                processo_executado.tempo_retorno_io = tempo + TEMPO_IMPRESSORA;
                processo_executado.fila_origem = IMPRESSORA;
                break;
            }
            processo_executado.atual_io = i;
            if (processo_executado.tempo_inicio_io[i]==DISCO){
                inserir(fila_io_baixa, processo_executado);
            } else{
                inserir(fila_io_alta, processo_executado);
            }
            processo_executado.tempo_inicio_io[i] = NONE; // Prevenir loops infinitos
            printf("Processo ID %d movido da fila de ALTA para I/O (tipo %s) após executar por %d ciclos.\n",
                processo_executado.id,
                (processo_executado.atual_io == ALTA ? "DISCO" : (i == BAIXA ? "FITA" : "IMPRESSORA")),
                processo_executado.tempo_executado);
            fez_io = TRUE;
            break;
            /*
            processo_executado.atual_io = i;
            processo_executado.tempo_inicio_io[i] = NONE; // Prevenir loops infinitos
            inserir(fila_io, processo_executado);
            printf("Processo ID %d movido da fila de Baixa para I/O (tipo %s) após executar por %d ciclos.\n",
                processo_executado.id,
                (i == ALTA ? "DISCO" : (i == BAIXA ? "FITA" : "IMPRESSORA")),
                processo_executado.tempo_executado);
            fez_io = TRUE;
            break;
            */
        }
    }

    if (!fez_io) {
        printf("Processo %d FUROU FILA de BAIXA\n", processo_executado.id);
        furar_fila(fila_baixa_prioridade, processo_executado); 
        // usar pular_fila para processo que volta do I/O p/ baixa prioridade passar a frente dos outros
        tratar_io_baixa(fila_io_baixa, fila_baixa_prioridade, tempo);
        tratar_io_alta(fila_io_alta, fila_alta_prioridade, tempo);
        processo_executado = remover(fila_baixa_prioridade);
        //printf("Processo %d está na FILA de BAIXA\n", processo_executado.id);
        processo_executado.tempo_executado++;
        //printf("TEMPO EXECUTADO do Processo %d = %d\n", processo_executado.id, processo_executado.tempo_executado);
        printf("Processo ID %d está sendo executado na Fila de Baixa prioridade\n", processo_executado.id);
        if (processo_executado.tempo_executado >= processo_executado.tempo_servico) {
            (*processos_concluidos)++;
            printf("Processo ID %d concluído após %d ciclos de execução.\n",
                processo_executado.id,
                processo_executado.tempo_executado);
        } else {
            printf("Não terminei. Tempo de serviço = %d", processo_executado.tempo_servico);
            inserir(fila_baixa_prioridade, processo_executado);
        }
    } 
}


void tratar_io_alta(Fila* fila_io_alta, Fila* fila_alta_prioridade, int tempo) {
    int tamanho_io = tamanho_fila(fila_io_alta);
    int n = 0;
    while(n++ < tamanho_io) {
        Processo processo_io = remover(fila_io_alta);
        if (tempo == processo_io.tempo_retorno_io) {
            furar_fila(fila_alta_prioridade, processo_io);
            printf("Processo ID %d voltou do I/O (tipo %s)\n", 
                    processo_io.id, 
                    (processo_io.atual_io == FITA ? "FITA" : "IMPRESSORA"));
        } else {
            inserir(fila_io_alta, processo_io);
        }
    }
}

int verificar_necessidade_io_alta(Fila* fila_io_alta, Fila* fila_alta_prioridade, int tempo) {
    int tamanho_io = tamanho_fila(fila_io_alta);
    int n = 0;
    while(n++ < tamanho_io) {
        Processo processo_io = remover(fila_io_alta);
        if (tempo == processo_io.tempo_retorno_io) {
            return TRUE;
        }
    }
    return FALSE;
}


void tratar_io_baixa(Fila* fila_io_baixa, Fila* fila_baixa_prioridade, int tempo) {
    int tamanho_io = tamanho_fila(fila_io_baixa);
    int n = 0;
    while(n++ < tamanho_io) {
        Processo processo_io = remover(fila_io_baixa);
        if (tempo == processo_io.tempo_retorno_io) {
            inserir(fila_baixa_prioridade, processo_io);
            printf("Processo ID %d voltou do I/O (tipo DISCO)\n", processo_io.id);
        } else {
            inserir(fila_io_baixa, processo_io);
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