#include <stdio.h>
#include <stdlib.h>
#include "grafo_lista.h"

static void remover_da_lista(No **lista, int destino)
{
    No *atual = *lista;
    No *anterior = NULL;

    while (atual != NULL) {
        if (atual->destino == destino) {
            if (anterior == NULL) {
                *lista = atual->prox;
            } else {
                anterior->prox = atual->prox;
            }
            free(atual);
            return;
        }
        anterior = atual;
        atual = atual->prox;
    }
}

GrafoLista *criar_grafo_lista(int n)
{
    GrafoLista *g = malloc(sizeof(GrafoLista));
    g->n = n;
    g->adj = calloc(n, sizeof(No *));
    return g;
}

void inserir_aresta_lista(GrafoLista *g, int u, int v)
{
    if (u < 0 || u >= g->n || v < 0 || v >= g->n) {
        return;
    }

    No *novo_u = malloc(sizeof(No));
    novo_u->destino = v;
    novo_u->prox = g->adj[u];
    g->adj[u] = novo_u;

    No *novo_v = malloc(sizeof(No));
    novo_v->destino = u;
    novo_v->prox = g->adj[v];
    g->adj[v] = novo_v;
}

void remover_aresta_lista(GrafoLista *g, int u, int v)
{
    if (u < 0 || u >= g->n || v < 0 || v >= g->n) {
        return;
    }

    remover_da_lista(&g->adj[u], v);
    remover_da_lista(&g->adj[v], u);
}

int grau_lista(GrafoLista *g, int v)
{
    int grau = 0;
    No *atual = g->adj[v];

    while (atual != NULL) {
        grau++;
        atual = atual->prox;
    }

    return grau;
}

int sao_adjacentes_lista(GrafoLista *g, int u, int v)
{
    No *atual = g->adj[u];

    while (atual != NULL) {
        if (atual->destino == v) {
            return 1;
        }
        atual = atual->prox;
    }

    return 0;
}

void liberar_grafo_lista(GrafoLista *g)
{
    for (int i = 0; i < g->n; i++) {
        No *atual = g->adj[i];
        while (atual != NULL) {
            No *tmp = atual;
            atual = atual->prox;
            free(tmp);
        }
    }
    free(g->adj);
    free(g);
}

void exibir_grafo_lista(GrafoLista *g)
{
    printf("Lista de Adjacencia:\n");
    for (int i = 0; i < g->n; i++) {
        printf("%d:", i);
        No *atual = g->adj[i];
        while (atual != NULL) {
            printf(" -> %d", atual->destino);
            atual = atual->prox;
        }
        printf("\n");
    }
}
