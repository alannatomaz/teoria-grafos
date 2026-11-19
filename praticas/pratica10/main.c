#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "hamiltoniano.h"
#include "tps.h"

/* Grafo de Petersen: 10 vertices, tem caminho hamiltoniano mas NAO tem ciclo hamiltoniano */
static Grafo *grafo_petersen(void)
{
    Grafo *g = criar_grafo(10);
    for (int i = 0; i < 5; i++)
    {
        inserir_aresta(g, i, (i + 1) % 5);         // ciclo de fora 0-1-2-3-4
        inserir_aresta(g, i, i + 5);               // raios
        inserir_aresta(g, i + 5, (i + 2) % 5 + 5); // estrela de dentro 5-7-9-6-8
    }
    return g;
}

/* "Casinha": quadrado 0-1-2-3 com telhado 4 ligado em 0 e 1. Tem ciclo hamiltoniano. */
static Grafo *grafo_casinha(void)
{
    Grafo *g = criar_grafo(5);
    inserir_aresta(g, 0, 1);
    inserir_aresta(g, 1, 2);
    inserir_aresta(g, 2, 3);
    inserir_aresta(g, 3, 0);
    inserir_aresta(g, 0, 4);
    inserir_aresta(g, 1, 4);
    return g;
}

static void testar_hamiltoniano(const char *nome, Grafo *g)
{
    int *seq = malloc(g->n * sizeof(int));

    printf("\n--- %s (n = %d) ---\n", nome, g->n);

    printf("Caminho hamiltoniano: ");
    if (encontrar_caminho_hamiltoniano_bt(g, seq))
    {
        imprimir_sequencia(g->n, seq, 0);
    }
    else
    {
        printf("nao existe");
    }
    printf("\n");

    printf("Ciclo hamiltoniano:   ");
    if (encontrar_ciclo_hamiltoniano_bt(g, seq))
    {
        imprimir_sequencia(g->n, seq, 1);
    }
    else
    {
        printf("nao existe");
    }
    printf("\n");

    /* mesma pergunta usando a reducao pro TSP */
    MatrizCustos *m = grafo_para_tsp(g);
    int custo = tsp_forca_bruta(m, seq);
    printf("Reducao pro TSP: custo otimo %d -> %s\n", custo,
           custo == g->n ? "igual a n, tem ciclo hamiltoniano" : "maior que n, nao tem ciclo hamiltoniano");
    liberar_matriz_custos(m);

    free(seq);
}

/* cidades em pontos aleatorios de um quadrado 100x100, custo = distancia arredondada */
static MatrizCustos *instancia_aleatoria(int n)
{
    int *x = malloc(n * sizeof(int));
    int *y = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
    {
        x[i] = rand() % 101;
        y[i] = rand() % 101;
    }

    MatrizCustos *m = criar_matriz_custos(n);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            double dx = x[i] - x[j];
            double dy = y[i] - y[j];
            m->custo[i][j] = (int)(sqrt(dx * dx + dy * dy) + 0.5);
        }
    }

    free(x);
    free(y);
    return m;
}

/* tipo 0 = forca bruta, 1 = vizinho mais proximo, 2 = insercao.
   Repete ate passar uns 50 ms pra ter um tempo medio (em ms) mais confiavel. */
static double medir(MatrizCustos *m, int tipo, int *rota, int *custo)
{
    int repeticoes = 0;
    clock_t inicio = clock();
    clock_t fim;
    do
    {
        if (tipo == 0)
        {
            *custo = tsp_forca_bruta(m, rota);
        }
        else if (tipo == 1)
        {
            *custo = tsp_vizinho_mais_proximo(m, 0, rota);
        }
        else
        {
            *custo = tsp_insercao(m, rota);
        }
        repeticoes++;
        fim = clock();
    } while (fim - inicio < CLOCKS_PER_SEC / 20);

    return 1000.0 * (double)(fim - inicio) / CLOCKS_PER_SEC / repeticoes;
}

int main(void)
{
    /* ----- Caminho / Ciclo Hamiltoniano ----- */
    printf("=== Caminho e Ciclo Hamiltoniano (backtracking) ===");

    Grafo *casinha = grafo_casinha();
    testar_hamiltoniano("Casinha", casinha);
    liberar_grafo(casinha);

    Grafo *petersen = grafo_petersen();
    testar_hamiltoniano("Grafo de Petersen", petersen);
    liberar_grafo(petersen);

    /* ----- TSP: exemplo pequeno ----- */
    printf("\n=== TSP: exemplo com 5 cidades ===\n");
    int custos[5][5] = {
        {0, 2, 9, 10, 7},
        {2, 0, 6, 4, 3},
        {9, 6, 0, 8, 5},
        {10, 4, 8, 0, 6},
        {7, 3, 5, 6, 0}
    };
    MatrizCustos *m = criar_matriz_custos(5);
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            m->custo[i][j] = custos[i][j];
        }
    }

    int rota[5];
    int c;

    c = tsp_forca_bruta(m, rota);
    printf("Forca bruta:          custo %3d  rota: ", c);
    imprimir_sequencia(5, rota, 1);
    printf("\n");

    c = tsp_vizinho_mais_proximo(m, 0, rota);
    printf("Vizinho mais proximo: custo %3d  rota: ", c);
    imprimir_sequencia(5, rota, 1);
    printf("\n");

    c = tsp_insercao(m, rota);
    printf("Insercao:             custo %3d  rota: ", c);
    imprimir_sequencia(5, rota, 1);
    printf("\n");

    liberar_matriz_custos(m);

    /* ----- Experimento ----- */
    printf("\n=== Experimento: cidades aleatorias num quadrado 100x100 ===\n");
    printf("%4s | %12s %14s | %12s %14s | %12s %14s\n",
           "n", "custo FB", "tempo FB (ms)", "custo VMP", "tempo VMP (ms)", "custo INS", "tempo INS (ms)");

    srand(10); // semente fixa
    int tamanhos[] = {5, 8, 10, 12};

    for (int k = 0; k < 4; k++)
    {
        int n = tamanhos[k];
        MatrizCustos *inst = instancia_aleatoria(n);
        int *r = malloc(n * sizeof(int));
        int c_fb, c_vmp, c_ins;

        double t_fb = medir(inst, 0, r, &c_fb);
        double t_vmp = medir(inst, 1, r, &c_vmp);
        double t_ins = medir(inst, 2, r, &c_ins);

        printf("%4d | %12d %14.4f | %12d %14.4f | %12d %14.4f\n",
               n, c_fb, t_fb, c_vmp, t_vmp, c_ins, t_ins);

        free(r);
        liberar_matriz_custos(inst);
    }

    printf("\nFB = forca bruta, VMP = vizinho mais proximo, INS = insercao do mais proximo\n");

    return 0;
}
