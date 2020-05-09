#ifndef _ENERGIA
#define _ENERGIA

#include <stdbool.h>

#include "bombas.h"
#include "guindastes.h"

/* Número de turbinas eólicas, definido pelo desafio. */
#define NUM_TURBINAS 50
/* Potência dos sistemas auxiliares da plataforma, em kW, definida
pelo desafio. */
#define P_AUXILIAR 3620
/* Potência máxima que pode ser fornecida pela termelétrica, em kW,
calculada pela equipe baseado nos dados fornecidos pelo desafio. */
#define P_TERMELETRICA 40100 // TODO: Calcular a potência da termelétrica.
/* Custo em reais por kWh da energia fornecida pela termelétrica,
calculado pela equipe baseado nos dados fornecidos pelo desafio. */
#define C_TERMELETRICA 1
/* Eficiência dos inversores de frequência, definida pelo desafio. */
#define E_INVERSORES 0.95

/* Dá uma quantidade pré-determinada de passos, e retorna o custo
total dessas etapas. */
double passosN(int passos, Bombas *bombas, Guindastes *guindastes,
               int *hora, int *minuto);

/* Funciona como passosN, mas em vez de dar uma quantidade pré-
-determinada de passos, avança a simulação até o navio atracado
na plataforma atingir sua capacidade. */
double passosNavio(Bombas *bombas, Guindastes *guindastes,
                   int *hora, int *minuto);

/* Simula um passo (um minuto) de operação da plataforma. Retorna
true se há um navio na plataforma, false se não. A fração da
capacidade da termelétrica que é demandada pela plataforma é
colocada no endereço de memória especificado. */
bool passo(Bombas *bombas, Guindastes *guindastes,
           int *hora, int *minuto, double *fracaoDaTermeletrica);

/* Calcula a porcentagem da demanda da termelétrica que deve ser
direcionada para a plataforma de petróleo para a sua operação e, se
necessário, ajusta a quantidade de guindastes que podem ser
ativados. Retorna a fração da potência fornecida pela termelétrica
que deve ser direcionada à plataforma. */
double ajustarDemanda(Bombas *bombas, Guindastes *guindastes,
                      int horario);

/* Calcula a potência que deve ser fornecida pela termelétrica, dado
um horário do dia e uma demanda total, em kW. */
double demandaDaTermeletrica(double demandaTotal, int horario);

/* Calcula a potência gerada pelas turbinas eólicas, em kW, em um
certo horário do dia. */
double potenciaDasTurbinas(int horario);

/* Retorna true se str for uma string numérica, false se não. */
bool strNumerica(char *str);

#endif // _ENERGIA
