#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <time.h>
#include <signal.h>
#include <sys/wait.h>
#define exit_on_error(s,m); if(s<0){ perror(m), exit(1);}
#define exit_on_null(s,m) if (s==NULL) { perror(m); exit(1); }
#include <sys/ipc.h>
#include <sys/sem.h>

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

typedef struct {
int numero;
char nome[50];
char turma[10];
char telemovel[15];
char email[40];
char tipo[20];
char marca[20];
char matricula[15];
int viagens;
int pontos;
float saldo;
int activo;
long disponivel_desde;
int PID;
} Tcondutor;


struct sembuf UP = { 0, 1, 0 } ;
struct sembuf DOWN = { 0, -1, 0 };


int numberLinesFile(){
FILE *condutores;
		char s1[100];
        int cont=0;
		condutores = fopen ("condutores.txt", "r");
		while ( fgets (s1, 100, condutores ) != NULL ) {
            cont++;
        }
return cont;
}


static int number;

    void resposta(int sinal){
        printf("Numero do condutor: %d\n",number);


            int sem_id2;
            int statusSemaforo2;
            sem_id2 = semget ( 1000, 1, 0600 );
            exit_on_error (sem_id2, "Criação/Ligação");
            statusSemaforo2 = semop ( sem_id2, &DOWN, 1 );


        int id_condutor = shmget(1234567,0,0);
        exit_on_error(id_condutor,"Erro no shmget do condutor.");
        char * mem_addr=shmat(id_condutor,0,0);
        exit_on_null(mem_addr,"Erro shmat do condutor.");
        Tcondutor *condPointer=(Tcondutor *)mem_addr;
        int codPointer_index;
        for(int codPointer_index=0;codPointer_index!=numberLinesFile();codPointer_index++){
            if(condPointer[codPointer_index].numero==number){
                printf("Condutor inativado.\n");
                condPointer[codPointer_index].activo=0;
            }
        }
    statusSemaforo2 = semop ( sem_id2, &UP, 1 );	
    exit(0);
}



int main(){


/*Inicio semaforo*/
    int idSemaforo = semget ( 1000, 1, IPC_CREAT | 0666 );
    exit_on_error (idSemaforo, "Criação/Ligação");
/*Fim semaforo*/

        int sem_id;
        int statusSemaforo;
        sem_id = semget ( 1000, 1, 0600 );
        exit_on_error (sem_id, "Criação/Ligação");


    int activo;
    int idCOND= shmget (1234567, 0, 0);
    exit_on_error(idCOND,"Erro no shmget do condutor.");
    char num[10];
    MsgViagem msg;
    printf("Número de condutor: ");
    fgets(num, 10, stdin);
    int numeroCondutor = atoi(num);
        char * mem_addr=shmat(idCOND,0,0);
        exit_on_null(mem_addr,"Erro shmat do condutor.");
        Tcondutor *condPointer=(Tcondutor *)mem_addr;
        int codPointer_index;
        for(int codPointer_index=0;codPointer_index!=numberLinesFile();codPointer_index++){
            if(condPointer[codPointer_index].numero==numeroCondutor){
                number=numeroCondutor;
                printf("Nome do condutor: %s\n",condPointer[codPointer_index].nome);
                condPointer[codPointer_index].activo=1;
                condPointer[codPointer_index].disponivel_desde=time(NULL);
                condPointer[codPointer_index].PID=getpid();
            }
        }
 statusSemaforo = semop ( sem_id, &UP, 1 );	

    signal (SIGINT, resposta );

//espera mensagens tipo 2
while(1){
    int idFilaMensagem = msgget ( 2500, 0600 );
    int status_msg;
	//Espera pela resposta
	printf("Esperando a mensagem tipo 2\n");
    printf("O meu PID é: %d\n",getpid());
	status_msg = msgrcv( idFilaMensagem, &msg, sizeof(msg.dados), getpid(),0);
	printf("PID do passageiro: %d\n", msg.dados.pid_passageiro);     


//enviar mensagem tipo 3
        msg.tipo = msg.dados.pid_passageiro;
        msg.dados.pid_condutor = getpid();
        printf("Indique onde se encontra: ");
        char place[100];
        fgets(place, 100, stdin);
        strcpy(msg.dados.local_encontro,place);

        int status;
        printf("Sending type 3 message.\n");
        status = msgsnd( idFilaMensagem, &msg, sizeof(msg.dados), 0);
        exit_on_error(status, "Erro de envio da mensagem.")
	    printf("Just sent type 3 message.\n");
    
//simular serviço

srand(time(NULL));
int waitingTime = rand() % 20 + 1;
printf("Numero aleatorio: %d\n",waitingTime);
sleep(waitingTime);

//enviar recibo
        char v[10];
        printf("Valor da viagem:");
		fgets(v, 10, stdin);
		float valor = atof(v);

        char class[10];
		printf("Classifique a viagem de 0 a 10: ");
		fgets(class, 10, stdin);
		float classificacao = atof(class);
		
        int status2;
		msg.tipo = 4;
		msg.dados.pid_condutor = getpid();
		msg.dados.pontos = classificacao;
		msg.dados.valor = valor;
        printf("Vou enviar mensagem do tipo 4.\n");
		status2 = msgsnd( idFilaMensagem, &msg, sizeof(msg.dados), 0);
        printf("Acabei de enviar mensagem do tipo 4.\n");
		exit_on_error (status2, "Erro envio");
}

}
