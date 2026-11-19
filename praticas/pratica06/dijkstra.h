#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#define INFINITO 2147483647

/* ----- Grafo ponderado em lista de adjacencia ----- */

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

/* ----- Min-Heap (fila de prioridade) ----- */

typedef struct {
    int vertice;
    int distancia;
} NoHeap;

typedef struct {
    NoHeap *dados;
    int capacidade, tamanho;
    int *posicao; // mapeia vertice -> indice no heap
} MinHeap;

MinHeap *criar_min_heap(int capacidade);
void inserir_min_heap(MinHeap *h, int vertice, int distancia);
NoHeap extrair_minimo(MinHeap *h);
void diminuir_chave(MinHeap *h, int vertice, int nova_distancia);
int min_heap_vazia(MinHeap *h);
void liberar_min_heap(MinHeap *h);

/* ----- Dijkstra ----- */

void dijkstra(GrafoListaPonderado *g, int origem, int *dist, int *pred);
void reconstruir_caminho(int origem, int destino, int *pred);

#endif
