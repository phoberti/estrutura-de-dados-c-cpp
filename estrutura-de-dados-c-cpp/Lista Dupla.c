/*
Pedro Henrique de Oliveira Berti
Colegiado de C. Computação
Estrutura de Dados
Prof Fabiane
*/

#include <stdio.h>
#include <stdlib.h>

struct Node
{
    int num;
    struct Node *prox;
    struct Node *ant;
};

typedef struct Node node;
typedef struct Node *LISTA;

LISTA criarLista()
{
    LISTA inicio = NULL;
    return inicio;
}

void ordena(LISTA *lista)
{
    int cont, aux;
    node *tmp;

    if ((*lista) == NULL)
    {
        printf("LISTA VAZIA");
    }
    else
    {
        do
        {
            cont = 0;
            tmp = *lista;
            while (tmp->prox != NULL)
            {
                if (tmp->num > tmp->prox->num)
                {
                    aux = tmp->num;
                    tmp->num = tmp->prox->num;
                    tmp->prox->num = aux;
                    cont++;
                }
                tmp = tmp->prox;
            }
        } while (cont != 0);
    }
}

void insereInicio(LISTA *lista)
{
    node *novo = (node *)malloc(sizeof(node));
    if (novo == NULL)
    {
        printf("\nErro na Alocação");
        exit(0);
    }
    printf("\nINFORME O VALOR DO NOVO ELEMENTO: ");
    scanf("%d", &novo->num);

    novo->prox = *lista;
    novo->ant = NULL;

    if (*lista != NULL)
    {
        (*lista)->ant = novo;
    }

    *lista = novo;
}

void insereFinal(LISTA *lista)
{
    node *novo = (node *)malloc(sizeof(node));
    if (novo == NULL)
    {
        printf("\nErro na Alocação");
        exit(0);
    }

    printf("\nINFORME O VALOR DO NOVO ELEMENTO: ");
    scanf("%d", &novo->num);

    novo->prox = NULL;

    if (*lista == NULL)
    {
        novo->ant = NULL;
        *lista = novo;
    }
    else
    {
        node *tmp = *lista;
        while (tmp->prox != NULL)
        {
            tmp = tmp->prox;
        }
        tmp->prox = novo;
        novo->ant = tmp;
    }
}

void exibe(LISTA *lista)
{
    if (*lista == NULL)
    {
        printf("\nLISTA VAZIA");
    }
    else
    {
        node *tmp;
        tmp = *lista;
        while (tmp != NULL)
        {

            printf(" %d->", tmp->num);
            tmp = tmp->prox;
        }
    }
    printf("\n");
}

void libera(LISTA *lista)
{
    if (*lista == NULL)
    {
        printf("\nLISTA VAZIA");
    }
    else
    {
        node *tmp;
        while (*lista != NULL)
        {
            tmp = *lista;
            *lista = (*lista)->prox;
            free(tmp);
        }
    }
}

void insereOrdenado(LISTA *lista)
{
    node *novo = (node *)malloc(sizeof(node));
    if (novo == NULL)
    {
        printf("\nErro na Alocação");
        exit(0);
    }
    printf("\nINFORME O VALOR DO NOVO ELEMENTO: ");
    scanf("%d", &novo->num);
    novo->prox = NULL;
    novo->ant = NULL;

    if (*lista == NULL)
    {

        *lista = novo;
    }
    else if (novo->num <= (*lista)->num)
    {

        novo->prox = *lista;
        (*lista)->ant = novo;
        *lista = novo;
    }
    else
    {

        node *atual = *lista;
        while (atual->prox != NULL && novo->num > atual->prox->num)
        {
            atual = atual->prox;
        }

        novo->prox = atual->prox;
        if (atual->prox != NULL)
        {
            atual->prox->ant = novo;
        }
        novo->ant = atual;
        atual->prox = novo;
    }
}

void removeElemento(LISTA *lista)
{
    if (*lista == NULL)
    {
        printf("\nLISTA VAZIA");
        return;
    }

    int valorRemover;
    printf("\nInforme o valor a ser removido: ");
    scanf("%d", &valorRemover);

    node *atual = *lista;
    while (atual != NULL)
    {
        if (atual->num == valorRemover)
        {
            if (atual->ant == NULL)
            {

                *lista = atual->prox;
                if (atual->prox != NULL)
                {
                    atual->prox->ant = NULL;
                }
            }
            else
            {

                atual->ant->prox = atual->prox;
                if (atual->prox != NULL)
                {
                    atual->prox->ant = atual->ant;
                }
            }
            free(atual);
            printf("Elemento removido com sucesso.\n");
            return;
        }
        atual = atual->prox;
    }

    printf("Elemento não encontrado na lista.\n");
}

void exibeOrdemDecrescente(LISTA *lista)
{

    int cont, aux;
    node *tmp;

    if ((*lista) == NULL)
    {
        printf("LISTA VAZIA");
    }
    else
    {
        do
        {
            cont = 0;
            tmp = *lista;
            while (tmp->prox != NULL)
            {
                if (tmp->num < tmp->prox->num)
                {
                    aux = tmp->num;
                    tmp->num = tmp->prox->num;
                    tmp->prox->num = aux;
                    cont++;
                }
                tmp = tmp->prox;
            }
        } while (cont != 0);
    }

    if (*lista == NULL)
    {
        printf("\nLISTA VAZIA");
    }
    else
    {
        node *tmp;
        tmp = *lista;
        while (tmp != NULL)
        {

            printf(" %d->", tmp->num);
            tmp = tmp->prox;
        }
    }
    printf("\n");
}

int main()
{
    LISTA lista = criarLista();
    int op;

    do
    {
        printf("\nSELECIONE O ITEM DESEJADO");
        printf("\n1- INSERIR NO INICIO DA LISTA");
        printf("\n2- INSERIR NO FINAL DA LISTA");
        printf("\n3- MOSTRAR LISTA");
        printf("\n4- REMOVER LISTA COMPLETA");
        printf("\n5- ORDENAR LISTA");
        printf("\n6- INSERIR ORDENADO ( AUTOMATICAMENTE ORDENA A LSITA)");
        printf("\n7- REMOVER QUALQUER ELEMENTO");
        printf("\n8- EXIBE CRESCENTE");
        printf("\n9- EXIBE DECRESCENTE");
        printf("\n0- SAIR");
        printf("\nITEM ESCOLHIDO: ");
        scanf("%d", &op);

        switch (op)
        {
        case 1:
            insereInicio(&lista);
            break;
        case 2:
            insereFinal(&lista);
            break;
        case 3:
            exibe(&lista);
            break;
        case 4:
            libera(&lista);
            break;
        case 5:
            ordena(&lista);
            break;
        case 6:
            ordena(&lista);
            insereOrdenado(&lista);
            break;
        case 7:
            removeElemento(&lista);
            break;
        case 8:
            ordena(&lista);
            exibe(&lista);
            break;
        case 9:
            ordena(&lista);
            exibeOrdemDecrescente(&lista);
            break;
        case 0:
            break;
        default:
            printf("OPÇÃO NÃO CADASTRADA");
            break;
        }
    } while (op != 0);

    libera(&lista);
    return 0;
}
