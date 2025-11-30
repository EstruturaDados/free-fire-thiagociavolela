/*
    ============================================================================
                     DESAFIO CÓDIGO DA ILHA – EDIÇÃO FREE FIRE
                         Nível Aventureiro: Mochila com Busca
    ============================================================================
    
    Objetivo:
    - Evoluir a mochila do nível novato adicionando busca por nome.
    - Permitir localizar um item e exibir seus detalhes.

    Funcionalidades Adicionadas:
    ✔ Busca sequencial por nome (strcmp)
    ✔ Nova opção no menu: Buscar item
    ✔ Exibição completa do item encontrado
    ✔ Mensagem amigável quando não encontra
    
    Conceitos utilizados:
    - Vetor estático
    - struct
    - strcmp
    - flag de controle
    - menu interativo
    ============================================================================
*/

#include <stdio.h>
#include <string.h>

#define MAX_ITENS 10

// ============================================================================
// ESTRUTURA DO ITEM DA MOCHILA
// ============================================================================
typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
    int ativo;
} Item;

// ============================================================================
// PROTÓTIPOS
// ============================================================================
void adicionarItem(Item mochila[]);
void removerItem(Item mochila[]);
void listarItens(Item mochila[]);
void buscarItem(Item mochila[]);
void limparBuffer();

// ============================================================================
// FUNÇÃO PRINCIPAL
// ============================================================================
int main() {
    Item mochila[MAX_ITENS] = {0};

    int opcao;

    do {
        printf("\n=============== MOCHILA DO SOBREVIVENTE ===============\n");
        printf("1 - Adicionar item\n");
        printf("2 - Remover item\n");
        printf("3 - Listar itens\n");
        printf("4 - Buscar item por nome\n");
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
            case 4:
                buscarItem(mochila);
                break;
            case 0:
                printf("\nSaindo do sistema...\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}

// ============================================================================
// ADICIONAR ITEM
// ============================================================================
void adicionarItem(Item mochila[]) {
    int pos = -1;

    for (int i = 0; i < MAX_ITENS; i++) {
        if (mochila[i].ativo == 0) {
            pos = i;
            break;
        }
    }

    if (pos == -1) {
        printf("\nA mochila está cheia! Não é possível adicionar mais itens.\n");
        return;
    }

    printf("\n--- Adicionar Item ---\n");
    printf("Nome do item: ");
    fgets(mochila[pos].nome, sizeof(mochila[pos].nome), stdin);
    mochila[pos].nome[strcspn(mochila[pos].nome, "\n")] = '\0';

    printf("Tipo do item: ");
    fgets(mochila[pos].tipo, sizeof(mochila[pos].tipo), stdin);
    mochila[pos].tipo[strcspn(mochila[pos].tipo, "\n")] = '\0';

    printf("Quantidade: ");
    scanf("%d", &mochila[pos].quantidade);
    limparBuffer();

    mochila[pos].ativo = 1;

    printf("Item adicionado com sucesso!\n");
}

// ============================================================================
// REMOVER ITEM
// ============================================================================
void removerItem(Item mochila[]) {
    char nomeBusca[30];

    printf("\n--- Remover Item ---\n");
    printf("Nome do item a remover: ");
    fgets(nomeBusca, sizeof(nomeBusca), stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

    for (int i = 0; i < MAX_ITENS; i++) {
        if (mochila[i].ativo == 1 && strcmp(mochila[i].nome, nomeBusca) == 0) {
            mochila[i].ativo = 0;
            printf("Item removido com sucesso!\n");
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

    int encontrou = 0;

    for (int i = 0; i < MAX_ITENS; i++) {
        if (mochila[i].ativo == 1) {
            encontrou = 1;
            printf("\nSlot %d:\n", i);
            printf("Nome: %s\n", mochila[i].nome);
            printf("Tipo: %s\n", mochila[i].tipo);
            printf("Quantidade: %d\n", mochila[i].quantidade);
        }
    }

    if (!encontrou)
        printf("A mochila está vazia.\n");

    printf("\n========================================\n");
}

// ============================================================================
// BUSCAR ITEM POR NOME (Nível Aventureiro)
// ============================================================================
void buscarItem(Item mochila[]) {
    char nomeBusca[30];
    int encontrado = 0;

    printf("\n--- Buscar Item ---\n");
    printf("Digite o nome do item: ");
    fgets(nomeBusca, sizeof(nomeBusca), stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

    for (int i = 0; i < MAX_ITENS; i++) {
        if (mochila[i].ativo == 1 && strcmp(mochila[i].nome, nomeBusca) == 0) {
            encontrado = 1;

            printf("\nItem encontrado!\n");
            printf("Nome: %s\n", mochila[i].nome);
            printf("Tipo: %s\n", mochila[i].tipo);
            printf("Quantidade: %d\n", mochila[i].quantidade);
            return;
        }
    }

    if (!encontrado)
        printf("\n⚠️  Item não encontrado na mochila.\n");
}

// ============================================================================
// LIMPAR BUFFER
// ============================================================================
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
