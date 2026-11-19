#ifndef HAMILTONIANO_H
#define HAMILTONIANO_H

/* ----- Grafo simples nao direcionado (matriz de adjacencia) ----- */

typedef struct
{
    int n;
    int **adj;
} Grafo;

Grafo *criar_grafo(int n);
void inserir_aresta(Grafo *g, int u, int v);
void liberar_grafo(Grafo *g);

/* ----- Caminho e Ciclo Hamiltoniano (backtracking) -----
   Retornam 1 se encontrou e 0 se nao existe.
   O vetor recebe a ordem dos n vertices (no ciclo, volta do ultimo pro primeiro). */

int encontrar_caminho_hamiltoniano_bt(Grafo *g, int *caminho);
int encontrar_ciclo_hamiltoniano_bt(Grafo *g, int *ciclo);
void imprimir_sequencia(int n, int *seq, int fecha_ciclo);

#endif
