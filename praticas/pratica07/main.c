#include <stdio.h>
#include "bellman_ford.h"

static void executar(GrafoListaPonderado *g, int origem)
{
    exibir_grafo_ponderado(g);

    int dist[g->n];
    int pred[g->n];

    if (bellman_ford(g, origem, dist, pred))
    {
        printf("\nSem ciclo negativo. Distancias a partir do vertice %d:\n", origem);
        for (int v = 0; v < g->n; v++)
        {
            if (dist[v] == INFINITO)
            {
                printf("  %d -> %d : inalcancavel\n", origem, v);
                continue;
            }
            printf("  %d -> %d : custo %3d  | caminho: ", origem, v, dist[v]);
            imprimir_caminho(origem, v, pred);
            printf("\n");
        }
    }
    else
    {
        printf("\nBellman-Ford detectou ciclo negativo!\n");
        reconstruir_ciclo_negativo(g, dist, pred);
    }
}

int main(void)
{
    /* Exemplo 1: tem arestas negativas, mas nenhum ciclo negativo
       (s=0 t=1 x=2 y=3 z=4) */
    GrafoListaPonderado *g1 = criar_grafo_ponderado(5);

    inserir_aresta_ponderada(g1, 0, 1, 6);  // s -> t
    inserir_aresta_ponderada(g1, 0, 3, 7);  // s -> y
    inserir_aresta_ponderada(g1, 1, 2, 5);  // t -> x
    inserir_aresta_ponderada(g1, 1, 3, 8);  // t -> y
    inserir_aresta_ponderada(g1, 1, 4, -4); // t -> z
    inserir_aresta_ponderada(g1, 2, 1, -2); // x -> t
    inserir_aresta_ponderada(g1, 3, 2, -3); // y -> x
    inserir_aresta_ponderada(g1, 3, 4, 9);  // y -> z
    inserir_aresta_ponderada(g1, 4, 0, 2);  // z -> s
    inserir_aresta_ponderada(g1, 4, 2, 7);  // z -> x

    printf("=== Exemplo 1: sem ciclo negativo ===\n");
    executar(g1, 0);
    liberar_grafo_ponderado(g1);

    /* Exemplo 2: o ciclo 1 -> 2 -> 3 -> 1 custa 1 + (-2) + (-1) = -2 */
    GrafoListaPonderado *g2 = criar_grafo_ponderado(5);

    inserir_aresta_ponderada(g2, 0, 1, 4);
    inserir_aresta_ponderada(g2, 1, 2, 1);
    inserir_aresta_ponderada(g2, 2, 3, -2);
    inserir_aresta_ponderada(g2, 3, 1, -1);
    inserir_aresta_ponderada(g2, 3, 4, 2);

    printf("\n=== Exemplo 2: com ciclo negativo ===\n");
    executar(g2, 0);
    liberar_grafo_ponderado(g2);

    return 0;
}
