#include "functions.h"


typedef struct {
long tipo;
struct {
int pid_passageiro;
int pid_condutor;
char local_encontro[100];
long data;
float pontos;
float valor;
} dados;
} MsgViagem;


void fUSR(int sinal) {
atualizaEstruturas();
}

void fTERM(int sinal){
atualizaFicheiro();
}

void mensagens(int a){
   int result=fork();
   if(result==0){
        
        alarm(60);
    }
}

Tcondutor *driverArray;
Tpassageiro *passengerArray;

struct sembuf UP = { 0, 1, 0 } ;
struct sembuf DOWN = { 0, -1, 0 };


int main(){


signal (SIGUSR1, fUSR ); 
signal(SIGTERM, fTERM);
signal(SIGALRM,mensagens);


/*Criar semaforo*/
int idSemaforo = semget ( 1000, 1, IPC_CREAT | 0666 );
exit_on_error (idSemaforo, "Criação/Ligação");
int statusInit = semctl ( idSemaforo, 0, SETVAL, 1);
exit_on_error (statusInit, "Inicialização");
/*Fim semaforo*/


int idFilaMensagem = msgget ( 2500, IPC_CREAT | 0600 );
if ( idFilaMensagem < 0 ) {
printf ("Erro na criação da fila de mensagens\n" );
exit(1);
}

int idCOND= shmget ( 1234567, 100, 0600);
int idPASS= shmget (89101112, 100, 0600);

int pid;
pid=fork();

if(pid == -1){
    printf("Erro na criação do filho.\n");
}

if(pid == 0){ //processo filho para mensagens tipo 1
    int status;
    MsgViagem msg;
    while(1){
        printf("Waiting for type 1 message.\n");
        status = msgrcv( idFilaMensagem, &msg, sizeof(msg.dados), 1, 0);
        exit_on_error (status, "Erro na recepção da mensagem.\n");
        printf("Just received type 1 message.\n");
        printf("PID do passageiro: %d\n",msg.dados.pid_passageiro);

        int sem_id;
        int statusSemaforo;
        sem_id = semget ( 1000, 1, 0600 );
        exit_on_error (sem_id, "Criação/Ligação");

        statusSemaforo = semop ( sem_id, &DOWN, 1 );

        int id_condutorf = shmget(1234567,0,0);
        exit_on_error(id_condutorf,"Erro no shmget do condutor.");
        char * mem_addrf=shmat(id_condutorf,0,0);
        exit_on_null(mem_addrf,"Erro shmat do condutor.");
        Tcondutor *condPointerf=(Tcondutor *)mem_addrf;
        int codPointer_indexf;
        int min=time(NULL);
        int min_index=-1;
        for(int codPointer_indexf=0;codPointer_indexf!=numberLinesFile();codPointer_indexf++){
            if(condPointerf[codPointer_indexf].activo == 1){
                    if(condPointerf[codPointer_indexf].disponivel_desde < min){
                        min_index=codPointer_indexf;
                    } 
            }
        }      
        condPointerf[min_index].activo=0; //meter condutor inativo
        printf("Sending type 2 message.\n");
        printf("PID do condutor a quem foi atribuido o trabalho: %d\n",condPointerf[min_index].PID);
        msg.tipo=condPointerf[min_index].PID;
        status = msgsnd( idFilaMensagem, &msg, sizeof(msg.dados), 0);
        printf("Just sent type 2 message.\n");

        statusSemaforo = semop ( sem_id, &UP, 1 );
        
    }

}else{ //processo pai

int pid2;
pid2=fork();
if(pid2 == -1){
    printf("Erro na criação do filho.\n");
}
if(pid2 == 0){ //processo filho para processar mensagens tipo 4
   
  int status2;
    MsgViagem msg2;
    while(1){
        printf("Waiting for type 4 message.\n");
        status2 = msgrcv( idFilaMensagem, &msg2, sizeof(msg2.dados), 4, 0); 
        exit_on_error (status2, "Erro na recepção da mensagem.\n");
        printf("Just received type 4 message.\n");

        int sem_id2;
        int statusSemaforo2;
        sem_id2 = semget ( 1000, 1, 0600 );
        exit_on_error (sem_id2, "Criação/Ligação");

        statusSemaforo2 = semop ( sem_id2, &DOWN, 1 );
        
        int id_condutorf2 = shmget(1234567,0,0);
        exit_on_error(id_condutorf2,"Erro no shmget do condutor.");
        char * mem_addrf2=shmat(id_condutorf2,0,0);
        exit_on_null(mem_addrf2,"Erro shmat do condutor.");
        Tcondutor *condPointerf2=(Tcondutor *)mem_addrf2;
        int codPointer_indexf2;
        int min_index=-1;
        int num_cond=msg2.dados.pid_condutor;
        for(int codPointer_indexf2=0;codPointer_indexf2!=numberLinesFile();codPointer_indexf2++){
            if(condPointerf2[codPointer_indexf2].PID == num_cond){
                    float saldo1=msg2.dados.valor;
                    float pontos1=msg2.dados.pontos;
                    float saldoMemoria=condPointerf2[codPointer_indexf2].saldo;
                    int pontosMemoria=condPointerf2[codPointer_indexf2].pontos;
                    condPointerf2[codPointer_indexf2].saldo=saldo1+saldoMemoria;
                    condPointerf2[codPointer_indexf2].pontos=pontosMemoria+pontos1;
                    condPointerf2[codPointer_indexf2].activo=1;
                    condPointerf2[codPointer_indexf2].disponivel_desde=time(NULL);
            }
        }
         statusSemaforo2 = semop ( sem_id2, &UP, 1 );      
    }
}

else{ //processo pai
    if(idCOND == -1){

            int sem_id3;
            int statusSemaforo3;
            sem_id3 = semget ( 1000, 1, 0600 );
            exit_on_error (sem_id3, "Criação/Ligação");

            statusSemaforo3 = semop ( sem_id3, &DOWN, 1 );

            printf("Memoria condutor não existente. Vou criar.\n");
            //criar a memoria partilhada para o condutor
            int id_cond= shmget(1234567, (numberLinesFile()) * sizeof(Tcondutor) , IPC_CREAT | 0600 ); //determinar o numero de condutores existentes
            exit_on_error(id_cond, "shmget");
            Tcondutor* condVector = (Tcondutor *)shmat(id_cond, 0 , 0);
            exit_on_null(condVector,"shmat");

        FILE *condutores;
		char s1[100];
		condutores = fopen ("condutores.txt", "r");
        int cond_index=0;
		while ( fgets (s1, 100, condutores ) != NULL ) {
			char *numero_estudante= strtok(s1,":");
 			char *nome=strtok(NULL,":");
			char *turma= strtok (NULL,":");
 			char *telemovel=strtok(NULL,":");
			char *email= strtok (NULL,":");
 			char *tipo_viatura=strtok(NULL,":");
			char *marca= strtok (NULL,":");
 			char *matricula=strtok(NULL,":");
			char *numero_viagens= strtok(NULL,":");
 			char *pontos=strtok(NULL,":");
			char *saldoViagem=strtok(NULL,":");
			
            /*Converter pontos*/
            char auxPontinhos[10];
            strcpy(auxPontinhos,pontos);
            int pontosFinal=atoi(auxPontinhos);
        
            /*Converter saldo*/
            char auxSaldinho[10];
            strcpy(auxSaldinho,saldoViagem);
            double saldoFinal=atof(auxSaldinho);

            /*Converter viagens*/
            char auxViagens[10];
            strcpy(auxViagens,numero_viagens);
            int viagensFinal=atoi(auxViagens);
       
            /*Converter numero*/
            char auxNumero[20];
            strcpy(auxNumero,numero_estudante);
            int numeroFinal=atoi(auxNumero);

            condVector[cond_index].numero=numeroFinal; //numero estudante
            strcpy(condVector[cond_index].nome,nome);    //nome
            strcpy(condVector[cond_index].turma,turma);    //turma
            strcpy(condVector[cond_index].telemovel,telemovel);    //telemovel
            strcpy(condVector[cond_index].email,email);    //email
            strcpy(condVector[cond_index].tipo,tipo_viatura);    //tipo viatura
            strcpy(condVector[cond_index].marca,marca);    //marca
            strcpy(condVector[cond_index].matricula,matricula);    //matricula
            condVector[cond_index].viagens=viagensFinal; //numero viagens
            condVector[cond_index].saldo=saldoFinal;  //saldo 
            condVector[cond_index].pontos=pontosFinal; //pontos
            condVector[cond_index].activo=1; //activo
            condVector[cond_index].disponivel_desde=time(NULL); //disponivel desde
            condVector[cond_index].PID=getpid(); //PID
            cond_index++;
}
fclose(condutores);
printf("Acabei de criar a memória dos condutores.\n");
statusSemaforo3 = semop ( sem_id3, &UP, 1 ); 
        }

    if(idPASS == -1){

            int sem_id4;
            int statusSemaforo4;
            sem_id4 = semget ( 1000, 1, 0600 );
            exit_on_error (sem_id4, "Criação/Ligação");

            statusSemaforo4 = semop ( sem_id4, &DOWN, 1 );

            statusSemaforo4 = semop ( sem_id4, &UP, 1 ); 

            printf("Memória partilhada dos passageiros não existe. Vou criar.\n");
            //criar a memoria partilhada para o passageiro
            int id_pass= shmget(89101112, (numberLinesFilePassageiro()) * sizeof(Tpassageiro) , IPC_CREAT | 0600 ); //meter a memoria do condutor
            exit_on_error(id_pass, "shmget");
            Tpassageiro* passVector = (Tpassageiro *)shmat(id_pass, 0 , 0);
            exit_on_null(passVector,"shmat");
        FILE *passageiros;
		char p1[100];
		passageiros = fopen ("passageiros.txt", "r");
        int pass_index=0;
		while ( fgets (p1, 100, passageiros ) != NULL )  {
                char *aNumero  = strtok(p1,":");
				char auxNum[10];
				strcpy(auxNum,aNumero);
                passVector[pass_index].numero = atoi(auxNum);

				char *aNome = strtok(NULL,":");
				char auxNome[50];
				strcpy(auxNome,aNome);
				strcpy(passVector[pass_index].nome,auxNome);
				
				char *aTurma = strtok(NULL,":");
                char auxTurma[10];
                strcpy(auxTurma,aTurma);
                strcpy(passVector[pass_index].turma,auxTurma);

				char *aTelemovel = strtok(NULL,":");
                char auxTelemovel[15];
                strcpy(auxTelemovel,aTelemovel);
                strcpy(passVector[pass_index].telemovel,auxTelemovel);

				char *aEmail = strtok(NULL,":");
                char auxEmail[40];
                strcpy(auxEmail,aEmail);
                strcpy(passVector[pass_index].email,auxEmail);

				char *aCartao = strtok(NULL,":");
                char auxCartao[20];
                strcpy(auxCartao,aCartao);
                strcpy(passVector[pass_index].c_credito,auxCartao);
				
				pass_index++;

    }
fclose(passageiros);
printf("Acabei de criar a memória dos passageiros.\n");
statusSemaforo4= semop ( sem_id4, &UP, 1 ); 
  }

//apresentar o menu
char numeroOp[10];
int option = -1;
  while(option != 0) { /* percorre o ciclo enquanto for diferente de 0, cada case é uma opção válida para o utilizador */
    printf("Menu \n Para imprimir memoria insira 1 \n Para alterar passageiros insira 2 \n Para alterar condutores insira 3 \n Para sair insira qualquer outra coisa\n");
    printf("Opcao: ");
    fgets(numeroOp, 10, stdin);
    option = atoi(numeroOp);
    switch(option) {
    case 1:;
//dispor a informação dos condutores
    
            int sem_id5;
            int statusSemaforo5;
            sem_id5 = semget ( 1000, 1, 0600 );
            exit_on_error (sem_id5, "Criação/Ligação");
            statusSemaforo5 = semop ( sem_id5, &DOWN, 1 );

        int id_condutor = shmget(1234567,0,0);
        exit_on_error(id_condutor,"Erro no shmget do condutor.");
        char * mem_addr=shmat(id_condutor,0,0);
        exit_on_null(mem_addr,"Erro shmat do condutor.");
        Tcondutor *condPointer=(Tcondutor *)mem_addr;
        int codPointer_index;
        int activeDrivers=0;
        for(int codPointer_index=0;codPointer_index!=numberLinesFile();codPointer_index++){
            printf("Número do condutor: %d\n",condPointer[codPointer_index].numero);
            printf("Nome do condutor: %s\n",condPointer[codPointer_index].nome);
            printf("Turma do condutor: %s\n",condPointer[codPointer_index].turma);
            printf("Telemóvel do condutor: %s\n",condPointer[codPointer_index].telemovel);
            printf("Email do condutor: %s\n",condPointer[codPointer_index].email);
            printf("Tipo de viatura do condutor: %s\n",condPointer[codPointer_index].tipo);
            printf("Marca da viatura do condutor: %s\n",condPointer[codPointer_index].marca);
            printf("Matrícula da viatura do condutor: %s\n",condPointer[codPointer_index].matricula);
            printf("Número de viagens do condutor: %d\n",condPointer[codPointer_index].viagens);
            printf("Pontos do condutor: %d\n",condPointer[codPointer_index].pontos);
            printf("Saldo do condutor: %f\n",condPointer[codPointer_index].saldo);
            if(condPointer[codPointer_index].activo ==1){
                printf("Condutor activo. \n");
                activeDrivers++;
            }else{
                printf("Condutor inactivo.\n");
            }
            printf("Condutor disponível desde: %ld\n",condPointer[codPointer_index].disponivel_desde);  
        }
            printf("Número total de condutores: %d\n",numberLinesFile());            
            printf("Número de condutores activos: %d\n",activeDrivers);

//fim da informação dos condutores


//dispor informação dos passageiros

        int id_passageiro = shmget(89101112,0,0);
        exit_on_error(id_passageiro,"Erro no shmget do passageiro.");
        char * mem_addr_pass=shmat(id_passageiro,0,0);
        exit_on_null(mem_addr_pass,"Erro shmat do passageiro.");
        Tpassageiro *passPointer=(Tpassageiro *)mem_addr_pass;
        int passPointer_index;
        for(int passPointer_index=0;passPointer_index!=numberLinesFilePassageiro();passPointer_index++){
            printf("Número do passageiro: %d\n",passPointer[passPointer_index].numero);
            printf("Nome do passageiro: %s\n",passPointer[passPointer_index].nome);
            printf("Turma do passageiro: %s\n",passPointer[passPointer_index].turma);
            printf("Telemóvel do passageiro: %s\n",passPointer[passPointer_index].telemovel);
            printf("Email do passageiro: %s\n",passPointer[passPointer_index].email);
            printf("Cartão de crédito do passageiro: %s\n",passPointer[passPointer_index].c_credito);
        }
            printf("Número total de passageiros: %d\n",numberLinesFilePassageiro());            

//fim da informação dos passageiros
    statusSemaforo5 = semop ( sem_id5, &UP, 1 );
    break;


case 2:;


            int sem_id6;
            int statusSemaforo6;
            sem_id6 = semget ( 1000, 1, 0600 );
            exit_on_error (sem_id6, "Criação/Ligação");
            statusSemaforo6 = semop ( sem_id6, &DOWN, 1 );


        int id_passageiro2 = shmget(89101112,0,0);
        exit_on_error(id_passageiro2,"Erro no shmget do passageiro2.");
        char * mem_addr_pass2=shmat(id_passageiro2,0,0);
        exit_on_null(mem_addr_pass2,"Erro shmat do passageiro2.");
        Tpassageiro *passPointer2=(Tpassageiro *)mem_addr_pass2;

    	printf("Numero do passageiro a alterar: ");
   		fgets(numeroOp, 10, stdin);
		int numberP=atoi(numeroOp);
		int passInicio;
        int var=0;
        for(int passInicio=0;passInicio!=numberLinesFilePassageiro() && var!=1 ;passInicio++){
            if(passPointer2[passInicio].numero==numberP){
                printf("Encontramos o passageiro que procura.\n");
                
	            char line[50];
 	            int count;
  	           
			    printf("ATENÇÃO: Para todos os campos que não pretenda alterar, basta pressionar Enter. \n");
			   
			    printf("Inserir nova turma: ");
			    fgets(line,50,stdin);
			    line[strlen(line)-1]='\0';
			    if(strcmp(line,"")!=0){
				    strcpy(passPointer2[passInicio].turma,line);
			    }
			    printf("Inserir novo telemovel : ");
			    fgets(line,50,stdin);
			    line[strlen(line)-1]='\0';
			    if(strcmp(line,"")!=0){
				    strcpy(passPointer2[passInicio].telemovel,line);
			    }
			    printf("Inserir novo email: ");
			    fgets(line,50,stdin);
			    line[strlen(line)-1]='\0';
			    if(strcmp(line,"")!=0){
				    strcpy(passPointer2[passInicio].email,line);
			    }
			    printf("Inserir novo Cartão de Credito: ");
			    fgets(line,50,stdin);
			    line[strlen(line)-1]='\0';
			    if(strcmp(line,"")!=0){
				    strcpy(passPointer2[passInicio].c_credito,line);
			    }
			    var=1;
		    }
        }
   statusSemaforo6 = semop ( sem_id6, &UP, 1 );		
break;  

case 3:;

            int sem_id7;
            int statusSemaforo7;
            sem_id7 = semget ( 1000, 1, 0600 );
            exit_on_error (sem_id7, "Criação/Ligação");
            statusSemaforo7 = semop ( sem_id7, &DOWN, 1 );


    int id_condutor2 = shmget(1234567,0,0);
    exit_on_error(id_condutor2,"Erro no shmget do condutor.");
    char * mem_addr2=shmat(id_condutor2,0,0);
    exit_on_null(mem_addr2,"Erro shmat do condutor.");
    Tcondutor *condPointer2=(Tcondutor *)mem_addr2;

    printf("Numero de condutor a alterar: ");
   	fgets(numeroOp, 10, stdin);
	int numberC=atoi(numeroOp);
	int condInicio;
    int var2=0;
    for(int condInicio=0;condInicio!=numberLinesFile() && var2!=1 ;condInicio++){
	    if(condPointer2[condInicio].numero == numberC){
            char line[50];
            printf("Encontramos o condutor que procura.\n");
            printf("ATENÇÃO: Para todos os campos que não pretenda alterar, basta pressionar Enter. \n");
            printf("Inserir nova turma: ");
			fgets(line,50,stdin);
			line[strlen(line)-1]='\0';
			if(strcmp(line,"")!=0){
				strcpy(condPointer2[condInicio].turma,line);
			}
			printf("Inserir novo numero de telemovel : ");
			fgets(line,50,stdin);
			line[strlen(line)-1]='\0';
			if(strcmp(line,"")!=0){
				strcpy(condPointer2[condInicio].telemovel,line);
			}
			printf("Inserir novo email: ");
			fgets(line,50,stdin);
			line[strlen(line)-1]='\0';
			if(strcmp(line,"")!=0){
				strcpy(condPointer2[condInicio].email,line);
			}
			printf("Inserir novo tipo de viatura: ");
			fgets(line,50,stdin);
			line[strlen(line)-1]='\0';
			if(strcmp(line,"")!=0){
				strcpy(condPointer2[condInicio].tipo,line);
			}
			printf("Inserir nova marca de viatura: ");
			fgets(line,50,stdin);
			line[strlen(line)-1]='\0';
			if(strcmp(line,"")!=0){
				strcpy(condPointer2[condInicio].marca,line);
			}
			 printf("Inserir nova matricula: ");
			fgets(line,50,stdin);
			line[strlen(line)-1]='\0';
			if(strcmp(line,"")!=0){
				strcpy(condPointer2[condInicio].matricula,line);
			}
			var2=1;
        }
    }

    statusSemaforo6 = semop ( sem_id6, &UP, 1 );		
    break;

	default:
		exit(0);
  }
}

}
}

exit(0);
}
