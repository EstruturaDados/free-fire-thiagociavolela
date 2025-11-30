/*
    ============================================================================
                     DESAFIO CÓDIGO DA ILHA – EDIÇÃO FREE FIRE
                              Nível Mestre: Ordenação e Busca Binária
    ============================================================================
    
    Objetivo:
    - Evoluir a mochila virtual adicionando ordenação e busca binária.
    - Implementar prioridades, enum, insertion sort e binary search.

    Funcionalidades:
    ✔ Campo prioridade (1 a 5)
    ✔ Ordenação: nome, tipo ou prioridade
    ✔ Contador de comparações para análise de desempenho
    ✔ Busca binária por nome (apenas se estiver ordenado por nome)
    ✔ Menu acadêmico completo

    Conceitos Utilizados:
    - struct com novos campos
    - enum para critérios de ordenação
    - insertion sort
    - binary search
    - bool (controle de ordenação)
    - vetores estáticos

    ============================================================================
*/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_ITENS 10

// ============================================================================
// ESTRUTURA DO ITEM
// ============================================================================
typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
    int prioridade;     // 1 a 5 (novo)
    int ativo;
} Item;

// ============================================================================
// ENUM PARA CRITÉRIO DE ORDENAÇÃO
// ============================================================================
typedef enum {
    ORDENAR_NOME = 1,
    ORDENAR_TIPO = 2,
    ORDENAR_PRIORIDADE = 3
} Ordenacao;

// ============================================================================
// PROTÓTIPOS
// ============================================================================
void adicionarItem(Item mochila[]);
void removerItem(Item mochila[]);
void listarItens(Item mochila[]);
void buscarItemSequencial(Item mochila[]);
void limparBuffer();

void ordenarMochila(Item mochila[], Ordenacao criterio);
int comparacoesInsertion;
int comparar(Item a, Item b, Ordenacao criterio);

int buscarBinaria(Item mochila[], int tamanho, const char* nome);
void buscarPorNomeBinaria(Item mochila[]);

bool estaOrdenadaPorNome(Item mochila[]);

// ============================================================================
// FUNÇÃO PRINCIPAL
// ============================================================================
int main() {
    Item mochila[MAX_ITENS] = {0};
    int opcao;

    do {
        printf("\n=============== MOCHILA DO SOBREVIVENTE – NÍVEL MESTRE ===============\n");
        printf("1 - Adicionar item\n");
        printf("2 - Remover item\n");
        printf("3 - Listar itens\n");
        printf("4 - Ordenar mochila\n");
        printf("5 - Busca binária por nome\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1:
                adicionarItem(mochila);
                break;
            case 2:
                removerItem(mochila);
                break;
            case 3:
                listarItens(mochila);
                break;
            case 4: {
                int crit;
                printf("\nEscolha o critério de ordenação:\n");
                printf("1 - Nome\n");
                printf("2 - Tipo\n");
                printf("3 - Prioridade\n");
                printf("Escolha: ");
                scanf("%d", &crit);
                limparBuffer();

                if (crit < 1 || crit > 3) {
                    printf("Critério inválido!\n");
                    break;
                }

                comparacoesInsertion = 0;
                ordenarMochila(mochila, (Ordenacao)crit);

                printf("\nMochila ordenada com sucesso!\n");
                printf("Comparações realizadas: %d\n", comparacoesInsertion);
                break;
            }

            case 5:
                buscarPorNomeBinaria(mochila);
                break;

            case 0:
                printf("\nSaindo do sistema...\n");
                break;

            default:
                printf("Opção inválida.\n");
        }

    } while (opcao != 0);

    return 0;
}

// ============================================================================
// ADICIONAR ITEM (com prioridade)
// ============================================================================
void adicionarItem(Item mochila[]) {
    int pos = -1;

    for (int i = 0; i < MAX_ITENS; i++) {
        if (!mochila[i].ativo) {
            pos = i;
            break;
        }
    }

    if (pos == -1) {
        printf("A mochila está cheia!\n");
        return;
    }

    printf("\n--- Adicionar Item ---\n");

    printf("Nome: ");
    fgets(mochila[pos].nome, 30, stdin);
    mochila[pos].nome[strcspn(mochila[pos].nome, "\n")] = '\0';

    printf("Tipo: ");
    fgets(mochila[pos].tipo, 20, stdin);
    mochila[pos].tipo[strcspn(mochila[pos].tipo, "\n")] = '\0';

    printf("Quantidade: ");
    scanf("%d", &mochila[pos].quantidade);
    limparBuffer();

    do {
        printf("Prioridade (1 a 5): ");
        scanf("%d", &mochila[pos].prioridade);
        limparBuffer();
    } while (mochila[pos].prioridade < 1 || mochila[pos].prioridade > 5);

    mochila[pos].ativo = 1;

    printf("Item adicionado!\n");
}

// ============================================================================
// REMOVER ITEM
// ============================================================================
void removerItem(Item mochila[]) {
    char nomeBusca[30];

    printf("\nNome do item a remover: ");
    fgets(nomeBusca, 30, stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

    for (int i = 0; i < MAX_ITENS; i++) {
        if (mochila[i].ativo && strcmp(mochila[i].nome, nomeBusca) == 0) {
            mochila[i].ativo = 0;
            printf("Item removido!\n");
            return;
        }
    }

    printf("Item não encontrado!\n");
}

// ============================================================================
// LISTAR ITENS
// ============================================================================
void listarItens(Item mochila[]) {
    printf("\n=========== ITENS NA MOCHILA ===========\n");
    bool encontrou = false;

    for (int i = 0; i < MAX_ITENS; i++) {
        if (mochila[i].ativo) {
            encontrou = true;
            printf("\nSlot %d:\n", i);
            printf("Nome: %s\n", mochila[i].nome);
            printf("Tipo: %s\n", mochila[i].tipo);
            printf("Quantidade: %d\n", mochila[i].quantidade);
            printf("Prioridade: %d\n", mochila[i].prioridade);
        }
    }

    if (!encontrou)
        printf("A mochila está vazia!");

    printf("\n========================================\n");
}

// ============================================================================
// FUNÇÃO DE COMPARAÇÃO ENTRE ITENS
// ============================================================================
int comparar(Item a, Item b, Ordenacao criterio) {
    switch (criterio) {
        case ORDENAR_NOME:
            return strcmp(a.nome, b.nome);
        case ORDENAR_TIPO:
            return strcmp(a.tipo, b.tipo);
        case ORDENAR_PRIORIDADE:
            return a.prioridade - b.prioridade;
    }
    return 0;
}

// ============================================================================
// INSERTION SORT COM CONTADOR DE COMPARAÇÕES
// ============================================================================
void ordenarMochila(Item mochila[], Ordenacao criterio) {
    for (int i = 1; i < MAX_ITENS; i++) {
        Item atual = mochila[i];
        int j = i - 1;

        while (j >= 0 && mochila[j].ativo &&
               comparar(mochila[j], atual, criterio) > 0) {

            comparacoesInsertion++;
            mochila[j + 1] = mochila[j];
            j--;
        }

        mochila[j + 1] = atual;
    }
}

// ============================================================================
// VERIFICA SE ESTÁ ORDENADO POR NOME
// ============================================================================
bool estaOrdenadaPorNome(Item mochila[]) {
    for (int i = 0; i < MAX_ITENS - 1; i++) {
        if (mochila[i].ativo && mochila[i + 1].ativo) {
            if (strcmp(mochila[i].nome, mochila[i + 1].nome) > 0)
                return false;
        }
    }
    return true;
}

// ============================================================================
// BUSCA BINÁRIA POR NOME
// ============================================================================
int buscarBinaria(Item mochila[], int tamanho, const char* nome) {
    int esquerda = 0, direita = tamanho - 1;

    while (esquerda <= direita) {
        int meio = (esquerda + direita) / 2;

        if (!mochila[meio].ativo) {
            esquerda++;
            continue;
        }

        int cmp = strcmp(mochila[meio].nome, nome);

        if (cmp == 0)
            return meio;
        else if (cmp < 0)
            esquerda = meio + 1;
        else
            direita = meio - 1;
    }

    return -1;
}

// ============================================================================
// FUNÇÃO DO MENU: BUSCAR POR NOME BINÁRIA
// ============================================================================
void buscarPorNomeBinaria(Item mochila[]) {
    char nomeBusca[30];

    if (!estaOrdenadaPorNome(mochila)) {
        printf("\n⚠️  A mochila precisa estar ORDENADA POR NOME para usar busca binária!\n");
        return;
    }

    printf("\n--- Busca Binária ---\n");
    printf("Nome do item: ");
    fgets(nomeBusca, 30, stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

    int pos = buscarBinaria(mochila, MAX_ITENS, nomeBusca);

    if (pos == -1 || !mochila[pos].ativo) {
        printf("\nItem NÃO encontrado!\n");
    } else {
        printf("\nItem encontrado!\n");
        printf("Nome: %s\n", mochila[pos].nome);
        printf("Tipo: %s\n", mochila[pos].tipo);
        printf("Quantidade: %d\n", mochila[pos].quantidade);
        printf("Prioridade: %d\n", mochila[pos].prioridade);
    }
}

// ============================================================================
// LIMPAR BUFFER
// ============================================================================
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
