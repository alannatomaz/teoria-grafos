#include <stdio.h>
#include "dijkstra.h"

int main(void)
{
    /* Grafo do exemplo do professor: A=0 B=1 C=2 D=3 E=4 F=5 */
    int n = 6;
    GrafoListaPonderado *g = criar_grafo_ponderado(n);

    inserir_aresta_ponderada(g, 0, 1, 4);  // A-B
    inserir_aresta_ponderada(g, 0, 2, 2);  // A-C
    inserir_aresta_ponderada(g, 1, 2, 1);  // B-C
    inserir_aresta_ponderada(g, 1, 3, 5);  // B-D
    inserir_aresta_ponderada(g, 1, 4, 8);  // B-E
    inserir_aresta_ponderada(g, 2, 3, 4);  // C-D
    inserir_aresta_ponderada(g, 2, 4, 10); // C-E
    inserir_aresta_ponderada(g, 3, 4, 2);  // D-E
    inserir_aresta_ponderada(g, 3, 5, 1);  // D-F
    inserir_aresta_ponderada(g, 4, 5, 3);  // E-F

    exibir_grafo_ponderado(g);

    int dist[n];
    int pred[n];
    int origem = 0;

    dijkstra(g, origem, dist, pred);

    printf("\nDistancias a partir do vertice %d:\n", origem);
    for (int v = 0; v < n; v++) {
        printf("  %d -> %d : custo %d\n", origem, v, dist[v]);
    }

    printf("\nReconstrucao dos caminhos:\n");
    for (int v = 0; v < n; v++) {
        reconstruir_caminho(origem, v, pred);
    }

    liberar_grafo_ponderado(g);

    return 0;
}
