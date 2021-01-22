#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "Estruturas.h"
#include "getters.h"
#include <time.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#define exit_on_error(s,m) if ( s < 0 ) { perror(m); exit(1); }
#define exit_on_null(s,m) if (s==NULL) { perror(m); exit(1); }




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


void inicializarVetores(){
	int dimPassageiro=numberLinesFilePassageiro();
	int dimCondutor=numberLinesFile();
	driverArray=(Tcondutor *)malloc(dimCondutor*50 + sizeof(char)*140 + sizeof(int)*200);
	passengerArray=(Tpassageiro *)malloc(dimPassageiro*5 + sizeof(char)*140 + sizeof(int)*200);
}


void atualizaFicheiroCondutor(){
	int numero=numberLinesFile();
        FILE *fptr;
    	    fptr = fopen("atualizarCondutores.txt", "a");
   		    if(fptr == NULL){ //if file does not exist, create it
        	    freopen("atualizarCondutores.txt", "a", fptr);
    	    } 
        for(int i=0;i!=numero;i++){
		    fprintf(fptr,"%d:%s:%s:%s:%s%s:%s%s:%d:%d:%g\n",driverArray[i].numero,driverArray[i].nome,driverArray[i].turma,driverArray[i].telemovel,driverArray[i].email,driverArray[i].tipo,driverArray[i].marca,driverArray[i].matricula,driverArray[i].viagens,driverArray[i].pontos,driverArray[i].saldo);
        }        
        fclose(fptr);
}


void atualizaFicheiroPassageiro(){
	int numero=numberLinesFilePassageiro();
        FILE *fptr;
    	    fptr = fopen("atualizarPassageiros.txt", "a");
   		    if(fptr == NULL){ //if file does not exist, create it
        	    freopen("atualizarPassageiros.txt", "a", fptr);
    	    } 
        for(int i=0;i!=numero;i++){
		    fprintf(fptr,"%d:%s:%s:%s:%s:%s\n",passengerArray[i].numero,passengerArray[i].nome,passengerArray[i].turma,passengerArray[i].telemovel,passengerArray[i].email,passengerArray[i].c_credito);
        }        
        fclose(fptr);
}


void atualizaFicheiro(){
	atualizaFicheiroCondutor();
	atualizaFicheiroPassageiro();
	int dCF=remove("condutores.txt");
	int dPF=remove("passageiros.txt");

FILE *fptr1, *fptr2; 
char c;
fptr1 = fopen("atualizarCondutores.txt", "r");
fptr2 = fopen("condutores.txt", "a"); 
 c = fgetc(fptr1); 
    while (c != EOF) { 
        fputc(c, fptr2); 
        c = fgetc(fptr1); 
    } 
fclose(fptr1); 
fclose(fptr2);
int dAC=remove("atualizarCondutores.txt");

FILE *fptr3, *fptr4; 
char d;
fptr3 = fopen("atualizarPassageiros.txt", "r");
fptr4 = fopen("passageiros.txt", "a"); 
 d = fgetc(fptr3); 
    while (d != EOF) { 
        fputc(d, fptr4); 
        d = fgetc(fptr3); 
    } 
fclose(fptr3); 
fclose(fptr4);
int dAP=remove("atualizarPassageiros.txt");
}


void atualizaEstruturas(){
	int numero=numberLinesFile();
	for(int i=0;i!=numero;i++){
		FILE *condutores;
		char s1[100];
		condutores = fopen ("condutores.txt", "r");
		while ( fgets (s1, 100, condutores ) != NULL ) {
			char *numero_estudante= strtok(s1,":");
 			char *nomes=strtok(NULL,":");
			char *turmas= strtok (NULL,":");
 			char *telemovels=strtok(NULL,":");
			char *emails= strtok (NULL,":");
 			char *tipo_viatura=strtok(NULL,":");
			char *marcas= strtok (NULL,":");
 			char *matriculas=strtok(NULL,":");
			char *numero_viagens= strtok(NULL,":");
 			char *pontoss=strtok(NULL,":");
			char *saldoViagems=strtok(NULL,":");
			char auxNumero[50];
			strcpy(auxNumero,numero_estudante);
			int numeroFinal=atoi(auxNumero);
			if(numeroFinal == driverArray[i].numero){
				
				char auxViagens[10];
				strcpy(auxViagens,numero_viagens);
				int viagensFinal=atoi(auxViagens);
				driverArray[i].viagens=viagensFinal;
				
				char auxPontos[10];
				strcpy(auxPontos,pontoss);
				int pontosFinal=atoi(auxPontos);
				driverArray[i].pontos=pontosFinal;

				char auxSaldo[10];
				strcpy(auxSaldo,saldoViagems);
				float saldoFinal=atof(auxSaldo);
				driverArray[i].saldo=saldoFinal;
			}
		}
		fclose ( condutores );
	}
	printf("\n");
}

void inserirCondutoresEstrutura(){
FILE *condutores;
		char s1[100];
		condutores = fopen ("condutores.txt", "r");
		int index=0;
		while ( fgets (s1, 100, condutores ) != NULL ) {
			Tcondutor driver;
			driverArray[index]=driver;
			char *numero_estudante= strtok(s1,":");
 			char *nomes=strtok(NULL,":");
			char *turmas= strtok (NULL,":");
 			char *telemovels=strtok(NULL,":");
			char *emails= strtok (NULL,":");
 			char *tipo_viatura=strtok(NULL,":");
			char *marcas= strtok (NULL,":");
 			char *matriculas=strtok(NULL,":");
			char *numero_viagens= strtok(NULL,":");
 			char *pontoss=strtok(NULL,":");
			char *saldoViagems=strtok(NULL,":");

			char auxNumero[50];
			strcpy(auxNumero,numero_estudante);
			int numeroFinal=atoi(auxNumero);
			driverArray[index].numero=numeroFinal;

			char auxNome[10];
			strcpy(auxNome,nomes);
			strcpy ( driverArray[index].nome , auxNome );

			char auxTurma[10];
			strcpy(auxTurma,turmas);
			strcpy ( driverArray[index].turma , auxTurma );

			char auxTelemovel[15];
			strcpy(auxTelemovel,telemovels);
			strcpy ( driverArray[index].telemovel , auxTelemovel );

			char auxEmail[40];
			strcpy(auxEmail,emails);
			strcpy ( driverArray[index].email , auxEmail );

			char auxTipo[20];
			strcpy(auxTipo,tipo_viatura);
			strcpy ( driverArray[index].tipo , auxTipo );

			char auxMarca[20];
			strcpy(auxMarca,marcas);
			strcpy ( driverArray[index].marca , auxMarca );

			char auxMatricula[15];
			strcpy(auxMatricula,matriculas);
			strcpy ( driverArray[index].matricula , auxMatricula );

			char auxViagens[10];
			strcpy(auxViagens,numero_viagens);
			int viagensFinal=atoi(auxViagens);
			driverArray[index].viagens=viagensFinal;

			char auxPontos[10];
			strcpy(auxPontos,pontoss);
			int pontosFinal=atoi(auxPontos);
			driverArray[index].pontos=pontosFinal;

			char auxSaldo[10];
			strcpy(auxSaldo,saldoViagems);
			float saldoFinal=atof(auxSaldo);
			driverArray[index].saldo=saldoFinal;
			
			driverArray[index].PID=getpid();

			index++;
		}
printf("\n");
fclose ( condutores );
}

void inserirPassageirosEstrutura(){
 FILE *f = fopen("passageiros.txt","r");
        char fileLine[150];
		int index = 0;
            while (fgets(fileLine,150,f) != NULL ){
				Tpassageiro pass;
                char *a  = strtok(fileLine,":");
				char auxNum[10];
				strcpy(auxNum,a);
                pass.numero = atoi(auxNum);

				char *aNome = strtok(NULL,":");
				char auxNome[50];
				strcpy(auxNome,aNome);
				strcpy(pass.nome,auxNome);
				
				char *aTurma = strtok(NULL,":");
                char auxTurma[10];
                strcpy(auxTurma,aTurma);
                strcpy(pass.turma,auxTurma);

				char *aTelemovel = strtok(NULL,":");
                char auxTelemovel[15];
                strcpy(auxTelemovel,aTelemovel);
                strcpy(pass.telemovel,auxTelemovel);

				char *aEmail = strtok(NULL,":");
                char auxEmail[40];
                strcpy(auxEmail,aEmail);
                strcpy(pass.email,auxEmail);

				char *aCartao = strtok(NULL,":");
                char auxCartao[20];
                strcpy(auxCartao,aCartao);
                strcpy(pass.c_credito,auxCartao);
				
				passengerArray[index] = pass;
				index++;
	}
fclose(f);
}


void PID_operation(){
	FILE *ficheiro = fopen("lyftadmin.pid","w");
	fprintf(ficheiro,"%d\n",getpid());
    fclose(ficheiro);
}

void inserirElementosEstruturas(){
	inicializarVetores();
	inserirCondutoresEstrutura();
	inserirPassageirosEstrutura();
	PID_operation();
}



void imprimirMemoria(){
    int count;
    for(count = 0; count < numberLinesFilePassageiro(); count++){
   		printf("%d:", passengerArray[count].numero); //numero de estudante
		printf("%s:", passengerArray[count].nome); //nome
		printf("%s:", passengerArray[count].turma); //turma
		printf("%s:", passengerArray[count].telemovel); //telemovel
		printf("%s:", passengerArray[count].email); //email
		printf("%s:\n",passengerArray[count].c_credito); //cartão de crédito
	}

	printf("Número total de passageios existentes: %d\n",numberLinesFilePassageiro());

	for(count = 0; count < numberLinesFile(); count++){
		printf("%d:", driverArray[count].numero); //num_aluno
		printf("%s:", driverArray[count].nome); //nome
		printf("%s:", driverArray[count].turma); //turma
		printf("%s:", driverArray[count].telemovel); //telemovel
		printf("%s:", driverArray[count].email); //email
    	printf("%s:", driverArray[count].tipo); //tipo de viatura
		printf("%s:", driverArray[count].marca); //marca
		printf("%s:", driverArray[count].matricula); //matricula
		printf("%d:", driverArray[count].viagens); //numero de viagens
    	printf("%d:", driverArray[count].pontos); //pontos
		printf("%g:\n", driverArray[count].saldo); //saldo de viagens
	}
	printf("Número total de condutores existentes: %d\n",numberLinesFile());
}



void alterarPassageiro(int numeroEstudante){
	char line[50];
 	int count;
  	int var=0;
	for(count = 0; count < numberLinesFilePassageiro() &&var!=1 ; count++){
		if(passengerArray[count].numero==numeroEstudante){
			printf("ATENÇÃO: Para todos os campos que não pretenda alterar, basta pressionar Enter. \n");
			printf("Insirir novo nome: ");
			fgets(line,50,stdin);
			/*line[strlen(line)-1]='\0';*/
			if(strcmp(line,"\n")!=0){
				strcpy(passengerArray[count].nome,line);			
			}
			printf("Insirir novo turma: ");
			fgets(line,50,stdin);
			if(strcmp(line,"\n")!=0){
				strcpy(passengerArray[count].turma,line);
			}
			printf("Insirir novo telemovel : ");
			fgets(line,50,stdin);
			if(strcmp(line,"\n")!=0){
				strcpy(passengerArray[count].telemovel,line);
			}
			printf("Insirir novo email: ");
			fgets(line,50,stdin);
			if(strcmp(line,"\n")!=0){
				strcpy(passengerArray[count].email,line);
			}
			printf("Insirir novo Cartão de Credito: ");
			fgets(line,50,stdin);
			if(strcmp(line,"\n")!=0){
				strcpy(passengerArray[count].c_credito,line);
			}
			var=1;
		}
	}
	if(var==0){
		printf("Este passageiro não existe. \n");
	}
}

void alterarCondutor(int numeroEstudante){
	inicializarVetores();
	char line[50];
 	int count;
  	int var=0;
	for(count = 0; count < numberLinesFile() && var!=1; count++){
		if(driverArray[count].numero==numeroEstudante){
			printf("Insirir novo nome: ");
			fgets(line,50,stdin);
			if(strcmp(line,"\n")!=0){
				strcpy(driverArray[count].nome,line);			}
			printf("Insirir nova turma: ");
			fgets(line,50,stdin);
			if(strcmp(line,"\n")!=0){
				strcpy(driverArray[count].turma,line);
			}
			printf("Insirir novo numero de telemovel : ");
			fgets(line,50,stdin);
			if(strcmp(line,"\n")!=0){
				strcpy(driverArray[count].telemovel,line);
			}
			printf("Insirir novo email: ");
			fgets(line,50,stdin);
			if(strcmp(line,"\n")!=0){
				strcpy(driverArray[count].email,line);
			}
			printf("Insirir novo tipo de viatura: ");
			fgets(line,50,stdin);
			if(strcmp(line,"\n")!=0){
				strcpy(driverArray[count].tipo,line);
			}
  	  printf("Insirir nova marca de viatura: ");
			fgets(line,50,stdin);
			if(strcmp(line,"\n")!=0){
				strcpy(driverArray[count].marca,line);
			}
  	  printf("Insirir nova matricua: ");
			fgets(line,50,stdin);
			if(strcmp(line,"\n")!=0){
				strcpy(driverArray[count].matricula,line);
			}
			var=1;
		}
	}
	if(var==0) {
		printf("Este condutor não existe. \n");
	}
}










