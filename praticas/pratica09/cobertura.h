#ifndef COBERTURA_H
#define COBERTURA_H

/* ----- Grafo simples nao direcionado (matriz de adjacencia) ----- */

typedef struct
{
    int n;
    int **adj;
} Grafo;

Grafo *criar_grafo(int n);
void inserir_aresta(Grafo *g, int u, int v);
int numero_arestas(Grafo *g);
void liberar_grafo(Grafo *g);

/* ----- Cobertura de Vertices -----
   A cobertura e representada por um vetor 0/1: cobertura[v] = 1 se v esta nela. */

int eh_cobertura(Grafo *g, int *cobertura);
int cobertura_minima_bt(Grafo *g, int *melhor);
int cobertura_gulosa(Grafo *g, int *cobertura);
void imprimir_cobertura(int n, int *cobertura);

#endif
