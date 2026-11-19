#include <stdio.h>
#include <stdlib.h>
#include "tps.h"

/* ----- Matriz de custos ----- */

MatrizCustos *criar_matriz_custos(int n)
{
    MatrizCustos *m = malloc(sizeof(MatrizCustos));
    m->n = n;
    m->custo = malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++)
    {
        m->custo[i] = calloc(n, sizeof(int));
    }
    return m;
}

void liberar_matriz_custos(MatrizCustos *m)
{
    for (int i = 0; i < m->n; i++)
    {
        free(m->custo[i]);
    }
    free(m->custo);
    free(m);
}

/* soma das arestas da rota, incluindo a volta do ultimo pro primeiro */
int custo_rota(MatrizCustos *m, int *rota)
{
    if (m->n <= 1)
    {
        return 0;
    }

    int total = 0;
    for (int i = 0; i < m->n; i++)
    {
        total += m->custo[rota[i]][rota[(i + 1) % m->n]];
    }
    return total;
}

/* ----- Forca bruta ----- */

/* Gera todas as permutacoes dos vertices 1..n-1 (a origem 0 fica fixa),
   ou seja, (n-1)! rotas. O custo vai sendo somado conforme a rota cresce. */
static void permutar(MatrizCustos *m, int *atual, int *usado, int pos, int custo_parcial,
                     int *melhor_custo, int *melhor_rota)
{
    if (pos == m->n)
    {
        int total = custo_parcial + m->custo[atual[m->n - 1]][atual[0]];
        if (total < *melhor_custo)
        {
            *melhor_custo = total;
            for (int i = 0; i < m->n; i++)
            {
                melhor_rota[i] = atual[i];
            }
        }
        return;
    }

    for (int v = 1; v < m->n; v++)
    {
        if (!usado[v])
        {
            usado[v] = 1;
            atual[pos] = v;
            permutar(m, atual, usado, pos + 1, custo_parcial + m->custo[atual[pos - 1]][v],
                     melhor_custo, melhor_rota);
            usado[v] = 0;
        }
    }
}

int tsp_forca_bruta(MatrizCustos *m, int *rota)
{
    if (m->n == 0)
    {
        return 0;
    }
    if (m->n == 1)
    {
        rota[0] = 0;
        return 0;
    }

    int *atual = malloc(m->n * sizeof(int));
    int *usado = calloc(m->n, sizeof(int));
    int melhor = INFINITO;

    atual[0] = 0; // origem fixa
    usado[0] = 1;
    permutar(m, atual, usado, 1, 0, &melhor, rota);

    free(atual);
    free(usado);
    return melhor;
}

/* ----- Heuristica: vizinho mais proximo ----- */

/* Sai da origem e vai sempre pra cidade mais perto que ainda nao visitou. */
int tsp_vizinho_mais_proximo(MatrizCustos *m, int origem, int *rota)
{
    if (m->n == 0)
    {
        return 0;
    }

    int *visitado = calloc(m->n, sizeof(int));
    int atual = origem;

    rota[0] = origem;
    visitado[origem] = 1;

    for (int pos = 1; pos < m->n; pos++)
    {
        int proximo = -1;
        for (int v = 0; v < m->n; v++)
        {
            if (!visitado[v] && (proximo == -1 || m->custo[atual][v] < m->custo[atual][proximo]))
            {
                proximo = v;
            }
        }

        rota[pos] = proximo;
        visitado[proximo] = 1;
        atual = proximo;
    }

    free(visitado);
    return custo_rota(m, rota);
}

/* ----- Heuristica: insercao do mais proximo ----- */

/* Comeca com a rota so com o 0. A cada passo escolhe a cidade de fora que esta
   mais perto de alguma cidade da rota, e coloca ela no ponto da rota onde o
   custo aumenta menos: custo[a][k] + custo[k][b] - custo[a][b]. */
int tsp_insercao(MatrizCustos *m, int *rota)
{
    int n = m->n;
    if (n == 0)
    {
        return 0;
    }

    int *na_rota = calloc(n, sizeof(int));
    int *dist_rota = malloc(n * sizeof(int)); // menor distancia de cada cidade ate a rota
    int tamanho = 1;

    rota[0] = 0;
    na_rota[0] = 1;
    for (int v = 0; v < n; v++)
    {
        dist_rota[v] = m->custo[0][v];
    }

    while (tamanho < n)
    {
        /* cidade de fora mais proxima da rota */
        int k = -1;
        for (int v = 0; v < n; v++)
        {
            if (!na_rota[v] && (k == -1 || dist_rota[v] < dist_rota[k]))
            {
                k = v;
            }
        }

        /* melhor lugar pra inserir: entre rota[i] e rota[i+1] */
        int melhor_pos = 0;
        int melhor_aumento = INFINITO;
        for (int i = 0; i < tamanho; i++)
        {
            int a = rota[i];
            int b = rota[(i + 1) % tamanho];
            int aumento = m->custo[a][k] + m->custo[k][b] - m->custo[a][b];
            if (aumento < melhor_aumento)
            {
                melhor_aumento = aumento;
                melhor_pos = i;
            }
        }

        /* abre espaco depois de melhor_pos e coloca o k */
        for (int i = tamanho; i > melhor_pos + 1; i--)
        {
            rota[i] = rota[i - 1];
        }
        rota[melhor_pos + 1] = k;
        tamanho++;
        na_rota[k] = 1;

        for (int v = 0; v < n; v++)
        {
            if (m->custo[k][v] < dist_rota[v])
            {
                dist_rota[v] = m->custo[k][v];
            }
        }
    }

    free(na_rota);
    free(dist_rota);
    return custo_rota(m, rota);
}

/* ----- Reducao Ciclo Hamiltoniano -> TSP ----- */

/* Se o grafo tem ciclo hamiltoniano, da pra fazer uma rota usando so arestas
   de custo 1, entao o TSP otimo custa exatamente n. Se nao tem, o otimo e > n. */
MatrizCustos *grafo_para_tsp(Grafo *g)
{
    MatrizCustos *m = criar_matriz_custos(g->n);
    for (int i = 0; i < g->n; i++)
    {
        for (int j = 0; j < g->n; j++)
        {
            if (i == j)
            {
                m->custo[i][j] = 0;
            }
            else
            {
                m->custo[i][j] = g->adj[i][j] ? 1 : 2;
            }
        }
    }
    return m;
}
