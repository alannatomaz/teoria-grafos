#include <stdio.h>
#include <stdlib.h>
#include "hamiltoniano.h"

/* ----- Grafo ----- */

Grafo *criar_grafo(int n)
{
    Grafo *g = malloc(sizeof(Grafo));
    g->n = n;
    g->adj = malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++)
    {
        g->adj[i] = calloc(n, sizeof(int));
    }
    return g;
}

void inserir_aresta(Grafo *g, int u, int v)
{
    if (u < 0 || u >= g->n || v < 0 || v >= g->n || u == v)
    {
        return;
    }
    g->adj[u][v] = 1;
    g->adj[v][u] = 1;
}

void liberar_grafo(Grafo *g)
{
    for (int i = 0; i < g->n; i++)
    {
        free(g->adj[i]);
    }
    free(g->adj);
    free(g);
}

/* ----- Backtracking ----- */

/* Monta a permutacao posicao por posicao: na posicao 'pos' tenta todo vertice
   ainda nao visitado que seja vizinho do anterior. Se nao der, desfaz e tenta outro.
   Quando 'ciclo' = 1, no final ainda precisa existir a aresta ultimo -> primeiro. */
static int backtracking(Grafo *g, int *seq, int *visitado, int pos, int ciclo)
{
    if (pos == g->n)
    {
        if (ciclo)
        {
            return g->adj[seq[g->n - 1]][seq[0]];
        }
        return 1;
    }

    for (int v = 0; v < g->n; v++)
    {
        if (!visitado[v] && g->adj[seq[pos - 1]][v])
        {
            visitado[v] = 1;
            seq[pos] = v;

            if (backtracking(g, seq, visitado, pos + 1, ciclo))
            {
                return 1;
            }

            visitado[v] = 0; // desfaz e tenta o proximo
        }
    }

    return 0;
}

int encontrar_caminho_hamiltoniano_bt(Grafo *g, int *caminho)
{
    if (g->n == 0)
    {
        return 0;
    }

    int *visitado = calloc(g->n, sizeof(int));

    /* o caminho pode comecar em qualquer vertice, entao testa todos */
    for (int inicio = 0; inicio < g->n; inicio++)
    {
        caminho[0] = inicio;
        visitado[inicio] = 1;

        if (backtracking(g, caminho, visitado, 1, 0))
        {
            free(visitado);
            return 1;
        }

        visitado[inicio] = 0;
    }

    free(visitado);
    return 0;
}

int encontrar_ciclo_hamiltoniano_bt(Grafo *g, int *ciclo)
{
    if (g->n < 3)
    {
        return 0; // ciclo simples precisa de pelo menos 3 vertices
    }

    int *visitado = calloc(g->n, sizeof(int));

    /* no ciclo da pra fixar o inicio no 0: se existe ciclo, ele passa pelo 0 */
    ciclo[0] = 0;
    visitado[0] = 1;
    int achou = backtracking(g, ciclo, visitado, 1, 1);

    free(visitado);
    return achou;
}

void imprimir_sequencia(int n, int *seq, int fecha_ciclo)
{
    for (int i = 0; i < n; i++)
    {
        printf("%d", seq[i]);
        if (i < n - 1)
        {
            printf(" -> ");
        }
    }
    if (fecha_ciclo && n > 0)
    {
        printf(" -> %d", seq[0]);
    }
}
