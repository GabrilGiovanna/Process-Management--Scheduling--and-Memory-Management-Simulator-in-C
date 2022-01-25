#include "gestor.h"
//-----------------------------------------------------------------
//Memory management

//componente de memória

//função auxiliar da deallocate
/*NODO* findFirstPid(NODO* head, int pid){
    while(head->pid != pid){
        head=head->next;
    }
    return head;
}*/

//dealocar espaço de memória referente a um processo
int deallocate_mem (int process_id, NODO* auxMem){
    while(auxMem!=NULL){
        if(auxMem->pid==process_id)
            auxMem->pid=-1;
        auxMem=auxMem->next;
    }
    if(auxMem==NULL)
        return 1;//sucesso
    return -1;//erro
}

//Algoritmos de reserva de memória

//first fit
//alocar num_units (quantidade) de NODOS para o processo
int alocate_units_mem_first(int process_id, int num_units, NODO* auxMem){
    //int i=0;//index das unidades de memória
    int count=0;
    int x=num_units;  //variavel auxiliar
    int nodos=0;
    //auxMem->next->next->pid=9;
    //auxMem->next->next->next->next->next->next->next->next->next->pid=9;
    NODO* aux=auxMem;  //nodo auxiliar para percorrer a lista
    //NODO* aux2=auxMem; //para depois retornar o auxMem a esta posição para fazer de novo o algoritmo
    /*printf("----------------------------------------------------------First Fit - Estado atual da memMan:----------------------------------------------------\n");
    display(auxMem);
    printf("-------------------------------------------------------------------------------------------------------------------------------------\n");
    */
    while(aux!=NULL){   
        if(aux->pid==-1){  //se o nodo está vazio
            while(aux->next!=NULL){  //enquanto houver nodos vazios
                if(aux->pid!=-1) break;
                aux=aux->next;  //percorrer os nodos
                nodos++; //conta os nodos percorridos
                count++;  //contar quantos nodos vazios existe
            }
        }
        //printf("Count:%d\n",count);
        //printf("Nodos:%d\n",nodos);
        if(count>=num_units){  //Verifica se os nodos vazios são suficientes 
            nodos=nodos-count;//volta para a primeira posição do bloco livre suficiente para alocar o processo
            if(nodos!=0){  //se o inicio do nodo não é logo o primeiro 
                for(int j=0;j<nodos;j++){  //percorrer a outra lista (principal) até chegar ao espaço vazio
                    auxMem=auxMem->next;  
                }
            }
            for(int k=0;k<x;k++){  // vai preencher x nodos
                auxMem->pid=process_id;
                auxMem=auxMem->next;
            }
            return nodos; //se isto tudo acontece, já não é necessário percorrer mais a lista, portanto sai do while principal
        }

        if(aux!=NULL){//continua à procura de espaço livre
            aux=aux->next;
            nodos++;
        }
        count=0;
        memMan=auxMem;
    }
    
    return -1;
}

//-------------------------------------------------------------------

//--best fit --

BEST* NewNodeBest(int size)
{
    BEST* new = (BEST*)malloc(sizeof(BEST));

    new->size = size;
    new->head = NULL;	
	
    return(new);
}

/*NODO* alocar(int process_id,NODO* auxMem,int count){

for(int k=0;k<count;k++){ 
                auxMem->pid=process_id;
                auxMem=auxMem->next;
            }

return auxMem;
}*/ 

//procura a bloco mais pequena suficiente para o processo
int alocate_units_mem_best(int process_id, int num_units, NODO* auxMem){

    //auxMem->pid=9;
    //printf("P.I.D:%d",auxMem->pid);
    //auxMem->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->next->pid=9;
   
    /*printf("----------------------------------------------------------BestFit:----------------------------------------------------\n");
    display(auxMem);
    printf("-------------------------------------------------------------------------------------------------------------------------------------\n");
    */
    int q=0;
    int count=0;
    int c2=0;
    int nodos=0;
    NODO* aux=auxMem;
    //NODO* aux2=auxMem;
    int *sub = (int*)malloc(sizeof(int));
    int *x = (int*)malloc(sizeof(int));
    int z;
    int v=0;
    NODO** y=(NODO**)malloc(sizeof(NODO*));//guarda apontadores para o início do espaço livre
    while(aux!=NULL){
        if(aux->pid==-1){  //se o nodo está vazio
            c2++;
            y=(NODO**)realloc(y,c2*sizeof(NODO*));
            y[c2-1]=aux;
            //printf("Y:%d\n",y[c2]->pid);
            while(aux->next!=NULL){  //enquanto houver nodos vazios
                if(aux->pid!=-1) break;
                    aux=aux->next;  //percorrer os nodos
                    count++;  //contar quantos nodos vazios existe
                    nodos++;
            }
            //printf("Count:%d\n",count);
            x=(int*)realloc(x,c2*sizeof(int));
            x[c2-1]=count;  
            }

        count=0;
        if(aux!=NULL){
            aux=aux->next;
        }
    }
    
    sub=(int*)realloc(sub, c2*sizeof(int));

    for(int b=0;b<c2;b++){
        if(x[b]>=num_units) q=1;  //é só uma verificação(há pelo menos 1)
    }

    if(q!=1) return -1;  //se não há nenhum espaço livre disponível para o processo

    for(int i=0;i<c2;i++){ 
        sub[i]=x[i]-num_units;
    }

    //printf("sub:%d\n",sub[0]);
    //printf("sub1:%d\n",sub[1]);

    //falta verificar se o espaço livre é maior que num_units
    z=0;

    for(int j=0;j<c2;j++){
        if(sub[j]==0){
            auxMem=y[j];  // o auxMem vai apontar para o Nodo que estava na posição especifica
            v=1;
            //aloca
            for(int h=0;h<num_units;h++){ 
                if(auxMem!=NULL){
                    auxMem->pid=process_id;
                    auxMem=auxMem->next;
                }
            } 
            break;  
        }
        if(sub[j+1]<sub[j]&&(sub[j+1]>=0))
            z=j+1;
        else z=j;
    }

    if(v==0){
        if(z<(memoria/unit))
        z=z-1;
        //printf("%d\n",z);
        auxMem=y[z];

        //aloca
        for(int k=0;k<num_units;k++){ 
            if(auxMem!=NULL){
                auxMem->pid=process_id;
                auxMem=auxMem->next;
            }
        }

    }

    /*auxMem=aux2; //só para o display funcionar
    printf("----------------------------------------------------------BestFitFinal:----------------------------------------------------\n");
    display(auxMem);
    printf("-------------------------------------------------------------------------------------------------------------------------------------\n");
    */
    return nodos;
}

//next fit

int alocate_units_mem_next(int process_id, int num_units, NODO* auxMem){
    //int i=0;//index das unidades de memória
    int count=0;
    int x=num_units;  //variavel auxiliar
    int nodos=0;
    //auxMem->next->next->next->next->next->next->next->next->next->pid=9;
    auxMem=cabeca;
    
    NODO* aux=auxMem;  //nodo auxiliar para percorrer a lista

    /*printf("----------------------------------------------------------Estado atual da memMan NEXT FIT:----------------------------------------------------\n");
    display(auxMem);
    printf("-------------------------------------------------------------------------------------------------------------------------------------\n"); 
    */

    while(aux!=NULL){   
        if(aux->pid==-1){  //se o nodo está vazio
            while(aux->next!=NULL){  //enquanto houver nodos vazios
                if(aux->pid!=-1) break;
                    aux=aux->next;  //percorrer os nodos
                    nodos++;
                    count++;  //contar quantos nodos vazios existe
            }
        }
        //printf("Count:%d\n",count);
        //printf("Nodos:%d\n",nodos);
        if(count>=num_units){  //Verifica se os nodos vazios são suficientes 
            nodos=nodos-count;
            if(nodos!=0){  //se o inicio do nodo não é logo o primeiro 
                for(int j=0;j<nodos;j++){  //percorrer a outra lista até chegar ao espaço vazio
                    auxMem=auxMem->next;  
                }
            }
            for(int k=0;k<x;k++){  // vai preencher x nodos
                auxMem->pid=process_id;
                auxMem=auxMem->next;
            }
            return nodos; //se isto tudo acontece, já não é necessário percorrer mais a lista, portanto sai do while principal
        }
        if(aux!=NULL){
            aux=aux->next;
            nodos++;
        }
        count=0;
        memMan=auxMem;
    }
    //!!!!COMO A CABEÇA é alterada, o display só mostra a partir da nova head, logo só vai mostrar os espaços restantes(neste caso em particular)!!!!!
    
    cabeca=auxMem;
    return -1;
}

//worst fit
int alocate_units_mem_worst(int process_id, int num_units, NODO* auxMem){
    //auxMem->pid=9;
    //printf("P.I.D:%d",auxMem->pid);
    //auxMem->next->next->next->next->next->next->next->next->next->next->next->next->next->next->pid=9;
 /*  
    printf("----------------------------------------------------------WorstFit:----------------------------------------------------\n");
    display(auxMem);
    printf("-------------------------------------------------------------------------------------------------------------------------------------\n");
*/

    int q=0;
    int count=0;
    int c2=0;
    int nodos=0;
    NODO* aux=auxMem;
    //NODO* aux2=auxMem;
    int *sub = (int*)malloc(sizeof(int));
    int *x = (int*)malloc(sizeof(int));
    int z;
    //int v=0;
    NODO** y=(NODO**)malloc(sizeof(NODO*));
    while(aux!=NULL){
        if(aux->pid==-1){  //se o nodo está vazio
            c2++;
            y=(NODO**)realloc(y,c2*sizeof(NODO*));
            y[c2-1]=aux;
            //printf("Y:%d\n",y[c2]->pid);
            while(aux->next!=NULL){  //enquanto houver nodos vazios
                if(aux->pid!=-1) break;
                aux=aux->next;  //percorrer os nodos
                count++;  //contar quantos nodos vazios existe
                nodos++;
            }
            //printf("Count:%d\n",count);
            x=(int*)realloc(x,c2*sizeof(int));
            x[c2-1]=count;                
        }

        count=0;
        if(aux!=NULL){
            aux=aux->next;
        }
    }

    sub=(int*)realloc(sub, c2*sizeof(int));

    for(int b=0;b<c2;b++){
        if(x[b]>=num_units) q=1;  //é só uma verificação(há pelo menos 1)
    }

    if(q!=1) return -1;  //se não há nenhum espaço livre disponível para o processo

    for(int i=0;i<c2;i++){ 
        sub[i]=x[i]-num_units;
    }
    //printf("sub:%d\n",sub[0]);
    //printf("sub1:%d\n",sub[1]);

    //falta verificar se o espaço livre é maior que num_units
    z=0;
    int maior=INT_MIN;
    for(int j=0;j<c2;j++){
        if((sub[j]>maior)&&(sub[j]>=0)){
            z=j;
            maior=sub[j];
        }
        //printf("Z:%d\n",z);
    }


    //printf("%d\n",z);
    auxMem=y[z];


    //aloca
    for(int k=0;k<num_units;k++){ 
        if(auxMem!=NULL){
        auxMem->pid=process_id;
        auxMem=auxMem->next;
        }
    }



    /*auxMem=aux2; //só para o display funcionar
    printf("----------------------------------------------------------WorstFitFinal:----------------------------------------------------\n");
    display(auxMem);
    printf("-------------------------------------------------------------------------------------------------------------------------------------\n");
    */
    return nodos;
}


//-----------------------------------------------------------------//
int mediaFragmentosExternos(NODO* auxMemSol){
    int *x = (int*)malloc(sizeof(int));
    int c2=0;//tamanho do vetor
    int count=0, nodos=0;
    NODO* aux=auxMemSol;
    while(aux!=NULL){
        if(aux->pid==-1){  //se o nodo está vazio
            c2++;           
            while(aux->next!=NULL){  //enquanto houver nodos vazios
                if(aux->pid!=-1) break;
                    aux=aux->next;  //percorrer os nodos
                    count++;  //contar quantos nodos vazios existe
                    nodos++;
            }
    
            x=(int*)realloc(x,c2*sizeof(int));
            x[c2-1]=count;    
        }

        count=0;
        if(aux!=NULL){
            aux=aux->next;
        }
    }

    int media=somaVetor(x, c2)/c2;
    return media;
}

int tempoAlocacaoMedio(int nodos, int num_alocs){
    int tempo=nodos/num_alocs;
    return tempo;
}

//componente de geração de solicitações
void gerarSolicitacoes(int numSolic, NODO* auxMemSol){
    //NODO *aux=auxMemSol;
    //NODO *aux2=auxMemSol;
    int i=0, t=0, mf=0, contaNegas=0, controlDealoc, pid=3;//pid contém o valor do id do processo, começa em 3 para não gerar conflito com a simulação anterior (valor inicial do pid)
    srand(time(0));
    int ger;//vai conter o número que escolhe dealocação ou alocação
    int num_units;//num_units aleatório que vão ser alocados
    int random_pid;//pid aleatório que vai ser dealocado (x>= 3 e x<=valor atual do pid, ou seja, o último id que foi alocado)
    int random_pid2; //este pid vai ser usado para a dealocação
    int nodosDevolvidos=0;
    int random_algoritmo;
    float percentagem=0.0;
    int nmralocs=0;
    int count=0;
    random_pid=pid;
    int valorproc;

    printf("\n\n----------------------------------------------------------Estado inicial da auxMemSol:----------------------------------------------------------\n");
    display(auxMemSol);  
    printf("----------------------------------------------------------------------------------------------------------------------------------------------\n");
    
    while(i < numSolic){
        //random_pid-pid correspodne ao número de alocações

        ger = random_int(0, 1);//0 --> alocação  //1 --> dealocação
        num_units=random_int(1, (memoria/unit));
        
        if(i<3)  //As 3 primeiras solicitações são necessáriamente Alocações
            ger=0;
        if(ger==0){//alocar
            random_algoritmo=random_int(1,4);  //Algoritmo de alocação random: 1-first/2-next/3-best/4-worst
            
            if(random_algoritmo==1){
                //printf("Pid: %d\nNum_units: %d\n", pid, num_units);
                nodosDevolvidos=alocate_units_mem_first(random_pid,num_units, auxMemSol);
                //printf("Sucesso ao alocar\n");
                random_pid++; //incrementa o random_pid de forma a serem processos diferentes
                t=t+tempoAlocacaoMedio(nodosDevolvidos, random_pid-pid)-(t/(random_pid-pid));
                mf=mf+((mediaFragmentosExternos(auxMemSol)-mf)/(random_pid-pid));
                if(nodosDevolvidos==-1){//uma solicitação foi negada
                    contaNegas++;
                }
            }
            if(random_algoritmo==2){
                //printf("Pid: %d\nNum_units: %d\n", pid, num_units);
                nodosDevolvidos=alocate_units_mem_next(random_pid,num_units, auxMemSol);
                //printf("Sucesso ao alocar\n");
                random_pid++; //incrementa o random_pid de forma a serem processos diferentes
                t=t+tempoAlocacaoMedio(nodosDevolvidos, random_pid-pid)-(t/(random_pid-pid));
                mf=mf+((mediaFragmentosExternos(auxMemSol)-mf)/(random_pid-pid));
                if(nodosDevolvidos==-1){//uma solicitação foi negada
                    contaNegas++;
                }
            }
            if(random_algoritmo==3){
                //printf("Pid: %d\nNum_units: %d\n", random_pid, num_units);
                nodosDevolvidos=alocate_units_mem_best(random_pid,num_units, auxMemSol);
                //printf("Sucesso ao alocar\n");
                random_pid++; //incrementa o random_pid de forma a serem processos diferentes
                t=t+tempoAlocacaoMedio(nodosDevolvidos, random_pid-pid)-(t/(random_pid-pid));
                mf=mf+((mediaFragmentosExternos(auxMemSol)-mf)/(random_pid-pid));
                if(nodosDevolvidos==-1){//uma solicitação foi negada
                    contaNegas++;
                }
            }
            if(random_algoritmo==4){
                //printf("Pid: %d\nNum_units: %d\n", pid, num_units);
                nodosDevolvidos=alocate_units_mem_worst(random_pid,num_units, auxMemSol);
                //printf("Sucesso ao alocar\n");
                random_pid++; //incrementa o random_pid de forma a serem processos diferentes
                t=t+tempoAlocacaoMedio(nodosDevolvidos, random_pid-pid)-(t/(random_pid-pid));
                mf=mf+((mediaFragmentosExternos(auxMemSol)-mf)/(random_pid-pid));
                if(nodosDevolvidos==-1){//uma solicitação foi negada
                    contaNegas++;
                }
            }

        }
        else{//dealocar
        /*printf("\n\n----------------------------------------------------------Estado da auxMemSol antes de uma dealocação:----------------------------------------------------------\n");
    display(auxMemSol);  
    printf("----------------------------------------------------------------------------------------------------------------------------------------------\n");
          */
            count=contaproc(auxMemSol);
            if(count!=0) {
            //printf("Número de processos existentes:%d\n",count);
            //printf("Count:%d\n",count);
            int *x = (int*)malloc(count*sizeof(int));
            x=nmrproc(auxMemSol,count);
            random_pid2=random_int(1, count); 
            //printf("Random_pid2:%d\n",random_pid2);
            //printf("Random:%d\n",random_pid2);
            //if(random_pid <= pid){//programação defensiva --> verificação -----comentei isto porque já não se aplica, dado que o random_pid vai incrementar
            valorproc=x[random_pid2-1];
            //printf("pid do processo:%d\n",valorproc);
            //printf("ValorProc:%d\n",valorproc);
                controlDealoc=deallocate_mem(valorproc, auxMemSol); } //vai tentar dealocar
                if(controlDealoc==1){
                    mf=mf+((mediaFragmentosExternos(auxMemSol)-mf)/(random_pid-pid));
                    //printf("Sucesso ao dealocar\n");  
                }
                //if(controlDealoc==-1)
                    //printf("Erro ao dealocar\n");   
        }
        i++;
    }
   
    nmralocs=random_pid - pid;
    percentagem=(float)contaNegas/(float)nmralocs;
    percentagem=percentagem*100;
    printf("Tempo de Alocação Médio: %d\nMédia de Fragmentos Externos: %d\nPercentagem de solicitações negadas: %.2f por cento\n", t, mf, percentagem);
}

int contaproc(NODO *auxMemSol){ //conta o numero de processos alocados de momento na memória
    int pid=-1;
    int count=0;
    if(auxMemSol->pid!=-1) {
        pid=auxMemSol->pid;
        count++;
    }
    while(auxMemSol->next!=NULL){
        if((auxMemSol->next->pid!=pid)&&(auxMemSol->next->pid!=-1)){
            pid=auxMemSol->next->pid;
             count++;
        }
        auxMemSol=auxMemSol->next;
    }
    
return count;
}

int *nmrproc(NODO *auxMemSol,int count){   //guarda num array de inteiros, o pid dos processos alocados de momento na memoria
    //NODO *aux=auxMemSol;
    int *x = (int*)malloc(count*sizeof(int));
    int pid=-1;
    int i=0;
    if(auxMemSol->pid!=-1) {
        pid=auxMemSol->pid;
       x[i]=auxMemSol->pid;
       i++;
    }
    for(int j=i;j<count;j++){
    while(auxMemSol!=NULL){
        if((auxMemSol->next->pid!=pid)&&(auxMemSol->next->pid!=-1)){
            pid=auxMemSol->next->pid;
            x[j]=auxMemSol->next->pid;
            break;
        } 
        auxMemSol=auxMemSol->next;
    }  
    }
return x;
}  
//componenete de relatório--------------------------------------------//
int somaVetor(int *v, int tam){
    int soma=0;
    for(int i=0;i<tam;i++){
        soma+=v[i];
    }
    return soma;
}

//-----------------------------------------------------------------//

//função random
int random_int(int min, int max)
{
   //return min + rand() % (max+1 - min)+min;
  return ((rand()%(max-min+1))+min);
}

//-----------------------------------------------------------------//
//ler instruções dos ficheiros progenitor.txt filho1.txt etc
int countInstr(char* nome){
    char c;
    int count=0;
    FILE* fp = fopen(nome, "r");
    if (fp == NULL){
        perror("Error while opening the file.\n");
        exit(1);
    }
    for (c = getc(fp); c != EOF; c = getc(fp)){
        if (c == '\n') // Increment count if this character is newline 
            count = count + 1;
    }
    return count;
}

INSTRUCTION* lerIntr(char *nome){
    FILE* fp = fopen(nome, "r");
    if (fp == NULL){
        perror("Error while opening the file.\n");
        exit(1);
    }
    char c1;
    int count=countInstr(nome),val, k=0; 
    INSTRUCTION* Instrs = (INSTRUCTION*)malloc((count+1)*sizeof(INSTRUCTION));
    FILE* aux = fopen(nome, "r");
    while(fscanf(aux, "%c %d\n", &c1, &val)==2 && k<(count+1)){
        /*if(k=0 && (val < 3 || val > 10)){ //um processo apenas pode alocar entre 3 e 10 unidades de memória
            printf("FATAL ERROR: Alocação não permitida!");
            return 1;
        }*/
            
        Instrs[k].n=val;
        Instrs[k].instr=c1;
        k++;    
    }
    return Instrs;
}

//ler programas do ficheiro plan.txt
int pid=0; 
QUEUE *readPrograms(){
    FILE *fp;    
    fp=fopen("plan.txt", "r");
    if (fp == NULL){
        perror("Error while opening the file.\n");
        exit(1);
    }
    int start; //countAux=0;
    char nome[50];
    //printf("%d programas que vão ser executados\n", count);
    QUEUE* Programas = ConstructQueue(100);
    
    while(fscanf(fp, "%s %d\n", nome , &start)==2){
        //countAux+=(countInstr(nome)+1);
        PCB* proc=(PCB*)malloc(sizeof(PCB));
        strcpy(proc->nome, nome);
        proc->var=0;
        proc->pid=pid;
        proc->start=start;
        proc->instr=lerIntr(nome);
        proc->numInstr=countInstr(nome)+1;
        proc->tempVida=countInstr(nome);
        Enqueue(Programas, proc);
        if(isEmpty(Programas))
            printf("Vazia");
        pid++;
    }
    fclose(fp);
    return Programas;
}

int countControl(){//conta o número de linhas do ficheiro control.txt
    FILE *fp=fopen("control.txt", "r");
    int count=1;
    char c;
    for (c = getc(fp); c != EOF; c = getc(fp)) 
        if (c == '\n') // Increment count if this character is newline 
            count++; 
    return count;
}

//ler ficheiro control.txt e devolver array de intruções de gestão
char* lerControl(int tam, char* cmd){//passa o nº de controlos por referência e um array com os nº dos controlos
    char ci;
    int i=0;
    FILE *fp1=fopen("control.txt", "r"); 
    while(fscanf(fp1,"%c\n", &ci)==1){
        cmd[i]=ci;
        printf("Control[%d]: %c\n", i, cmd[i]);
        i++;
    }
    return cmd;
}

//tratar diferentes tipos de intruções 
PCB* execInstr(PCB* proc, int *i,int *t,QUEUE* Prontos,QUEUE* Blocked,QUEUE* Terminated, NODO* auxMem){
   // Enqueue(Prontos, proc);
    //proc->estadoProc=READY;
    //int startingTime=*t;//tempo em que o processo começa a correr
    
    
    char letra = memory[*i].instr;
    int num = memory[*i].n;
    printf("num: %d\n", num);
    
    //aloca memória
    if('G'== letra){
        proc->numMem=num;
        printf("----------Algoritmos de alocação de mememória----------\n\nSelecione uma opção:\n1-First Fit\n2-Next Fit\n3-Best Fit\n4-Worst Fit\n--------------------\n");
        int aloc, nodosPercorridos=0;
        scanf("%d", &aloc);
        
        switch(aloc){
            case 1:
                nodosPercorridos = alocate_units_mem_first(proc->pid,proc->numMem, auxMem);
            break;
            case 2:
                nodosPercorridos = alocate_units_mem_next(proc->pid,proc->numMem,auxMem);
            break;
            case 3:
                nodosPercorridos = alocate_units_mem_best(proc->pid,proc->numMem, auxMem);
            break;
            case 4:
                nodosPercorridos = alocate_units_mem_worst(proc->pid,proc->numMem, auxMem);
            break;
            default:
                nodosPercorridos = alocate_units_mem_first(proc->pid,proc->numMem, auxMem);
        }
        if(nodosPercorridos!=-1)
            printf("nodosPercorridos = %d\n", nodosPercorridos);
    }

    //dealoca memória
    /*if('J'== letra){
        int test = deallocate_mem(proc->pid, auxMem);
        if(test==1)
            printf("Dealocação bem sucedida!\n");
        else
            printf("Fracasso na dealocação\n");     
    }
    */

    //modifica o valor da variável
    if('M'== letra){
        proc->var=num;  
    }
    
    //adiciona o valor à variável
    if('A'== letra){
        proc->var+=num;
    }
    //subtrair o valor à variável
    if('S'==letra){
        proc->var-=num;
    }
    
    //Funcionalidades por implementar (parcial)
    //passa o estado do processo a WAITING e muda de fila
    if('B' == letra){
        proc->estadoProc = WAITING;
        proc = Dequeue(Prontos);
        Enqueue(Blocked, proc);

    }
    //passa o estado do programa a TERMINATED e muda de fila
    if('T' == letra){
        proc->estadoProc=TERMINATED;  //altera o valor da variavel para 3(Terminated)
        proc=Dequeue(Prontos);        //Tira da lista prontos
        Enqueue(Terminated, proc);    //passa para a fila Terminated


    }
    //fork(): cria cópia do programa e o filho executa logo de imediato após o pai
    /*if('C' == letra){
        proc->ppid = fork(); 
        if(proc->pid == 0){ //para ter acerteza que o fork funcionou
            printf("Filho! \n");
        }
        if(proc->ppid > 0){
            int returnv = sleep(TIMELIMIT * num); //vou acreditar que o TIMELIMIT é a unidade do tempo da simulação
            if(returnv == 0){ //só para ter a certeza que esperou(depois apagar)
                printf("I was a good boy and waited\n");
            }
        }

    }*/
    /*exec(): limpa o programa atual e substitui por filename. 
    Pode ter de carregar o novo programa em memória.*/
    /*if('L'==letra){
        while(num!=0){
            memory[aux2]=NULL;  //limpa o programa actual
            memory[aux2]=proc->instr[aux2]; //substituir por cada instrução
            aux2++;
            num--;
    }
    }
    */
        
    
    

    //if(*t==startingTime + proc->tempVida){//no momento em que o processo termina a execução
            
            //retirar do estado running
    //   } 

    return proc;
}


//passa um processo de ready para running
PCB* addRunning(PCB* proc, int *i){//devolve fila ready vazia
    //*i é o index do array memory
    int k=0;
    if(proc->estadoProc==READY){//confirma que está pronto
        printf("Running\n");
        int aux=*i;
        
        while((*i) < aux+proc->numInstr && k < proc->numInstr){
            
            memory[*i]=proc->instr[k];//adiciona todas as instruções do processo ao array (running) e tira da ready
            k++;
            (*i)++;
        }        
    }
    proc->tempVida=proc->numInstr;
    proc->estadoProc=RUNNING;
    return proc;
}

//---escalonador FCFS---//
PCB* FCFS(QUEUE* Prontos, int *time, int *index, QUEUE* Terminated){//adiciona um processo à memory (running)
    PCB* procFinal;
    if(!isEmpty(Prontos)){//se a fila não estiver vazia e os processos tiverem terminado
        
        procFinal=Dequeue(Prontos);
        procFinal=addRunning(procFinal, index);//adiciona as instruções ao array memory        
    }
    return procFinal;
}
//--------------------------------------------------
//Escalonador SJFS

/*//adiciona ao array Memory pela ordem dos menores 
PCB* addRunningSJF(QUEUE* Prontos, int *i){
    QUEUE* auxProntos;//fila auxiliar para encontrar o processo com menor tempo de vida
    PCB* proc, aux; //aux é o intermédio e o proc é o menor
    int k, iaux, min=INT_MAX;

    while(iaux != NULL){
        aux=Dequeue(Prontos);
        if(aux->tempVida < min){
            min=aux->tempVida;
            proc=aux;//proc contém o menor
        }
        iaux++;
    }


    if(proc->estadoProc==READY){//confirma que está pronto
        printf("Running\n");
        int aux=*i;

        while((*i) < aux+proc->numInstr && k < proc->numInstr){
                
                memory[*i]=proc->instr[k];//adiciona todas as instruções do processo ao array (running) e tira da ready
                k++;
                (*i)++;
            }        
    }

    proc->tempVida=proc->numInstr;
    proc->estadoProc=RUNNING;
    return proc;
}
*/

PCB* SJFS(QUEUE* Prontos, int* index)
{
    //variável de controlo do min
    PCB* minTempVida;
    minTempVida->tempVida=INT_MAX;
    
    PCB* procFinal;
    
    int count=0;
    if(!isEmpty(Prontos))
    {
        printf("Entrei SJFS\n");
        /*if(Prontos->size == 1)
        {
            procFinal = Dequeue(Prontos);
            procFinal = addRunning(procFinal, index);
        }
        else{*/
            //QUEUE* aux=Prontos;
            //encontrar o menor tempVida para o executar
            while(count < Prontos->size){
                procFinal = Dequeue(Prontos);
                if(procFinal->estadoProc==READY)//verificação -> programação defensiva
                    if(procFinal->tempVida < minTempVida->tempVida) 
                        minTempVida=procFinal;
                Enqueue(Prontos, procFinal);
                count++;
            }
            //retirar o menor da fila de prontos
            procFinal = addRunning(minTempVida, index);
        //}
    }
    
    return procFinal;
}

//-------------------------------------------------------------------

PCB* Prioridades(QUEUE* Prontos, int *index)
{    printf("Testar");
    //variável de controlo do min
    PCB* minPriori;
    minPriori->priori=INT_MAX;
    int count=0;
    PCB* procFinal;
    if(!isEmpty(Prontos))
    {   
        while(count < Prontos->size){
            procFinal = Dequeue(Prontos);
            if(procFinal->estadoProc==READY)//verificação -> programação defensiva
                if(procFinal->priori < minPriori->priori) 
                    minPriori=procFinal;
            Enqueue(Prontos, procFinal);
            count++;
        }
        procFinal = addRunning(minPriori, index);        
    }
    return procFinal;
}

void report(int *tempo, QUEUE* Bloqueados, QUEUE* Prontos, QUEUE* Terminados){
    printf("\n----------------\nReport do simulador:\nTEMPO ATUAL: %d\n", *tempo);
    printf("PROCESSO EM EXECUÇÂO:\n");
    printf("---------------\nPROCESSOS BLOQUEADOS:\n");
    PCB* proc;
    while((proc=Dequeue(Bloqueados))!=NULL){
        printf("PID: %d\nPPID: %d\nPrioridade: %d\nValor: %d\nTempo de iniciação: %d\nTempo do CPU Usado: %d\n", proc->pid, proc->ppid, proc->priori, proc->var, proc->start, proc->tempVida);
    }
    printf("---------------\nPROCESSOS PRONTOS A EXECUTAR:\n");
    while((proc=Dequeue(Prontos))!=NULL){
        printf("PID: %d\nPPID: %d\nPrioridade: %d\nValor: %d\nTempo de iniciação: %d\nTempo do CPU Usado: %d\n", proc->pid, proc->ppid, proc->priori, proc->var, proc->start, proc->tempVida);
    }
    printf("---------------\nPROCESSOS TERMINADOS:\n");
    while((proc=Dequeue(Terminados))!=NULL){
       printf("PID: %d\nPPID: %d\nPrioridade: %d\nValor: %d\nTempo de iniciação: %d\nTempo do CPU Usado: %d\n", proc->pid, proc->ppid, proc->priori, proc->var, proc->start, proc->tempVida);
    }
}

//imprime no ecrã informação final importane acerca do gestor
void printFinalInfo(QUEUE* Prontos, NODO* auxMem){
     //verifica se todos os processos foram escalonados
        printf("\n---Estado da fila---\n");
        if(isEmpty(Prontos)==1)
            printf("\tVazia\n");
        else
        {
            printf("\tNão vazia\n");
        }
        printf("----------------------------\n\n");
      
        printf("----------------------------------------------------------Estado final da memMan:----------------------------------------------------\n");
        display(auxMem);
        printf("-------------------------------------------------------------------------------------------------------------------------------------\n"); 
}

//-----------------------//       