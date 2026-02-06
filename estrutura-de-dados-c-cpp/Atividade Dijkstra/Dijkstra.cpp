/*
Pedro Henrique de Oliveira Berti
Colegiado de C. Computação
Estrutura de Dados
Prof Fabiane
*/

#include <iostream>
#include <limits>
#include <vector>

struct Grafo {
    int nro_vertices;
    int Gmax;
    int eh_ponderado;
    std::vector<int> grau;
    std::vector<std::vector<int>> arestas;
    std::vector<std::vector<float>> pesos;
};

Grafo* cria_Grafo(int nro_vertices, int Gmax, int eh_ponderado) {
    Grafo* gr = new Grafo;
    gr->nro_vertices = nro_vertices;
    gr->Gmax = Gmax;
    gr->eh_ponderado = (eh_ponderado != 0) ? 1 : 0;
    gr->grau = std::vector<int>(nro_vertices, 0);
    gr->arestas = std::vector<std::vector<int>>(nro_vertices, std::vector<int>(Gmax, 0));

    if (gr->eh_ponderado) {
        gr->pesos = std::vector<std::vector<float>>(nro_vertices, std::vector<float>(Gmax, 0.0));
    }

    return gr;
}

void libera_Grafo(Grafo* gr) {
    delete gr;
}

bool arestaExiste(Grafo* gr, int orig, int dest) {
    for (int i = 0; i < gr->grau[orig]; i++) {
        if (gr->arestas[orig][i] == dest)
            return true;
    }
    return false;
}

bool insereAresta(Grafo* gr, int orig, int dest, int digrafo, float peso) {
    if (gr == nullptr)
        return false;
    if (orig < 0 || orig >= gr->nro_vertices)
        return false;
    if (dest < 0 || dest >= gr->nro_vertices)
        return false;

    if (!arestaExiste(gr, orig, dest)) {
        gr->arestas[orig][gr->grau[orig]] = dest;
        if (gr->eh_ponderado)
            gr->pesos[orig][gr->grau[orig]] = peso;
        gr->grau[orig]++;
    }

    if (digrafo == 0)
        insereAresta(gr, dest, orig, 1, peso);

    return true;
}

bool removeAresta(Grafo* gr, int orig, int dest, int eh_digrafo) {
    if (gr == nullptr)
        return false;
    if (orig < 0 || orig >= gr->nro_vertices)
        return false;
    if (dest < 0 || dest >= gr->nro_vertices)
        return false;

    int i = 0;
    while (i < gr->grau[orig] && gr->arestas[orig][i] != dest)
        i++;

    if (i == gr->grau[orig])
        return false;

    gr->grau[orig]--;
    gr->arestas[orig][i] = gr->arestas[orig][gr->grau[orig]];

    if (gr->eh_ponderado)
        gr->pesos[orig][i] = gr->pesos[orig][gr->grau[orig]];

    if (eh_digrafo == 0)
        removeAresta(gr, dest, orig, 1);

    return true;
}

int procuraMenorDistancia(std::vector<float>& dist, std::vector<int>& visitado) {
    int menor = -1;
    bool primeiro = true;
    for (int i = 0; i < dist.size(); i++) {
        if (dist[i] >= 0 && visitado[i] == 0) {
            if (primeiro) {
                menor = i;
                primeiro = false;
            } else {
                if (dist[menor] > dist[i])
                    menor = i;
            }
        }
    }
    return menor;
}

void menorCaminho_Grafo(Grafo* gr, int ini, std::vector<int>& ant, std::vector<float>& dist) {
    int cont, NV, ind, vert;
    cont = NV = gr->nro_vertices;
    std::vector<int> visitado(NV, 0);

    for (int i = 0; i < NV; i++) {
        ant[i] = -1;
        dist[i] = std::numeric_limits<float>::max();
        visitado[i] = 0;
    }

    dist[ini] = 0;

    while (cont > 0) {
        vert = procuraMenorDistancia(dist, visitado);
        if (vert == -1)
            break;

        visitado[vert] = 1;
        cont--;

        for (int i = 0; i < gr->grau[vert]; i++) {
            ind = gr->arestas[vert][i];
            if (dist[ind] == std::numeric_limits<float>::max() || dist[ind] > dist[vert] + gr->pesos[vert][i]) {
                dist[ind] = dist[vert] + gr->pesos[vert][i];
                ant[ind] = vert;
            }
        }
    }
}

int main() {
    int printar[10];
    int iaaa = 0;
    int eh_digrafo = 0;

    Grafo* gr = cria_Grafo(5, 5, 1);
    insereAresta(gr, 0, 1, eh_digrafo, 48);
    insereAresta(gr, 0, 3, eh_digrafo, 33);
    insereAresta(gr, 1, 2, eh_digrafo, 52);
    insereAresta(gr, 2, 3, eh_digrafo, 16);
    insereAresta(gr, 2, 4, eh_digrafo, 108);

    std::vector<int> visitado(gr->nro_vertices, 0);
    std::vector<int> ant(gr->nro_vertices, 0);
    std::vector<float> dist(gr->nro_vertices, 0.0);

    for (int i = 0; i < gr->nro_vertices; i++) {
        dist[i] = std::numeric_limits<float>::max();
    }

    menorCaminho_Grafo(gr, 0, ant, dist);

    int destino = 4;

    if (dist[destino] < std::numeric_limits<float>::max()) {
        std::cout << "Menor caminho de 1 para 5 com peso total " << dist[destino] << ":\n";
        while (ant[destino] != -1) {
            printar[iaaa] = destino + 1;
            iaaa++;
            destino = ant[destino];
        }
        printar[iaaa] = destino + 1;
    } else {
        std::cout << "Não há caminho de 1 para 5.\n";
    }

    int ind;
    for (ind = iaaa; ind > 0; ind--) {
        std::cout << printar[ind] << " -> ";
    }
    std::cout << printar[ind];

    libera_Grafo(gr);
    return 0;
}
