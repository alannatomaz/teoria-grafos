#ifndef BELLMAN_FORD_H
#define BELLMAN_FORD_H

#define INFINITO 2147483647

/* ----- Grafo ponderado direcionado (lista de adjacencia) ----- */

typedef struct NoAdj
{
    int destino;
    int peso;
    struct NoAdj *prox;
} NoAdj;

typedef struct
{
    int n;
    NoAdj **adj;
} GrafoListaPonderado;

GrafoListaPonderado *criar_grafo_ponderado(int n);
void inserir_aresta_ponderada(GrafoListaPonderado *g, int u, int v, int peso);
void liberar_grafo_ponderado(GrafoListaPonderado *g);
void exibir_grafo_ponderado(GrafoListaPonderado *g);

/* ----- Bellman-Ford ----- */

int bellman_ford(GrafoListaPonderado *g, int origem, int *dist, int *pred);
void reconstruir_ciclo_negativo(GrafoListaPonderado *g, int *dist, int *pred);
void imprimir_caminho(int origem, int destino, int *pred);

#endif
