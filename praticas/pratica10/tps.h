#ifndef TPS_H
#define TPS_H

#include "hamiltoniano.h"

#define INFINITO 2147483647

/* ----- Problema do Caixeiro Viajante (TSP) -----
   Grafo completo representado por uma matriz de custos (simetrica).
   A rota e um vetor com a ordem dos n vertices; no final volta pro primeiro. */

typedef struct
{
    int n;
    int **custo;
} MatrizCustos;

MatrizCustos *criar_matriz_custos(int n);
void liberar_matriz_custos(MatrizCustos *m);

int custo_rota(MatrizCustos *m, int *rota);
int tsp_forca_bruta(MatrizCustos *m, int *rota);
int tsp_vizinho_mais_proximo(MatrizCustos *m, int origem, int *rota);
int tsp_insercao(MatrizCustos *m, int *rota);

/* reducao: aresta que existe vira custo 1, aresta que nao existe vira custo 2 */
MatrizCustos *grafo_para_tsp(Grafo *g);

#endif
