#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 5

// Estrutura que representa uma peça do jogo
typedef struct {
    char nome; 
    int id;    
} Peca;

// Estrutura que controla a fila circular
typedef struct {
    Peca itens[MAX];
    int inicio;
    int fim;
    int total;
} FilaPecas;

// Contador global para gerar IDs exclusivos
int contadorID = 0;

// Gera uma peça aleatória entre os tipos do Tetris
Peca gerarPeca() {
    Peca nova;
    char tipos[] = {'I', 'O', 'T', 'L'};
    nova.nome = tipos[rand() % 4];
    nova.id = contadorID++;
    return nova;
}

void inicializarFila(FilaPecas *f) {
    f->inicio = 0;
    f->fim = 0;
    f->total = 0;
}

int estaCheia(FilaPecas *f) {
    return f->total == MAX;
}

int estaVazia(FilaPecas *f) {
    return f->total == 0;
}

// Adiciona uma peça ao final da fila (Enqueue)
void inserirPeca(FilaPecas *f) {
    if (estaCheia(f)) {
        printf("\n[Erro] A fila de proximas pecas esta cheia!\n");
        return;
    }
    f->itens[f->fim] = gerarPeca();
    f->fim = (f->fim + 1) % MAX; // Logica circular
    f->total++;
}

// Remove a peça da frente para o jogador usar (Dequeue)
void jogarPeca(FilaPecas *f) {
    if (estaVazia(f)) {
        printf("\n[Erro] Nao ha pecas na fila para jogar!\n");
        return;
    }
    Peca p = f->itens[f->inicio];
    printf("\nVocé jogou a peca [%c] (ID: %d)\n", p.nome, p.id);
    f->inicio = (f->inicio + 1) % MAX; // Logica circular
    f->total--;
}

// Exibe o estado atual da fila no console
void exibirFila(FilaPecas *f) {
    printf("\nFila de pecas: ");
    if (estaVazia(f)) {
        printf("[Vazia]");
    } else {
        for (int i = 0; i < f->total; i++) {
            int idx = (f->inicio + i) % MAX;
            printf("[%c %d] ", f->itens[idx].nome, f->itens[idx].id);
        }
    }
    printf("\n");
}

int main() {
    FilaPecas fila;
    int opcao;

    inicializarFila(&fila);

    // Preenche a fila inicial conforme requisito
    for (int i = 0; i < MAX; i++) {
        inserirPeca(&fila);
    }

    do {
        exibirFila(&fila);
        printf("\nOpcoes de acao:");
        printf("\n1 - Jogar peca (dequeue)");
        printf("\n2 - Inserir nova peca (enqueue)");
        printf("\n0 - Sair");
        printf("\nEscolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                jogarPeca(&fila);
                break;
            case 2:
                inserirPeca(&fila);
                break;
            case 0:
                printf("Saindo do Tetris Stack...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 0);

    return 0;
};
