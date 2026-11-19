#include <stdio.h>
#include <stdlib.h>
#include "cobertura.h"

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

int numero_arestas(Grafo *g)
{
    int total = 0;
    for (int i = 0; i < g->n; i++)
    {
        for (int j = i + 1; j < g->n; j++)
        {
            total += g->adj[i][j];
        }
    }
    return total;
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

/* ----- Verificacao ----- */

/* toda aresta (u, v) precisa ter pelo menos uma ponta na cobertura */
int eh_cobertura(Grafo *g, int *cobertura)
{
    for (int u = 0; u < g->n; u++)
    {
        for (int v = u + 1; v < g->n; v++)
        {
            if (g->adj[u][v] && !cobertura[u] && !cobertura[v])
            {
                return 0;
            }
        }
    }
    return 1;
}

/* ----- Forca bruta com backtracking ----- */

/* Acha a primeira aresta que ainda nao esta coberta e tenta as duas opcoes:
   colocar u na cobertura ou colocar v (uma das duas tem que entrar).
   Poda: se ja usei tantos vertices quanto a melhor solucao achada, nao adianta continuar. */
static void backtracking(Grafo *g, int *atual, int tamanho, int *melhor, int *melhor_tamanho)
{
    if (tamanho >= *melhor_tamanho)
    {
        return; // poda por limite
    }

    int u = -1, v = -1;
    for (int i = 0; i < g->n && u == -1; i++)
    {
        if (atual[i])
        {
            continue;
        }
        for (int j = i + 1; j < g->n; j++)
        {
            if (g->adj[i][j] && !atual[j])
            {
                u = i;
                v = j;
                break;
            }
        }
    }

    if (u == -1)
    {
        /* nao sobrou aresta descoberta: achei uma cobertura menor */
        *melhor_tamanho = tamanho;
        for (int i = 0; i < g->n; i++)
        {
            melhor[i] = atual[i];
        }
        return;
    }

    atual[u] = 1;
    backtracking(g, atual, tamanho + 1, melhor, melhor_tamanho);
    atual[u] = 0;

    atual[v] = 1;
    backtracking(g, atual, tamanho + 1, melhor, melhor_tamanho);
    atual[v] = 0;
}

/* Retorna o tamanho da cobertura minima e preenche melhor[] */
int cobertura_minima_bt(Grafo *g, int *melhor)
{
    int *atual = calloc(g->n, sizeof(int));

    /* comeca com todos os vertices, que sempre e uma cobertura valida */
    int melhor_tamanho = g->n;
    for (int i = 0; i < g->n; i++)
    {
        melhor[i] = 1;
    }

    backtracking(g, atual, 0, melhor, &melhor_tamanho);

    free(atual);
    return melhor_tamanho;
}

/* ----- Heuristica gulosa ----- */

/* Enquanto tiver aresta descoberta, pega o vertice com maior grau "restante"
   (quantas arestas ainda descobertas ele cobriria). Nao garante o minimo. */
int cobertura_gulosa(Grafo *g, int *cobertura)
{
    int *grau = calloc(g->n, sizeof(int));
    int tamanho = 0;

    for (int i = 0; i < g->n; i++)
    {
        cobertura[i] = 0;
        for (int j = 0; j < g->n; j++)
        {
            grau[i] += g->adj[i][j];
        }
    }

    while (1)
    {
        int escolhido = -1;
        for (int i = 0; i < g->n; i++)
        {
            if (!cobertura[i] && grau[i] > 0 && (escolhido == -1 || grau[i] > grau[escolhido]))
            {
                escolhido = i;
            }
        }

        if (escolhido == -1)
        {
            break; // todas as arestas cobertas
        }

        cobertura[escolhido] = 1;
        tamanho++;
        grau[escolhido] = 0;

        /* as arestas do escolhido agora estao cobertas, entao os vizinhos perdem grau */
        for (int j = 0; j < g->n; j++)
        {
            if (g->adj[escolhido][j] && !cobertura[j])
            {
                grau[j]--;
            }
        }
    }

    free(grau);
    return tamanho;
}

void imprimir_cobertura(int n, int *cobertura)
{
    printf("{ ");
    for (int i = 0; i < n; i++)
    {
        if (cobertura[i])
        {
            printf("%d ", i);
        }
    }
    printf("}");
}
