#include <stdio.h>
#include <stdlib.h>
#include "agm.h"

/* ----- Union-Find ----- */

DSU *criar_dsu(int n)
{
    DSU *d = malloc(sizeof(DSU));
    d->n = n;
    d->pai = malloc(n * sizeof(int));
    d->rank = malloc(n * sizeof(int));

    for (int i = 0; i < n; i++)
    {
        d->pai[i] = i; // cada vertice comeca sozinho no seu conjunto
        d->rank[i] = 0;
    }

    return d;
}

/* compressao de caminho: no retorno da recursao, todo mundo
   no caminho passa a apontar direto pra raiz */
int find(DSU *d, int x)
{
    if (d->pai[x] != x)
    {
        d->pai[x] = find(d, d->pai[x]);
    }
    return d->pai[x];
}

/* uniao por rank: a arvore mais baixa fica embaixo da mais alta.
   Retorna 1 se juntou, 0 se ja estavam no mesmo conjunto. */
int unir(DSU *d, int x, int y)
{
    int rx = find(d, x);
    int ry = find(d, y);

    if (rx == ry)
    {
        return 0;
    }

    if (d->rank[rx] < d->rank[ry])
    {
        d->pai[rx] = ry;
    }
    else if (d->rank[rx] > d->rank[ry])
    {
        d->pai[ry] = rx;
    }
    else
    {
        d->pai[ry] = rx;
        d->rank[rx]++;
    }

    return 1;
}

void liberar_dsu(DSU *d)
{
    free(d->pai);
    free(d->rank);
    free(d);
}

/* ----- Grafo ponderado nao direcionado ----- */

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
    while (i > 0 && h->dados[i].chave < h->dados[(i - 1) / 2].chave)
    {
        troca(h, i, (i - 1) / 2);
        i = (i - 1) / 2;
    }
}

static void desce(MinHeap *h, int i)
{
    while (1)
    {
        int menor = i;
        int esq = 2 * i + 1;
        int dir = 2 * i + 2;

        if (esq < h->tamanho && h->dados[esq].chave < h->dados[menor].chave)
        {
            menor = esq;
        }
        if (dir < h->tamanho && h->dados[dir].chave < h->dados[menor].chave)
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

void inserir_min_heap(MinHeap *h, int vertice, int chave)
{
    if (h->tamanho == h->capacidade)
    {
        return;
    }

    int i = h->tamanho++;
    h->dados[i].vertice = vertice;
    h->dados[i].chave = chave;
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

void diminuir_chave(MinHeap *h, int vertice, int nova_chave)
{
    int i = h->posicao[vertice];
    if (i == -1)
    {
        return;
    }

    h->dados[i].chave = nova_chave;
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

/* ----- Kruskal ----- */

static int compara_arestas(const void *a, const void *b)
{
    const Aresta *x = a;
    const Aresta *y = b;
    return (x->peso > y->peso) - (x->peso < y->peso);
}

/* Ordena as arestas pelo peso (O(E log E)) e vai pegando da menor pra maior,
   so aceitando a aresta se ela liga dois conjuntos diferentes (nao forma ciclo).
   Obs: o vetor 'arestas' fica ordenado depois da chamada.
   Retorna o custo total; 'agm' recebe as arestas escolhidas e 'qtd_agm' quantas foram. */
int kruskal(int n, Aresta *arestas, int m, Aresta *agm, int *qtd_agm)
{
    qsort(arestas, m, sizeof(Aresta), compara_arestas);

    DSU *d = criar_dsu(n);
    int custo = 0;
    *qtd_agm = 0;

    for (int i = 0; i < m && *qtd_agm < n - 1; i++)
    {
        if (unir(d, arestas[i].u, arestas[i].v))
        {
            agm[(*qtd_agm)++] = arestas[i];
            custo += arestas[i].peso;
        }
    }

    liberar_dsu(d);
    return custo;
}

/* ----- Prim ----- */

/* Comeca pela raiz e a cada passo tira do heap o vertice mais barato de ligar
   na arvore, atualizando a chave dos vizinhos que ainda estao fora.
   pai[v] guarda a aresta (pai[v], v) da arvore. Retorna o custo total.
   Se o grafo for desconexo, so monta a arvore da componente da raiz. */
int prim(GrafoListaPonderado *g, int raiz, int *pai)
{
    int *chave = malloc(g->n * sizeof(int));
    MinHeap *h = criar_min_heap(g->n);

    for (int v = 0; v < g->n; v++)
    {
        chave[v] = INFINITO;
        pai[v] = -1;
        inserir_min_heap(h, v, INFINITO);
    }

    chave[raiz] = 0;
    diminuir_chave(h, raiz, 0);

    int custo = 0;

    while (!min_heap_vazia(h))
    {
        int u = extrair_minimo(h).vertice;

        if (chave[u] == INFINITO)
        {
            continue; // nao da pra alcancar a partir da raiz
        }

        custo += chave[u];

        for (NoAdj *a = g->adj[u]; a != NULL; a = a->prox)
        {
            int v = a->destino;

            /* posicao != -1 quer dizer que v ainda nao entrou na arvore */
            if (h->posicao[v] != -1 && a->peso < chave[v])
            {
                chave[v] = a->peso;
                pai[v] = u;
                diminuir_chave(h, v, a->peso);
            }
        }
    }

    free(chave);
    liberar_min_heap(h);
    return custo;
}
