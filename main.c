#include "gestor.h"

int main(){
    int count=0;
    int escal;
    int priori[]={2, 1, 3};//array com as prioridades dos processos em plan.txt
    do{
        printf("\n---GESTOR DE PROCESSOS---\n\n");
        printf("Escalonadores:\n1-FCFS\n2-Escalonador de longo prazo\n3-SJFS\n4-Prioridades\n\nReport:\n5-Escalonamento\n6-Sair\n--------------------------\nSelecione uma opção: ");
        scanf("%d", &escal);
        
    }while(escal < 1 || escal > 6);
    
    //caso o utilizador escolha encerrar o gestor
    if(escal==6){
        printf("\n**********Gestor terminado pelo utilizador**********\nTHE END!\n");
        exit(0);
    }


    printf("\n\n");
    int tempo=0, i=0;//tempo do while principal
    char instr;
    int indexMemoria=0;//index do array memory
    int indexExec=0;//index das instruções executadas
    PCB* procFinal;//proc depois de ter sofrido ação

    //---------------------------------------------------------//

    QUEUE *Prontos = ConstructQueue(100);//Ready queue
    QUEUE *Blocked = ConstructQueue(100);//Waiting queue
    QUEUE *Terminated = ConstructQueue(100);//Terminated queue

    //processos a correr estão representados no array num
    FILE *fp;
    fp=fopen("plan.txt", "r");
    if (fp == NULL){
        perror("Error while opening the file.\n");
        exit(1);
    }
    
    //---------------
    QUEUE *Programas = readPrograms();//fila com os programas de plan.txt
    QUEUE *ProgramasAux = readPrograms();//fila auxiliar

    int tam=countControl();//tamanho do control.txt
    char *controlArr=(char*)malloc((tam)*sizeof(char));//comandos de control.txt
    
    //---a substituir pela chamada à função
    char ci;
    int i2=0;
    FILE *fp1=fopen("control.txt", "r"); 
    while(fscanf(fp1,"%c\n", &ci)==1){
        controlArr[i2]=ci;
        i2++;
    }
    //---------------

    int iaux=0;
    while(iaux < tam){
        printf("ControlMain[%d]: %c\n", iaux, controlArr[iaux]);
        iaux++;
    }
    printf("\n");

    PCB* proc;
    
    //------------------------------------------------------------------------------------
    //inicializa a lista memMan (criar os nodos que representam a memória)
    memMan=NewNode(-1);//memória incializada a -1 -> espaço vazio
    int m=0;//aux para criar a memória
    NODO* auxMem=memMan;
    
    while(m<(memoria/unit)){//constante definida em gestor.h 
        NODO* nodo = NewNode(-1);//cria os nodos sem valor do pid (-1) por defeito e sem o seguinte atribuido
        memMan->next=nodo;
        memMan=memMan->next;
        m++;
    }
    cabeca=auxMem;
    printf("Numero de unidades de memória alocadas: %d\n", m);
    
    printf("----------------------------------------------------------Estado inicial da memMan:----------------------------------------------------------\n");
    display(auxMem);  
    printf("----------------------------------------------------------------------------------------------------------------------------------------------\n");

    //------------------------------------------------------------------------------------
    
    PCB* p; 
    //para o escalonador de prioridades e SJFS
    if(escal==4 || escal==3){//move os processos todos da fila Programas para Prontos
        
        while(isEmpty(Programas)!=1){ 
            p = Dequeue(Programas);
            p->estadoProc=READY;
            Enqueue(Prontos, p);
        }
        
    }  
    
    if(escal==4){//Para tratar as prioridades
        int num_processes = countInstr("plan.txt")+1;//devolve o nº de processos no ficheiro plan.txt
        PCB* P;
        
        int* prioridades = (int*)malloc(num_processes * sizeof(int));//criar vetor para guardar a prioridade de cada processo
        
        printf("Ordem de prioridade:\n");
        for(int aux=0;aux<num_processes;aux++){//ordem de execução: filho1, progenitor, filho2
            prioridades[aux]=priori[aux];
            
            printf("%d\t", prioridades[aux]);
        }
        printf("\n");

        for(int j=0;j<Prontos->size;j++){//atribuir as prioridades aos respetivos processos que estão em priori[] (definido nas primeiras linhas do programa)
            PCB* P = Dequeue(Prontos);
            P->priori=prioridades[j]; 
            Enqueue(Prontos, P);        
        }
    }

    if(escal==2)//inicializa a lista Blocked igual à Programas caso seja selecionado o escalonador de longo prazo
        Blocked = readPrograms();

    //------------------------------------------------------------------------------------
    int pid=0;//pid de cada processo para ser usado pela função de alocação
    while(tempo < GESTORLIMIT && i < tam){//enquanto não fizer os comandos todos do control.txt - ciclo principal
            
        //para o escalonador FCFS - passa um a um apra prontos
        if(escal==1){
            proc = Dequeue(Programas);
            proc->estadoProc=READY;
            Enqueue(Prontos, proc);
        }
        
        //*******TODO: se o tempo for superior ao tempo do plan.txt passa a pronto

        //escalonador longo prazo - passa um número aleatório de processos da fila de bloqueados para a fila ready
        int n;

        if(escal==2){
            int s=0, r = Blocked->size;
            srand(time(0));
            n = random_int(0,r);//passa n processos do estado bloqueado para ready
            printf("Número de processos passados para a fila de Prontos: %d\n", n);
            while(s < n){   
                proc = Dequeue(Blocked);
                proc->estadoProc=READY;
                Enqueue(Prontos, proc);
                s++;
            }
            //printFinalInfo(Prontos,auxMem);
            displayQueue(Prontos);
            printf("\n");
            return 0;
        }

        //Prioridades e SJFS
        QUEUE* aux=Prontos;
        if(((escal==4 || escal==3)) && (proc!=NULL)){//retira um processo sequencialmente por ordem de chegada
            proc = Dequeue(ProgramasAux);                
        }
    
        printf("\nInstr[0]: %c\n", proc->instr[1].instr);

        instr=controlArr[i];

        //----------------------------------Funcionalidade de Report-----------------------------------
        //Escalonamento
        if(escal==5){
            report(&tempo, Blocked, Prontos, Terminated);
        }

        //---------------------------------------------------------------------------------------------
        //executa os comandos de control.txt
        int startingTime;
        if('E'==instr){
            int auxg=i;//cada vez que trata um processom incrementa o i
            auxg++;//porque o i começou em 0 e está uma unidade atrás
            char instr1=controlArr[auxg];//control que vai ser tratado de momento
            
            //executar
            printf("Instr: %c\n", instr);
            printf("i: %d\n", i);
        
            startingTime=tempo;//tempo em que o processo começa a correr
            
            printf("Starting time:%d\n", startingTime);
            printf("Memory index: %d\n", indexMemoria);
            
            //FCFS
            if(escal==1){
                procFinal = FCFS(Prontos, &tempo, &indexMemoria, Terminated);//escalonar um processo pronto
            }

            //printf("tamanho prontos: %d\n", Prontos->size);
            displayQueue(Prontos);
            printf("\n");

            //SJFS
            if(escal==3){
                printf("testar entrada\n");
                procFinal = SJFS(Prontos, &indexMemoria);
                
            }

            //Prioridades
            if(escal==4){   
                printf("Entrei em prioridades\n");
                procFinal = Prioridades(Prontos, &indexMemoria);
            }
            
            printf("IndexExec: %d\n", indexExec);
            int auxindex=indexExec;

            //execução de cada processo (somas, subtrações, modificações, etc)
            printf("\n------------------------------\nEntrei na execução das instrs\n------------------------------\n\n");
            //printf("Memory[5]: %c\n", memory[5].instr);
            int limitInstr;
            //---acerto da apresentação das instruções---
            if(escal==1 || escal==4)
                limitInstr=auxindex+proc->numInstr;
            if(escal==3)
                limitInstr=auxindex+proc->numInstr-1;
            //-----------------------------------------//
            printf("limitInst: %d\n", limitInstr);
            //if(escal == 2) limitInstr = auxindex+n;//caso seja longo prazo então serão executadas n instruções
            
            while(indexExec < limitInstr){//corre todas as instruções em memory que dizem respeito a este processo
                
                procFinal = execInstr(procFinal, &indexExec, &tempo, Prontos, Blocked, Terminated, auxMem);//executa as instruções (altera a var)
                printf("var: %d\n", procFinal->var);
                printf("--------\nTempo: %d\n--------\n", tempo);
                //printf("\nI2 - tamanho do control: %d\n", i2);
                tempo++;
                //passa um processo de running para blocked
                if('I'== instr1){ 
                    //srand(time(0));
                    int random;//instante que o processo vai ser interrompido 
                    printf("Selecione o instante em que o processo é interrompido:");
                    scanf("%d", &random);   
                    while(random < tempo && random > startingTime+proc->numInstr){
                        printf("Valor de random incorreto");                   
                        scanf("%d", &random); 
                    }
                    //tempo=random;
                    if(tempo==random){//interrompe o processo a meio da sua execução
                        //printf("Random: %d\n", random);
                        startingTime=tempo;
                        procFinal->estadoProc = WAITING;  //no entanto vai ser interrompido, porque o controlArr detectou um "I", portanto o estado é alterado para "WAITING"
                        Enqueue(Blocked, proc);  //E é posto na fila Blocked
                        printf("\nProcesso Interrompido! %c\n\n", indexExec); //Em vez de uma mensagem a dizer que o processo foi executado, aparece uma mensagem a dizer que foi interrompido
                        //printf("---------------\nvar: %d\n---------------\n", procFinal->var);
                        indexExec=auxindex+proc->numInstr;
                        i++;
                        break;//para a execução e avança para o seguinte
                    }
                }
                //termina o processo (Running -> Terminated)
                if('T'== instr1){
                    startingTime=tempo;//a mesma lógica aplica-se ao T
                    count+=procFinal->numInstr;
                    printf("Processo Terminado!\n");
                    //printf("---------------\nvar: %d\n---------------\n", procFinal->var);
                    procFinal->estadoProc=TERMINATED;        
                    Enqueue(Terminated, proc); 
                    //i++;
                    indexExec++; 
                    break;//para a execução
                }

                if(indexExec == (auxindex + proc->numInstr)){
                    printf("Processo Executado!\n");
                    printf("Proc->var: %d\n", proc->var);
                    proc->estadoProc=TERMINATED;
                    Enqueue(Terminated, proc);
                }

                indexExec++;  
            }      
        }
        if('R' == instr){
            report(&tempo, Blocked, Prontos, Terminated);
        }                   
        i++; 
    }

    printFinalInfo(Prontos,auxMem);
    //int dea=deallocate_mem(1, auxMem); //--para testar a dealocação


    printFinalInfo(Terminated,auxMem);



    //------------------------------------------------------------------------------//

    //Secção da ****geração de solicitações aleatórias**** (independente de todo a parte do programa acima)
        // pegamos na memória no seu estado que vem do tratamento anterior
    
    //inicializa a lista memMan (criar os nodos que representam a memória)
    NODO* memSol;//nova memória para experimentar a alocação
    memSol=NewNode(-1);//memória incializada a -1 -> espaço vazio
    int mSol=0;//aux para criar a memória
    NODO* auxMemSol=memSol;
    while(mSol<(memoria/unit)){//constante definida em gestor.h 
        NODO* nodoSol = NewNode(-1);//cria os nodos sem valor do pid (-1) por defeito e sem o seguinte atribuido
        memSol->next=nodoSol;
        memSol=memSol->next;
        mSol++;
    }

    int N_solic=10000;
    printf("\n****************************************************************************************************************\n\t\t\t\t\tGerador de solicitações\n****************************************************************************************************************\n");
    gerarSolicitacoes(N_solic, auxMemSol);
    
    printf("----------------------------------------------------------Estado Final MemSol da auxMemSol:----------------------------------------------------------\n");
    display(auxMemSol);  
    printf("----------------------------------------------------------------------------------------------------------------------------------------------\n");
    
   return 0;
}