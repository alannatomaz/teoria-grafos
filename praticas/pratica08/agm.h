#ifndef AGM_H
#define AGM_H

#define INFINITO 2147483647

/* ----- Union-Find (DSU) ----- */

typedef struct {
    int *pai, *rank;
    int n;
} DSU;

DSU *criar_dsu(int n);
int find(DSU *d, int x);
int unir(DSU *d, int x, int y);
void liberar_dsu(DSU *d);

/* ----- Aresta (usada no Kruskal) ----- */

typedef struct {
    int u, v, peso;
} Aresta;

/* ----- Grafo ponderado nao direcionado (usado no Prim) ----- */

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

/* ----- Min-Heap (usado no Prim) ----- */

typedef struct {
    int vertice;
    int chave;
} NoHeap;

typedef struct {
    NoHeap *dados;
    int capacidade, tamanho;
    int *posicao; // mapeia vertice -> indice no heap
} MinHeap;

MinHeap *criar_min_heap(int capacidade);
void inserir_min_heap(MinHeap *h, int vertice, int chave);
NoHeap extrair_minimo(MinHeap *h);
void diminuir_chave(MinHeap *h, int vertice, int nova_chave);
int min_heap_vazia(MinHeap *h);
void liberar_min_heap(MinHeap *h);

/* ----- Arvore Geradora Minima ----- */

int kruskal(int n, Aresta *arestas, int m, Aresta *agm, int *qtd_agm);
int prim(GrafoListaPonderado *g, int raiz, int *pai);

#endif
