#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cobertura.h"

/* grafo aleatorio onde cada aresta existe com probabilidade 'prob' (em %) */
static Grafo *grafo_aleatorio(int n, int prob)
{
    Grafo *g = criar_grafo(n);
    for (int u = 0; u < n; u++)
    {
        for (int v = u + 1; v < n; v++)
        {
            if (rand() % 100 < prob)
            {
                inserir_aresta(g, u, v);
            }
        }
    }
    return g;
}

/* Uma execucao so leva menos que a resolucao do clock(), entao repito
   ate passar uns 50 ms e divido pelo numero de repeticoes (ms por execucao). */
static double medir_bt(Grafo *g, int *cobertura, int *tamanho)
{
    int repeticoes = 0;
    clock_t inicio = clock();
    clock_t fim;
    do
    {
        *tamanho = cobertura_minima_bt(g, cobertura);
        repeticoes++;
        fim = clock();
    } while (fim - inicio < CLOCKS_PER_SEC / 20);

    return 1000.0 * (double)(fim - inicio) / CLOCKS_PER_SEC / repeticoes;
}

static double medir_guloso(Grafo *g, int *cobertura, int *tamanho)
{
    int repeticoes = 0;
    clock_t inicio = clock();
    clock_t fim;
    do
    {
        *tamanho = cobertura_gulosa(g, cobertura);
        repeticoes++;
        fim = clock();
    } while (fim - inicio < CLOCKS_PER_SEC / 20);

    return 1000.0 * (double)(fim - inicio) / CLOCKS_PER_SEC / repeticoes;
}

int main(void)
{
    /* ----- Exemplo pequeno: o guloso erra -----
       0 ligado em 1, 2, 3 e cada um deles ligado numa folha (4, 5, 6).
       O guloso pega o 0 primeiro (maior grau) e depois precisa de 1, 2 e 3: total 4.
       O otimo e so {1, 2, 3}: total 3. */
    Grafo *g = criar_grafo(7);
    inserir_aresta(g, 0, 1);
    inserir_aresta(g, 0, 2);
    inserir_aresta(g, 0, 3);
    inserir_aresta(g, 1, 4);
    inserir_aresta(g, 2, 5);
    inserir_aresta(g, 3, 6);

    int cob[7];

    printf("=== Exemplo: 0 ligado em 1,2,3 e folhas 1-4, 2-5, 3-6 ===\n");

    int t_bt = cobertura_minima_bt(g, cob);
    printf("Backtracking: tamanho %d  ", t_bt);
    imprimir_cobertura(7, cob);
    printf("  valida? %s\n", eh_cobertura(g, cob) ? "sim" : "nao");

    int t_gul = cobertura_gulosa(g, cob);
    printf("Guloso:       tamanho %d  ", t_gul);
    imprimir_cobertura(7, cob);
    printf("  valida? %s\n", eh_cobertura(g, cob) ? "sim" : "nao");

    liberar_grafo(g);

    /* ----- Experimento ----- */
    printf("\n=== Experimento: grafos aleatorios (probabilidade de aresta 50%%) ===\n");
    printf("%4s %8s %8s %8s %16s %16s\n", "n", "arestas", "|BT|", "|guloso|", "tempo BT (ms)", "tempo gul. (ms)");

    srand(42); // semente fixa pra dar sempre o mesmo resultado
    int tamanhos[] = {5, 8, 10, 12, 15};

    for (int k = 0; k < 5; k++)
    {
        int n = tamanhos[k];
        Grafo *ga = grafo_aleatorio(n, 50);
        int *c_bt = malloc(n * sizeof(int));
        int *c_gul = malloc(n * sizeof(int));
        int tam_bt, tam_gul;

        double tempo_bt = medir_bt(ga, c_bt, &tam_bt);
        double tempo_gul = medir_guloso(ga, c_gul, &tam_gul);

        printf("%4d %8d %8d %8d %16.4f %16.4f", n, numero_arestas(ga), tam_bt, tam_gul, tempo_bt, tempo_gul);
        if (!eh_cobertura(ga, c_bt) || !eh_cobertura(ga, c_gul))
        {
            printf("  (ERRO: cobertura invalida)");
        }
        printf("\n");

        free(c_bt);
        free(c_gul);
        liberar_grafo(ga);
    }

    return 0;
}
