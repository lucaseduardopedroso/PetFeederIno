#include <stdio.h>
#include <stdlib.h>

/*------------------------------*/
/* Problema da Mochila Limitado
* ------------------------------
* Capacidade da Mochila de um viajante é de 400kg e ele precisa escolher os items melhores para serem levados em um acampamento.
*
* */


/* Estrutura que representa um item a ser selecionado */ 

typedef struct {

    int code; // tipo código de barra  
    char *name; // nome do item
    int weight; // o quanto pesa
    int value; // o preço
    int count; // quantidade do mesmo item levados

} item_t;

// Matriz de Exemplo
 
item_t items[] = {

    /* Esse é o formato que um arquivo deveria ter é só escrever em um txt e fazer ser lido por uma matriz em um laço for,
    preferi traabalhar com a matriz direto em vez do arquivo txt, por causa das cores. */
    {1, "mapa",                      9,   150,   1},
    {2, "bussula",                  13,    35,   1},
    {3, "agua",                    153,   200,   2},
    {4, "sanduiche",                50,    60,   2},
    {5, "remedio",                  15,    60,   2},
    {6, "panela",                   68,    45,   3},
    {7, "banana",                   27,    60,   3},
    {8, "maça",                     39,    40,   3},
    {9, "queijo",                   23,    30,   1},
    {10, "cerveja",                 52,    10,   3},
    {11, "pasta",                   11,    70,   1},
    {12, "camera",                  32,    30,   1},
    {13, "camiseta",                24,    15,   2},
    {14, "calça",                   48,    10,   2},
    {15, "guarda-chuva",            73,    40,   1},
    {16, "botas ipermeáveis",       42,    70,   1},
    {17, "arma",                    43,    75,   1},
    {18, "livro",                   22,    80,   1},
    {19, "óculos",                  7,    20,   1},
    {20, "toalha",                  18,    12,   2},
    {21, "meias",                    4,    50,   1},
    {22, "cobertor",                 30,    10,   2},

};


// O Resultado esperado para esse problema é:

/*
1 mapa                       1     9   150
2 bussula                    1    13    35
3 agua                       1   153   200
5 remedio                    2    30   120
7 banana                     3    81   180
9 queijo                     1    23    30
11 pasta                     1    11    70
17 arma                      1    43    75
18 livro                     1    22    80
19 óculos                    1     7    20
21 meias                     1     4    50
count, weight, value:        14   396  1010     [Quantidade de itens = 14] [Peso total = 396]  e [Valor de todos os items = 1010]
*/


/* Matriz de Pokemon 
*
*   Ash é um treinador Pokemon e esta ajudando o professor Carvalho, para isso ele precisa enviar o máximo de Pokemons possível para o laboratório do professor.
    A mochila de Ash suporta 400kg.
* */


typedef struct {

    int pokecode; // tipo código de barra  
    char *pokename; // nome do pokemon
    int pokeweight; // o quanto pesa
    int pokevalue; // o nível de poder
    int count; // quantidade do mesmo pokemon levados

} pokemon_t;



pokemon_t pokedex[] = {

    /* Esse é o formato que um arquivo deveria ter é só escrever em um txt e fazer ser lido por uma matriz em um laço for,
    preferi traabalhar com a matriz direto em vez do arquivo txt, por causa das cores. */
    {1, "bulbassaur",               6,   45,   1},
    {2, "snubbull",                 8,   60,   1},
    {3, "squirtle",                 9,   44,   2},
    {4, "metapod",                  10,  50,   2},
    {5, "vulpix",                   38,    60,   2},
    {6, "pikachu",                  6,    45,   3},
    {7, "zubat",                    8,    60,   3},
    {8, "meltan",                   8,    40,   3},
    {9, "aggron",                   360,    30,   1},
    {10, "gyrados",                 335,    10,   3},
    {11, "turtonator",              212,    70,   1},
    {12, "tyranitar",               203,    30,   1},
    {13, "arcanine",                155,    15,   2},
    {14, "evee",                    7,    10,   2},
    {15, "magikarp",                10,    40,   1},
    {16, "tapu koko",               21,    70,   1},
    {17, "raichu",                  21,    75,   1},
    {18, "dragonair",               17,    80,   1},
    {19, "komala",                  20,    20,   1},
    {20, "ditto",                   4,    12,   2},
    {21, "rockruff",                11,    50,   1},

};



 
int n = sizeof (items) / sizeof (item_t);
 
int *knapsack (int w) {
    int i, j, k, v, *mm, **m, *s;
    mm = calloc((n + 1) * (w + 1), sizeof (int));
    m = malloc((n + 1) * sizeof (int *));
    m[0] = mm;
    for (i = 1; i <= n; i++) {
        m[i] = &mm[i * (w + 1)];
        for (j = 0; j <= w; j++) {
            m[i][j] = m[i - 1][j];
            for (k = 1; k <= items[i - 1].count; k++) {
                if (k * items[i - 1].weight > j) {
                    break;
                }
                v = m[i - 1][j - k * items[i - 1].weight] + k * items[i - 1].value;
                if (v > m[i][j]) {
                    m[i][j] = v;
                }
            }
        }
    }
    s = calloc(n, sizeof (int));
    for (i = n, j = w; i > 0; i--) {
        int v = m[i][j];
        for (k = 0; v != m[i - 1][j] + k * items[i - 1].value; k++) {
            s[i - 1]++;
            j -= items[i - 1].weight;
        }
    }
    free(mm);
    free(m);
    return s;
}
 
int main () {
    int i, tc = 0, tw = 0, tv = 0, *s;
    s = knapsack(400);
    for (i = 0; i < n; i++) {
        if (s[i]) {
            printf("%d %-22s %5d %5d %5d\n", items[i].code, items[i].name, s[i], s[i] * items[i].weight, s[i] * items[i].value);
            tc += s[i];
            tw += s[i] * items[i].weight;
            tv += s[i] * items[i].value;
        }
    }
    printf("%-22s %5d %5d %5d\n", "count, weight, value:", tc, tw, tv);
    return 0;
}