#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NAME_COLON_OCCURRENCES 1
#define POINTS_COLON_OCCURRENCES 3
#define TRIPVALUE_COLON_OCCURRENCES 4

void getNome(char s[],char *name){
    int cont=0;
    for(int i=0; i!= strlen(s)-1; i++){
        if(s[i] == ':'){
            char aux[50];
            int index=0;
            cont++;
            for(int j=i+1;s[j]!=':'&&j!=strlen(s)-1;j++){
                aux[index]=s[j];
                index++;
            }
            if(cont == NAME_COLON_OCCURRENCES){ 
                strcpy(name,aux);
            }
        }
    }

}

void getPontos(char s[],char *name){
    int cont=0;
    for(int i=0; i!= strlen(s); i++){
        if(s[i] == ':'){
            char aux[5];
            int index=0;
            cont++;
            for(int j=i+1;s[j]!=':'&&j!=strlen(s);j++){
				if(cont==POINTS_COLON_OCCURRENCES){
					aux[index]=s[j];
					index++;
				}
            }
            if(cont == 3){ 
                strcpy(name,aux);
            }
        }
    }

}

void getValorViagem(char s[],char *name){
    int cont=0;
    for(int i=0; i!= strlen(s); i++){
        if(s[i] == ':'){
            char aux[10];
            int index=0;
            cont++;
            for(int j=i+1;s[j]!=':'&&j!=strlen(s);j++){
                aux[index]=s[j];
                index++;
            }
            if(cont == TRIPVALUE_COLON_OCCURRENCES){ 
                strcpy(name,aux);
            }
        }
    }

}
