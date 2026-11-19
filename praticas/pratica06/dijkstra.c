#include <stdio.h>
#include <stdlib.h>
#include "dijkstra.h"

/* ----- Grafo ponderado em lista ----- */

GrafoListaPonderado *criar_grafo_ponderado(int n)
{
    GrafoListaPonderado *g = malloc(sizeof(GrafoListaPonderado));
    g->n = n;
    g->adj = calloc(n, sizeof(NoAdj *));
    return g;
}

static void inserir_direcionado(GrafoListaPonderado *g, int origem, int destino, int peso)
{
    NoAdj *novo = malloc(sizeof(NoAdj));
    novo->destino = destino;
    novo->peso = peso;
    novo->prox = g->adj[origem];
    g->adj[origem] = novo;
}

void inserir_aresta_ponderada(GrafoListaPonderado *g, int u, int v, int peso)
{
    if (u < 0 || u >= g->n || v < 0 || v >= g->n)
    {
        return;
    }

    inserir_direcionado(g, u, v, peso);
    inserir_direcionado(g, v, u, peso);
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
    printf("Lista de Adjacencia (ponderada):\n");
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

/* ----- Min-Heap ----- */

static void troca(MinHeap *h, int i, int j)
{
    NoHeap tmp = h->dados[i];
    h->dados[i] = h->dados[j];
    h->dados[j] = tmp;

    h->posicao[h->dados[i].vertice] = i;
    h->posicao[h->dados[j].vertice] = j;
}

static void sobe(MinHeap *h, int i)
{
    while (i > 0)
    {
        int pai = (i - 1) / 2;
        if (h->dados[i].distancia >= h->dados[pai].distancia)
        {
            break;
        }
        troca(h, i, pai);
        i = pai;
    }
}

static void desce(MinHeap *h, int i)
{
    while (1)
    {
        int menor = i;
        int esq = 2 * i + 1;
        int dir = 2 * i + 2;

        if (esq < h->tamanho && h->dados[esq].distancia < h->dados[menor].distancia)
        {
            menor = esq;
        }
        if (dir < h->tamanho && h->dados[dir].distancia < h->dados[menor].distancia)
        {
            menor = dir;
        }
        if (menor == i)
        {
            break;
        }
        troca(h, i, menor);
        i = menor;
    }
}

MinHeap *criar_min_heap(int capacidade)
{
    MinHeap *h = malloc(sizeof(MinHeap));
    h->dados = malloc(capacidade * sizeof(NoHeap));
    h->posicao = malloc(capacidade * sizeof(int));
    h->capacidade = capacidade;
    h->tamanho = 0;

    for (int i = 0; i < capacidade; i++)
    {
        h->posicao[i] = -1;
    }

    return h;
}

void inserir_min_heap(MinHeap *h, int vertice, int distancia)
{
    if (h->tamanho == h->capacidade)
    {
        return;
    }

    int i = h->tamanho++;
    h->dados[i].vertice = vertice;
    h->dados[i].distancia = distancia;
    h->posicao[vertice] = i;

    sobe(h, i);
}

NoHeap extrair_minimo(MinHeap *h)
{
    NoHeap raiz = h->dados[0];

    h->posicao[raiz.vertice] = -1;

    h->tamanho--;
    if (h->tamanho > 0)
    {
        h->dados[0] = h->dados[h->tamanho];
        h->posicao[h->dados[0].vertice] = 0;
        desce(h, 0);
    }

    return raiz;
}

void diminuir_chave(MinHeap *h, int vertice, int nova_distancia)
{
    int i = h->posicao[vertice];
    if (i == -1)
    {
        return;
    }

    h->dados[i].distancia = nova_distancia;
    sobe(h, i);
}

int min_heap_vazia(MinHeap *h)
{
    return h->tamanho == 0;
}

void liberar_min_heap(MinHeap *h)
{
    free(h->dados);
    free(h->posicao);
    free(h);
}

/* ----- Dijkstra ----- */

void dijkstra(GrafoListaPonderado *g, int origem, int *dist, int *pred)
{
    MinHeap *h = criar_min_heap(g->n);

    for (int v = 0; v < g->n; v++)
    {
        dist[v] = INFINITO;
        pred[v] = -1;
        inserir_min_heap(h, v, INFINITO);
    }

    dist[origem] = 0;
    diminuir_chave(h, origem, 0);

    while (!min_heap_vazia(h))
    {
        NoHeap minimo = extrair_minimo(h);
        int u = minimo.vertice;

        if (dist[u] == INFINITO)
        {
            continue;
        }

        NoAdj *atual = g->adj[u];
        while (atual != NULL)
        {
            int v = atual->destino;
            int peso = atual->peso;

            if (h->posicao[v] != -1 && dist[u] + peso < dist[v])
            {
                dist[v] = dist[u] + peso;
                pred[v] = u;
                diminuir_chave(h, v, dist[v]);
            }

            atual = atual->prox;
        }
    }

    liberar_min_heap(h);
}

void reconstruir_caminho(int origem, int destino, int *pred)
{
    if (destino != origem && pred[destino] == -1)
    {
        printf("Nao ha caminho de %d a %d.\n", origem, destino);
        return;
    }

    int caminho[1024];
    int tamanho = 0;
    int v = destino;

    while (v != -1)
    {
        caminho[tamanho++] = v;
        if (v == origem)
        {
            break;
        }
        v = pred[v];
    }

    printf("Caminho: ");
    for (int i = tamanho - 1; i >= 0; i--)
    {
        printf("%d", caminho[i]);
        if (i > 0)
        {
            printf(" -> ");
        }
    }
    printf("\n");
}
