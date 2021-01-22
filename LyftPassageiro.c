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
#define exit_on_error(s,m); if(s<0){ perror(m), exit(1);}
#define exit_on_null(s,m) if (s==NULL) { perror(m); exit(1); }

typedef struct {
int numero;
char nome[50];
char turma[10];
char telemovel[15];
char email[40];
char c_credito[20];
} Tpassageiro;

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

int numberLinesFilePassageiro(){
FILE *condutores;
		char s1[100];
        int cont=0;
		condutores = fopen ("passageiros.txt", "r");
		while ( fgets (s1, 100, condutores ) != NULL ) {
            cont++;
        }
return cont;
}

Tpassageiro*part;

struct sembuf UP = { 0, 1, 0 } ;
struct sembuf DOWN = { 0, -1, 0 };

int main(){

/*Semaforo*/

    int idSemaforo = semget ( 1000, 1, IPC_CREAT | 0666 );
    exit_on_error (idSemaforo, "Criação/Ligação");
	
/*Fim semaforo*/

	char num[10];
    int status;
    MsgViagem msg;
//Associa memória partilhada ao passageiro


            int sem_id;
            int statusSemaforo;
            sem_id = semget ( 1000, 1, 0600 );
            exit_on_error (sem_id, "Criação/Ligação");
            statusSemaforo = semop ( sem_id, &DOWN, 1 );

    int idMemPass = shmget(89101112,0,0);
    exit_on_error(idMemPass,"Erro no shmget do passageiro.");

    printf("Numero de estudante do passageiro:");
    fgets(num, 10, stdin);
    int numeroPassageiro = atoi(num);
    printf("Numero de passageiro pesquisado: %d\n",numeroPassageiro);
//validar o passageiro
        
        char * mem_addr_pass=shmat(idMemPass,0,0);
        exit_on_null(mem_addr_pass,"Erro shmat do passageiro.");
        Tpassageiro *passPointer=(Tpassageiro *)mem_addr_pass;
        int passPointer_index;
        for(int passPointer_index=0;passPointer_index!=numberLinesFilePassageiro();passPointer_index++){
            if(passPointer[passPointer_index].numero == numeroPassageiro){
                printf("Nome do passageiro encontrado: %s\n",passPointer[passPointer_index].nome);
            }
        }          

 statusSemaforo = semop ( sem_id, &UP, 1 );	
//fim da validação do passageiro

        printf("Qual o Local de encontro:");
		fgets(msg.dados.local_encontro, 100, stdin);
        msg.tipo = 1;
        msg.dados.pid_passageiro = getpid();
        msg.dados.data=time(NULL);
		
        printf("Local de encontro: %s\n",msg.dados.local_encontro);
		
    int idFilaMensagem = msgget ( 2500, 0600 );

	//Enviar mensagem tipo 1
    printf("Sending type 1 message.\n");
    status = msgsnd( idFilaMensagem, &msg, sizeof(msg.dados), 0);
    exit_on_error(status, "Erro de envio da mensagem.")
	printf("Just sent type 1 message.\n");

int status_msg;
	//Espera pela resposta
	printf("Esperando a mensagem tipo 3\n");
	status_msg = msgrcv( idFilaMensagem, &msg, sizeof(msg.dados), getpid(),0);
	printf("PID do condutor: %d\n", msg.dados.pid_condutor);
	exit(0);

}	
