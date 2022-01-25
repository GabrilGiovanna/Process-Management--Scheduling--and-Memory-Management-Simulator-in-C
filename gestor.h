#include <stdio.h>
#include <stdlib.h>  
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include <math.h>
#include <limits.h>

#define GESTORLIMIT 1000 //tempo máximo do gestor
#define TIMELIMIT 100 //tempo máximo de um processo (e isntruções num processo
//isto porque é executada um instrução por unidade de tempo)

//----------------------------------------------------------//

typedef struct instruction{
    char instr;     //instrução
    int n;          //valor para a variável
    char nome[15];  //nome do processo - nome em caso de ser um exec ou NULL caso contrário  
}INSTRUCTION;

INSTRUCTION memory[1000];

//declarar aqui as funções usadas
//----------------------------------------------------------//
//Referência: https://gist.github.com/ArnonEilat/4471278
//FILA
#define TRUE 1
#define FALSE 0

//estados do processo
#define WAITING 2
#define RUNNING 1
#define TERMINATED 3
#define READY 0

typedef struct Node_t{
    char nome[50];     //nome do processo
    int var;           //valor da variável
    int pid;           //identificador do processo
    int ppid;          //identificador do pai do processo
    int start;         //momentos que se deve inserir o programa na ready queue
    int priori;        //prioridade de execucao 1 > 2 > 3
    int tempVida;      //tempo que tem de ser executado
    int PC;            //próxima instrução a executar
    int numMem;        //número de unidades de memória
    int valStInstrs;   //valor da primeira instrução
    int numInstr;      //número de instruções
    INSTRUCTION *instr;//array as instruções do processo
    int estadoProc;    //estado de execução do processo
    int openFiles;     //º de ficheiros abertos
    struct Node_t *prev;
} PCB;//controlar cada processo          

/* the HEAD of the Queue, hold the amount of node's that are in the queue*/

//fila para guardar processos
typedef struct Queue {
    PCB *head;
    PCB *tail;
    int size;
    int limit;
} QUEUE;

//declarar aqui as funções usadas

QUEUE *ConstructQueue(int limit);
void DestructQueue(QUEUE *queue);
int Enqueue(QUEUE *pQueue, PCB *item);
PCB *Dequeue(QUEUE *pQueue);
int isEmpty(QUEUE* pQueue);

//----------------------------------------------------------//
//Estrutura de dados para alocação de memória
typedef struct Nodo { 
    int pid; 
    struct Nodo* next; 
}NODO; 

//variáveis globais e constantes de memória----
#define memoria 256//tamanho da memória
#define unit 8//tamanho de cada unidade de memória

NODO* memMan;//lista ligada que serve de suporte para a simulação de gestão de memória

//--------------------------------------

//Memory Management 

//funções comuns a todos os algoritmos
int deallocate_mem (int process_id, NODO* auxMem);
int fragment_count (); //*******

//linkedList
NODO* NewNode(int pid);
NODO* LastNode(NODO *head);
int countNodes();
void display(NODO *memMam);

//first fit
int alocate_units_mem_first(int process_id, int num_units, NODO* auxMem);
NODO* copiar(NODO *ola);//função auxiliar não usada atualmente

//next fit
int alocate_units_mem_next(int process_id, int num_units, NODO* auxMem);

NODO* cabeca; //Variável global para usar no next fit, de forma a alterar a cabeça do auxMem
//best fit

//para definir o nodo mínimo
typedef struct best {
    int size;
    NODO* head;
}BEST;

int alocate_units_mem_best(int process_id, int num_units, NODO* auxMem);

NODO* alocar(int process_id,NODO* auxMem,int count);

//worst fit
int alocate_units_mem_worst(int process_id, int num_units, NODO* auxMem);

//----------------------------------------------------------//
//gestorFun
int countInstr(char* nome);
QUEUE *readPrograms();
int countControl();
char *lerControl(int tam, char *cmd);//implementar uso da função -- erro de apontadores
INSTRUCTION* lerIntr(char *nome);

PCB* execInstr(PCB* proc, int *i,int *t,QUEUE* Prontos,QUEUE* Blocked,QUEUE* Terminated, NODO* auxmem);
void report(int *tempo, QUEUE* Bloqueados, QUEUE* Prontos, QUEUE* Terminados);
void displayQueue(QUEUE* Prontos);
//----------------------------------------------------------//
//Escalonamento de longo prazo
int random_int(int min, int max);
//----------------------------------------------------------//
//Escalonamento SJF
PCB* SJFS(QUEUE* Prontos, int *index);
//PCB* addRunningSJF(QUEUE* Prontos, int *i);
//----------------------------------------------------------//
//Escalonamento FCFS
PCB* FCFS(QUEUE* Prontos, int *time, int *index, QUEUE* Terminated);
PCB* addRunning(PCB* proc, int *i);
//----------------------------------------------------------//
//Escalonamento Prioridades
PCB* Prioridades(QUEUE* Prontos, int *index);
//---------------------------------------------------------//
void printFinalInfo(QUEUE* Prontos, NODO* auxMem);

//---------------------------------------------------------//
//Componenente de geração de solicitações
void gerarSolicitacoes(int numSolic, NODO* auxMemSol);

//---------------------------------------------------------//

//Report Gestão memória
void memoryReport(NODO* auxMemSol);
//---fragmentos médios---
int somaVetor(int *v, int tam);
int mediaFragmentosExternos(NODO* auxMemSol);
//---tempo de alocação médio---
int tempoAlocacaoMedio(int nodos, int num_alocs);
//---

int contaproc(NODO *auxMemSol);

int *nmrproc(NODO *auxMemSol,int count);