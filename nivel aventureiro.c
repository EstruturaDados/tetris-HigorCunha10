#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILA 5
#define MAX_PILHA 3

// Estrutura do elemento Peca
typedef struct {
    char nome;
    int id;
} Peca;

// Estrutura de Fila Circular
typedef struct {
    Peca itens[MAX_FILA];
    int inicio;
    int fim;
    int total;
} FilaPecas;

// Estrutura de Pilha Linear
typedef struct {
    Peca itens[MAX_PILHA];
    int topo;
} PilhaReserva;

// Contador global para IDs unicos
int contadorID = 0;

// Gera pecas automaticamente
Peca gerarPeca() {
    Peca p;
    char tipos[] = {'I', 'O', 'T', 'L'};
    p.nome = tipos[rand() % 4];
    p.id = contadorID++;
    return p;
}

// Inicializacao das estruturas
void inicializarFila(FilaPecas *f) {
    f->inicio = 0;
    f->fim = 0;
    f->total = 0;
}

void inicializarPilha(PilhaReserva *p) {
    p->topo = -1;
}

// Funcoes de suporte para Fila
void enqueue(FilaPecas *f, Peca p) {
    if (f->total < MAX_FILA) {
        f->itens[f->fim] = p;
        f->fim = (f->fim + 1) % MAX_FILA;
        f->total++;
    }
}

Peca dequeue(FilaPecas *f) {
    Peca p = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % MAX_FILA;
    f->total--;
    return p;
}

// Funcoes de suporte para Pilha
int pilhaCheia(PilhaReserva *p) {
    return p->topo == MAX_PILHA - 1;
}

int pilhaVazia(PilhaReserva *p) {
    return p->topo == -1;
}

void push(PilhaReserva *p, Peca peca) {
    p->topo++;
    p->itens[p->topo] = peca;
}

Peca pop(PilhaReserva *p) {
    Peca peca = p->itens[p->topo];
    p->topo--;
    return peca;
}

// Exibicao dos estados
void exibirEstado(FilaPecas *f, PilhaReserva *p) {
    printf("\n--- ESTADO ATUAL ---\n");
    
    // Mostra Fila Circular
    printf("Fila de pecas: ");
    for (int i = 0; i < f->total; i++) {
        int idx = (f->inicio + i) % MAX_FILA;
        printf("[%c %d] ", f->itens[idx].nome, f->itens[idx].id);
    }

    // Mostra Pilha de Reserva (Topo para Base)
    printf("\nPilha de reserva (Topo -> Base): ");
    if (pilhaVazia(p)) {
        printf("[Vazia]");
    } else {
        for (int i = p->topo; i >= 0; i--) {
            printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
        }
    }
    printf("\n--------------------\n");
}

int main() {
    srand(time(NULL));
    FilaPecas fila;
    PilhaReserva pilha;
    int opcao;

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Preenchimento inicial da fila
    for (int i = 0; i < MAX_FILA; i++) {
        enqueue(&fila, gerarPeca());
    }

    do {
        exibirEstado(&fila, &pilha);
        printf("\nOpcoes de Acao:");
        printf("\n1 - Jogar peca (Remove da fila)");
        printf("\n2 - Reservar peca (Fila -> Pilha)");
        printf("\n3 - Usar peca reservada (Remove da pilha)");
        printf("\n0 - Sair");
        printf("\nOpcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                // Jogar peca: remove da fila e gera nova
                {
                    Peca p = dequeue(&fila);
                    printf("\nVoce jogou a peca [%c %d].\n", p.nome, p.id);
                    enqueue(&fila, gerarPeca());
                }
                break;

            case 2:
                // Reservar: move da fila para pilha
                if (pilhaCheia(&pilha)) {
                    printf("\n[Erro] Pilha de reserva cheia!\n");
                } else {
                    Peca p = dequeue(&fila);
                    push(&pilha, p);
                    printf("\nPeca [%c %d] movida para a reserva.\n", p.nome, p.id);
                    enqueue(&fila, gerarPeca());
                }
                break;

            case 3:
                // Usar reservada: apenas remove da pilha
                if (pilhaVazia(&pilha)) {
                    printf("\n[Erro] Nao ha pecas na reserva!\n");
                } else {
                    Peca p = pop(&pilha);
                    printf("\nVoce usou a peca reservada [%c %d].\n", p.nome, p.id);
                }
                break;

            case 0:
                printf("\nSaindo...\n");
                break;

            default:
                printf("\nOpcao invalida!\n");
        }
    } while (opcao != 0);

    return 0;
}