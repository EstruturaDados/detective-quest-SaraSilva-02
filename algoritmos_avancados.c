#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para representar um cômodo (nó da árvore)
typedef struct Sala {
    char nome[50];
    struct Sala *esquerda; // Caminho à esquerda (e)
    struct Sala *direita;  // Caminho à direita (d)
} Sala;

// Protótipos das funções
Sala* criarSala(const char *nome);
void explorarMansao(Sala *salaAtual);
void menuExploracao(Sala *raiz);
void liberarArvore(Sala *no);

// Função principal que constrói o mapa da mansão e inicia o jogo

int main() {
    printf("--- O Mistério da Mansão Blackwood ---\n");
    printf("Bem-vindo, detetive. Seu objetivo é explorar a mansão e descobrir o culpado.\n\n");

    // 1. Construção da Árvore Binária (Mapa da Mansão)
    // A árvore é criada manualmente aqui, simulando a estrutura pré-definida.

    // Nível 0 (Raiz)
    Sala *hall = criarSala("Hall de Entrada");

    // Nível 1
    hall->esquerda = criarSala("Sala de Estar");
    hall->direita = criarSala("Cozinha");

    // Nível 2 - Filhos da Sala de Estar (Esquerda)
    hall->esquerda->esquerda = criarSala("Biblioteca");
    hall->esquerda->direita = criarSala("Jardim de Inverno");

    // Nível 2 - Filhos da Cozinha (Direita)
    hall->direita->esquerda = criarSala("Despensa");
    hall->direita->direita = criarSala("Quarto Principal");

    // Nível 3 - Folhas (Fins de exploração)
    // Biblioteca (Esquerda da Sala de Estar)
    hall->esquerda->esquerda->esquerda = criarSala("Escritório Secreto (Culpado!)");
    // Note: O nó "Escritório Secreto" não terá filhos (é um nó-folha)

    // Jardim de Inverno (Direita da Sala de Estar)
    hall->esquerda->direita->direita = criarSala("Varanda dos Fundos");

    // Despensa (Esquerda da Cozinha)
    hall->direita->esquerda->esquerda = criarSala("Porão Úmido");

    // Quarto Principal (Direita da Cozinha)
    hall->direita->direita->esquerda = criarSala("Banheiro Luxuoso");
    hall->direita->direita->direita = criarSala("Closet Vazio");
    
    // Iniciando o jogo
    menuExploracao(hall);

    // Libera a memória alocada
    liberarArvore(hall);

    return 0;
}

/**
 * @brief Cria um novo nó (sala) na árvore, alocando memória dinamicamente.
 * @param nome O nome da sala a ser criada.
 * @return Ponteiro para a nova sala criada.
 */
Sala* criarSala(const char *nome) {
    Sala *novaSala = (Sala*)malloc(sizeof(Sala));
    if (novaSala == NULL) {
        perror("Erro ao alocar memória para a sala");
        exit(EXIT_FAILURE);
    }
    // Copia o nome para a estrutura
    strncpy(novaSala->nome, nome, sizeof(novaSala->nome) - 1);
    novaSala->nome[sizeof(novaSala->nome) - 1] = '\0'; // Garantir terminação nula
    
    // Inicializa os ponteiros de caminho como nulos
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    
    return novaSala;
}

/**
 * @brief Função recursiva que gerencia a exploração interativa da mansão.
 * @param salaAtual O nó atual (cômodo) em que o jogador se encontra.
 */
void explorarMansao(Sala *salaAtual) {
    // Se o jogador chegou a um local inválido (o que não deve ocorrer se a navegação for correta)
    if (salaAtual == NULL) {
        printf("\n--- Fim da exploração inesperada. ---\n");
        return;
    }

    printf("\nVocê está na: %s\n", salaAtual->nome);

    // Verifica se é um nó-folha (Fim da exploração)
    if (salaAtual->esquerda == NULL && salaAtual->direita == NULL) {
        printf("\n=====================================================\n");
        printf("VOCÊ ALCANÇOU UM CÔMODO FINAL (Nó-Folha): %s\n", salaAtual->nome);
        printf("Exploração encerrada neste caminho.\n");
        printf("=====================================================\n");
        return;
    }

    // Se não for um nó-folha, exibe o menu de opções
    char escolha;
    
    printf("Para onde deseja ir? (e) Esquerda | (d) Direita | (s) Sair do Jogo: ");
    scanf(" %c", &escolha); // Espaço antes de %c ignora caracteres de quebra de linha pendentes

    switch (escolha) {
        case 'e':
        case 'E':
            if (salaAtual->esquerda != NULL) {
                explorarMansao(salaAtual->esquerda); // Chamada recursiva
            } else {
                printf("\nNão há caminho para a esquerda a partir daqui.\n");
                menuExploracao(salaAtual); // Permite nova escolha no mesmo local
            }
            break;

        case 'd':
        case 'D':
            if (salaAtual->direita != NULL) {
                explorarMansao(salaAtual->direita); // Chamada recursiva
            } else {
                printf("\nNão há caminho para a direita a partir daqui.\n");
                menuExploracao(salaAtual); // Permite nova escolha no mesmo local
            }
            break;

        case 's':
        case 'S':
            printf("\nVocê decidiu parar a investigação. Saindo da Mansão Blackwood.\n");
            break;

        default:
            printf("\nOpção inválida. Tente novamente.\n");
            menuExploracao(salaAtual); // Permite nova escolha no mesmo local
            break;
    }
}

/**
 * @brief Função auxiliar para centralizar a chamada do explorador quando há reentrada no mesmo nó.
 * @param raiz O ponteiro para o nó atual a ser explorado.
 */
void menuExploracao(Sala *raiz) {
    // Se a escolha for 's' na função exploradorMansao, ela retorna, e voltamos para main.
    // Se a escolha for um caminho bloqueado ou inválido, voltamos aqui para re-solicitar a entrada.
    
    // Verifica se o jogador não escolheu sair (S) no nível anterior
    if (raiz != NULL) {
        printf("\n--- Menu de Escolha em %s ---\n", raiz->nome);
        // Chama a função de exploração novamente, mas como já estamos no ponto de decisão, 
        // precisamos garantir que a próxima chamada de scanf não seja pulada.
        
        // Vamos realocar a lógica de input aqui para evitar recursão infinita no menu
        char escolha;
        printf("Para onde deseja ir? (e) Esquerda | (d) Direita | (s) Sair do Jogo: ");
        scanf(" %c", &escolha);
        
        if (escolha == 'e' || escolha == 'E') {
            if (raiz->esquerda != NULL) {
                explorarMansao(raiz->esquerda);
            } else {
                printf("\nNão há caminho para a esquerda a partir daqui.\n");
                menuExploracao(raiz); // Tenta novamente neste cômodo
            }
        } else if (escolha == 'd' || escolha == 'D') {
            if (raiz->direita != NULL) {
                explorarMansao(raiz->direita);
            } else {
                printf("\nNão há caminho para a direita a partir daqui.\n");
                menuExploracao(raiz); // Tenta novamente neste cômodo
            }
        } else if (escolha == 's' || escolha == 'S') {
             printf("\nVocê decidiu parar a investigação. Saindo da Mansão Blackwood.\n");
             // Esta saída deve levar ao fim do programa, mas para manter a estrutura recursiva simples, 
             // tratamos o 's' como um retorno para a função chamadora em exploradorMansao.
             return;
        } else {
            printf("\nOpção inválida. Tente novamente.\n");
            menuExploracao(raiz);
        }
    }
}


/**
 * @brief Libera a memória alocada para todos os nós da árvore (pós-ordem).
 * @param no O nó inicial para a liberação.
 */
void liberarArvore(Sala *no) {
    if (no == NULL) {
        return;
    }
    liberarArvore(no->esquerda);
    liberarArvore(no->direita);
    free(no);
}


