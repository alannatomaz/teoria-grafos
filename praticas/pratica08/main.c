#include <stdio.h>
#include "agm.h"

int main(void)
{
    /* Grafo do exemplo classico de AGM (a=0 b=1 c=2 d=3 e=4 f=5 g=6 h=7 i=8) */
    int n = 9;
    Aresta arestas[] = {
        {0, 1, 4},  {0, 7, 8},  {1, 2, 8},  {1, 7, 11}, {2, 3, 7},
        {2, 5, 4},  {2, 8, 2},  {3, 4, 9},  {3, 5, 14}, {4, 5, 10},
        {5, 6, 2},  {6, 7, 1},  {6, 8, 6},  {7, 8, 7}
    };
    int m = sizeof(arestas) / sizeof(arestas[0]);

    /* o Prim usa lista de adjacencia, entao monta o grafo com as mesmas arestas */
    GrafoListaPonderado *g = criar_grafo_ponderado(n);
    for (int i = 0; i < m; i++)
    {
        inserir_aresta_ponderada(g, arestas[i].u, arestas[i].v, arestas[i].peso);
    }

    printf("=== Kruskal (Union-Find) ===\n");
    Aresta agm[9];
    int qtd = 0;
    int custo_kruskal = kruskal(n, arestas, m, agm, &qtd);

    for (int i = 0; i < qtd; i++)
    {
        printf("  %d - %d  (peso %d)\n", agm[i].u, agm[i].v, agm[i].peso);
    }
    printf("Custo total: %d\n", custo_kruskal);

    printf("\n=== Prim (Min-Heap) ===\n");
    int pai[9];
    int custo_prim = prim(g, 0, pai);

    for (int v = 0; v < n; v++)
    {
        if (pai[v] != -1)
        {
            printf("  %d - %d\n", pai[v], v);
        }
    }
    printf("Custo total: %d\n", custo_prim);

    printf("\nOs dois deram o mesmo custo? %s\n", custo_kruskal == custo_prim ? "Sim" : "Nao");

    liberar_grafo_ponderado(g);
    return 0;
}
