#include <stdio.h>
#include <stdlib.h>
#include "bellman_ford.h"

/* ----- Grafo ponderado direcionado ----- */

GrafoListaPonderado *criar_grafo_ponderado(int n)
{
    GrafoListaPonderado *g = malloc(sizeof(GrafoListaPonderado));
    g->n = n;
    g->adj = calloc(n, sizeof(NoAdj *));
    return g;
}

/* Aqui a aresta e so u -> v. No Bellman-Ford o grafo tem que ser
   orientado, senao uma aresta negativa ja vira um ciclo negativo (u-v-u). */
void inserir_aresta_ponderada(GrafoListaPonderado *g, int u, int v, int peso)
{
    if (u < 0 || u >= g->n || v < 0 || v >= g->n)
    {
        return;
    }

    NoAdj *novo = malloc(sizeof(NoAdj));
    novo->destino = v;
    novo->peso = peso;
    novo->prox = g->adj[u];
    g->adj[u] = novo;
}

void liberar_grafo_ponderado(GrafoListaPonderado *g)
{
    for (int i = 0; i < g->n; i++)
    {
        NoAdj *atual = g->adj[i];
        while (atual != NULL)
        {
            NoAdj *tmp = atual;
            atual = atual->prox;
            free(tmp);
        }
    }
    free(g->adj);
    free(g);
}

void exibir_grafo_ponderado(GrafoListaPonderado *g)
{
    printf("Lista de Adjacencia (direcionada):\n");
    for (int i = 0; i < g->n; i++)
    {
        printf("%d:", i);
        NoAdj *atual = g->adj[i];
        while (atual != NULL)
        {
            printf(" -> %d(%d)", atual->destino, atual->peso);
            atual = atual->prox;
        }
        printf("\n");
    }
}

/* ----- Bellman-Ford ----- */

int bellman_ford(GrafoListaPonderado *g, int origem, int *dist, int *pred)
{
    for (int v = 0; v < g->n; v++)
    {
        dist[v] = INFINITO;
        pred[v] = -1;
    }
    dist[origem] = 0;

    /* n-1 iteracoes relaxando todas as arestas */
    for (int i = 1; i <= g->n - 1; i++)
    {
        int mudou = 0;

        for (int u = 0; u < g->n; u++)
        {
            if (dist[u] == INFINITO)
            {
                continue;
            }

            for (NoAdj *a = g->adj[u]; a != NULL; a = a->prox)
            {
                if (dist[u] + a->peso < dist[a->destino])
                {
                    dist[a->destino] = dist[u] + a->peso;
                    pred[a->destino] = u;
                    mudou = 1;
                }
            }
        }

        if (!mudou)
        {
            break; // nada mudou, ja convergiu
        }
    }

    /* n-esima iteracao: se ainda da pra relaxar alguma aresta, tem ciclo negativo */
    for (int u = 0; u < g->n; u++)
    {
        if (dist[u] == INFINITO)
        {
            continue;
        }

        for (NoAdj *a = g->adj[u]; a != NULL; a = a->prox)
        {
            if (dist[u] + a->peso < dist[a->destino])
            {
                return 0;
            }
        }
    }

    return 1;
}

/* menor peso entre as arestas u -> v (pode ter aresta repetida) */
static int peso_aresta(GrafoListaPonderado *g, int u, int v)
{
    int menor = INFINITO;
    for (NoAdj *a = g->adj[u]; a != NULL; a = a->prox)
    {
        if (a->destino == v && a->peso < menor)
        {
            menor = a->peso;
        }
    }
    return menor;
}

/* Faz mais uma passada para achar um vertice que ainda relaxa, depois volta
   n vezes pelo pred[] (assim com certeza cai dentro do ciclo) e imprime o ciclo. */
void reconstruir_ciclo_negativo(GrafoListaPonderado *g, int *dist, int *pred)
{
    int x = -1;

    for (int u = 0; u < g->n && x == -1; u++)
    {
        if (dist[u] == INFINITO)
        {
            continue;
        }

        for (NoAdj *a = g->adj[u]; a != NULL; a = a->prox)
        {
            if (dist[u] + a->peso < dist[a->destino])
            {
                pred[a->destino] = u;
                x = a->destino;
                break;
            }
        }
    }

    if (x == -1)
    {
        printf("Nenhum ciclo negativo encontrado.\n");
        return;
    }

    for (int i = 0; i < g->n; i++)
    {
        x = pred[x];
        if (x == -1)
        {
            printf("Nao foi possivel reconstruir o ciclo.\n");
            return;
        }
    }

    /* o pred[] anda de tras pra frente, entao guarda e imprime invertido */
    int *ciclo = malloc((g->n + 1) * sizeof(int));
    int tamanho = 0;
    int v = x;

    do
    {
        ciclo[tamanho++] = v;
        v = pred[v];
    } while (v != x);
    ciclo[tamanho++] = x;

    int custo = 0;
    printf("Ciclo negativo: ");
    for (int i = tamanho - 1; i >= 0; i--)
    {
        printf("%d", ciclo[i]);
        if (i > 0)
        {
            printf(" -> ");
            custo += peso_aresta(g, ciclo[i], ciclo[i - 1]);
        }
    }
    printf("  (custo total %d)\n", custo);

    free(ciclo);
}

void imprimir_caminho(int origem, int destino, int *pred)
{
    if (destino == origem)
    {
        printf("%d", origem);
        return;
    }

    if (pred[destino] == -1)
    {
        printf("sem caminho");
        return;
    }

    imprimir_caminho(origem, pred[destino], pred);
    printf(" -> %d", destino);
}
