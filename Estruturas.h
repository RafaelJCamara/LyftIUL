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


typedef struct {
int numero;
char nome[50];
char turma[10];
char telemovel[15];
char email[40];
char c_credito[20];
} Tpassageiro;


Tcondutor *driverArray;
Tpassageiro *passengerArray;
