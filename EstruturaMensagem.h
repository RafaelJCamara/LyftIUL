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
