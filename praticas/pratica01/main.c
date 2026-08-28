#include <stdio.h>
#include "grafo_matriz.h"
#include "grafo_lista.h"

int main(void)
{
       int n = 6;

       printf("- Grafo com Matriz de Adjacencia: \n\n");
       GrafoMatriz *gm = criar_grafo_matriz(n);

       inserir_aresta_matriz(gm, 0, 1);
       inserir_aresta_matriz(gm, 0, 2);
       inserir_aresta_matriz(gm, 1, 2);
       inserir_aresta_matriz(gm, 1, 3);
       inserir_aresta_matriz(gm, 3, 4);
       inserir_aresta_matriz(gm, 4, 5);

       exibir_grafo_matriz(gm);

       printf("\nGrau do vertice 1: %d\n", grau_matriz(gm, 1));
       printf("0 e 2 sao adjacentes? %s\n",
              sao_adjacentes_matriz(gm, 0, 2) ? "Sim" : "Nao");

       remover_aresta_matriz(gm, 0, 2);
       printf("\nDepois de remover a aresta (0,2):\n");
       printf("0 e 2 sao adjacentes? %s\n",
              sao_adjacentes_matriz(gm, 0, 2) ? "Sim" : "Nao");

       liberar_grafo_matriz(gm);

       printf("\n- Grafo com Lista de Adjacencia: \n\n");
       GrafoLista *gl = criar_grafo_lista(n);

       inserir_aresta_lista(gl, 0, 1);
       inserir_aresta_lista(gl, 0, 2);
       inserir_aresta_lista(gl, 1, 2);
       inserir_aresta_lista(gl, 1, 3);
       inserir_aresta_lista(gl, 3, 4);
       inserir_aresta_lista(gl, 4, 5);

       exibir_grafo_lista(gl);

       printf("\nGrau do vertice 1: %d\n", grau_lista(gl, 1));
       printf("0 e 2 sao adjacentes? %s\n",
              sao_adjacentes_lista(gl, 0, 2) ? "Sim" : "Nao");

       remover_aresta_lista(gl, 0, 2);
       printf("\nDepois de remover a aresta (0,2):\n");
       printf("0 e 2 sao adjacentes? %s\n",
              sao_adjacentes_lista(gl, 0, 2) ? "Sim" : "Nao");

       liberar_grafo_lista(gl);

       return 0;
}
