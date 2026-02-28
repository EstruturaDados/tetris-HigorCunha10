#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILA 5
#define MAX_PILHA 3

typedef struct {
    char nome;
    int id;
} Peca;

typedef struct {
    Peca itens[MAX_FILA];
    int inicio, fim, total;
} FilaCircular;

typedef struct {
    Peca itens[MAX_PILHA];
    int topo;
} PilhaReserva;

int contadorID = 0;

// Gera pecas aleatorias com IDs exclusivos
Peca gerarPeca() {
    Peca p;
    char tipos[] = {'I', 'O', 'T', 'L'};
    p.nome = tipos[rand() % 4];
    p.id = contadorID++;
    return p;
}

void inicializarFila(FilaCircular *f) {
    f->inicio = 0;
    f->fim = 0;
    f->total = 0;
}

void inicializarPilha(PilhaReserva *p) {
    p->topo = -1;
}

// Funcoes de suporte para Fila
void enqueue(FilaCircular *f, Peca p) {
    if (f->total < MAX_FILA) {
        f->itens[f->fim] = p;
        f->fim = (f->fim + 1) % MAX_FILA;
        f->total++;
    }
}

Peca dequeue(FilaCircular *f) {
    Peca p = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % MAX_FILA;
    f->total--;
    return p;
}

// Funcoes de suporte para Pilha
void push(PilhaReserva *p, Peca peca) {
    if (p->topo < MAX_PILHA - 1) {
        p->itens[++p->topo] = peca;
    }
}

Peca pop(PilhaReserva *p) {
    return p->itens[p->topo--];
}

// Exibe o estado das estruturas no console
void exibirEstado(FilaCircular *f, PilhaReserva *p) {
    printf("\n--- ESTADO ATUAL ---\n");
    printf("Fila de pecas: ");
    for (int i = 0; i < f->total; i++) {
        int idx = (f->inicio + i) % MAX_FILA;
        printf("[%c %d] ", f->itens[idx].nome, f->itens[idx].id);
    }
    printf("\nPilha de reserva (Topo -> Base): ");
    if (p->topo == -1) printf("[Vazia]");
    else {
        for (int i = p->topo; i >= 0; i--) {
            printf("[%c %d] ", p->itens[i].nome, p->itens[i].id);
        }
    }
    printf("\n--------------------\n");
}

int main() {
    srand(time(NULL));
    FilaCircular fila;
    PilhaReserva pilha;
    int opcao;

    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Inicializa fila completa
    for (int i = 0; i < MAX_FILA; i++) enqueue(&fila, gerarPeca());

    do {
        exibirEstado(&fila, &pilha);
        printf("\n1-Jogar 2-Reservar 3-Usar Reserva 4-Trocar Atual 5-Troca Multipla (3) 0-Sair\nOpcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: // Jogar peca
                if (fila.total > 0) {
                    Peca p = dequeue(&fila);
                    printf("\nJogou: [%c %d]\n", p.nome, p.id);
                    enqueue(&fila, gerarPeca());
                }
                break;

            case 2: // Reservar peca
                if (pilha.topo < MAX_PILHA - 1 && fila.total > 0) {
                    push(&pilha, dequeue(&fila));
                    enqueue(&fila, gerarPeca());
                } else printf("\nPilha cheia ou fila vazia!\n");
                break;

            case 3: // Usar peca da reserva
                if (pilha.topo != -1) {
                    Peca p = pop(&pilha);
                    printf("\nUsou reserva: [%c %d]\n", p.nome, p.id);
                } else printf("\nReserva vazia!\n");
                break;

            case 4: // Trocar peca da frente com topo da pilha
                if (fila.total > 0 && pilha.topo != -1) {
                    Peca temp = fila.itens[fila.inicio];
                    fila.itens[fila.inicio] = pilha.itens[pilha.topo];
                    pilha.itens[pilha.topo] = temp;
                    printf("\nTroca simples realizada!\n");
                }
                break;

            case 5: // Troca múltipla (3 primeiras peças)
                if (fila.total >= 3 && pilha.topo == 2) {
                    for (int i = 0; i < 3; i++) {
                        int idxFila = (fila.inicio + i) % MAX_FILA;
                        int idxPilha = 2 - i; // Topo para base
                        Peca temp = fila.itens[idxFila];
                        fila.itens[idxFila] = pilha.itens[idxPilha];
                        pilha.itens[idxPilha] = temp;
                    }
                    printf("\nTroca múltipla concluida!\n");
                } else printf("\nNecessario 3 pecas em ambas estruturas!\n");
                break;
        }
    } while (opcao != 0);

    return 0;
};
