/*
Pedro Henrique de Oliveira Berti
Colegiado de C. Computação
Estrutura de Dados
Prof Fabiane
*/

#include <stdio.h>
#include <stdlib.h>

typedef struct grafo {
    int nro_vert;
    int Gmax;
    int eh_ponderado;
    int* grau;
    int** arestas;
    float** pesos;
} Grafo;

Grafo* cria_Grafo(int nro_vert, int Gmax, int eh_ponderado) {
    Grafo* gr = (Grafo*)malloc(sizeof(struct grafo));
    if (gr != NULL) {
        int i;
        gr->nro_vert = nro_vert;
        gr->Gmax = Gmax;
        gr->eh_ponderado = (eh_ponderado != 0) ? 1 : 0;
        gr->grau = (int*)calloc(nro_vert, sizeof(int));

        gr->arestas = (int**)malloc(nro_vert * sizeof(int*));
        for (i = 0; i < nro_vert; i++) {
            gr->arestas[i] = (int*)malloc(nro_vert * sizeof(int));
        }

        if (gr->eh_ponderado) {
            gr->pesos = (float**)malloc(nro_vert * sizeof(float*));
            for (i = 0; i < nro_vert; i++) {
                gr->pesos[i] = (float*)malloc(nro_vert * sizeof(float));
            }
        }
    }
    return gr;
}

void libera_Grafo(Grafo* gr) {
    if (gr != NULL) {
        int i;
        for (i = 0; i < gr->nro_vert; i++)
            free(gr->arestas[i]);
        free(gr->arestas);

        if (gr->eh_ponderado) {
            for (i = 0; i < gr->nro_vert; i++)
                free(gr->pesos[i]);
            free(gr->pesos);
        }
        free(gr->grau);
        free(gr);
    }
}

int arestaExiste(Grafo* gr, int orig, int dest) {
    int i;
    for (i = 0; i < gr->grau[orig]; i++) {
        if (gr->arestas[orig][i] == dest)
            return 1; // A aresta já existe
    }
    return 0; // A aresta não existe
}

int insereAresta(Grafo* gr, int orig, int dest, int digrafo, float peso) {
    if (gr == NULL)
        return 0;
    if (orig < 0 || orig >= gr->nro_vert)
        return 0;
    if (dest < 0 || dest >= gr->nro_vert)
        return 0;

    if (!arestaExiste(gr, orig, dest)) {
        gr->arestas[orig][gr->grau[orig]] = dest;
        if (gr->eh_ponderado)
            gr->pesos[orig][gr->grau[orig]] = peso;
        gr->grau[orig]++;
    }

    if (digrafo == 0)
        insereAresta(gr, dest, orig, 1, peso);
    return 1;
}

int removeAresta(Grafo* gr, int orig, int dest, int eh_digrafo) {
    if (gr == NULL)
        return 0;
    if (orig < 0 || orig >= gr->nro_vert)
        return 0;
    if (dest < 0 || dest >= gr->nro_vert)
        return 0;

    int i = 0;
    while (i < gr->grau[orig] && gr->arestas[orig][i] != dest)
        i++;

    if (i == gr->grau[orig])  // elemento não encontrado
        return 0;

    gr->grau[orig]--;
    gr->arestas[orig][i] = gr->arestas[orig][gr->grau[orig]];

    if (gr->eh_ponderado)
        gr->pesos[orig][i] = gr->pesos[orig][gr->grau[orig]];

    if (eh_digrafo == 0)
        removeAresta(gr, dest, orig, 1);

    return 1;
}

void buscaProfundidade(Grafo* gr, int ini, int* visitado, int cont) {
    int i;
    visitado[ini] = cont;
    for (i = 0; i < gr->grau[ini]; i++) {
        if (!visitado[gr->arestas[ini][i]])
            buscaProfundidade(gr, gr->arestas[ini][i], visitado, cont + 1);
    }
}

void buscaProfundidade_Grafo(Grafo* gr, int ini, int* visitado) {
    int i, cont = 1;
    for (i = 0; i < gr->nro_vert; i++)
        visitado[i] = 0;
    buscaProfundidade(gr, ini, visitado, cont);
}

void buscaLargura_Grafo(Grafo* gr, int ini, int* visitado) {
    int i, vert, NV, cont = 1;
    int* fila, IF = 0, FF = 0;
    for (i = 0; i < gr->nro_vert; i++)
        visitado[i] = 0;

    NV = gr->nro_vert;
    fila = (int*)malloc(NV * sizeof(int));
    FF++;
    fila[FF] = ini;
    visitado[ini] = cont;

    while (IF != FF) {
        IF = (IF + 1) % NV;
        vert = fila[IF];
        cont++;

        for (i = 0; i < gr->grau[vert]; i++) {
            if (!visitado[gr->arestas[vert][i]]) {
                FF = (FF + 1) % NV;
                fila[FF] = gr->arestas[vert][i];
                visitado[gr->arestas[vert][i]] = cont;
            }
        }
    }
    free(fila);
}

int main() {
    int eh_digrafo = 1;
    Grafo* gr = cria_Grafo(5, 5, 0);

    insereAresta(gr, 0, 1, eh_digrafo, 0);
    insereAresta(gr, 1, 3, eh_digrafo, 0);
    insereAresta(gr, 1, 2, eh_digrafo, 0);
    insereAresta(gr, 2, 4, eh_digrafo, 0);
    insereAresta(gr, 3, 0, eh_digrafo, 0);
    insereAresta(gr, 3, 4, eh_digrafo, 0);
    insereAresta(gr, 4, 1, eh_digrafo, 0);

    int escolha;
    printf("Escolha o tipo de busca:\n");
    printf("1. Busca em Profundidade\n");
    printf("2. Busca em Largura\n");
    scanf("%d", &escolha);

    int* visitado = (int*)calloc(gr->nro_vert, sizeof(int));

    if (escolha == 1) {
        printf("Busca em Profundidade:\n");
        buscaProfundidade_Grafo(gr, 0, visitado);
    } else if (escolha == 2) {
        printf("Busca em Largura:\n");
        buscaLargura_Grafo(gr, 0, visitado);
    } else {
        printf("Opção inválida.\n");
    }

    // Imprimir resultados da busca
    printf("Resultado da Busca:\n");
    for (int i = 0; i < gr->nro_vert; i++) {
        printf("Vértice %d: %d\n", i, visitado[i]);
    }

    // Liberar memória
    libera_Grafo(gr);
    free(visitado);
    printf("\nobs : cada vertice eh visitado e marcado com um numero que indica a ordem em que foi descoberto durante a busca\n");
  printf("se dois vertices tiverem o mesmo numero, significa que foram descobertos ao mesmo tempo \n");
    return 0;
}
