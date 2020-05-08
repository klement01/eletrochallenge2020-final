#ifndef _BOMBAS
#define _BOMBAS
#endif

#include <stdbool.h>

// Número de bombas padrão, definido pelo desafio.
#define NUM_BOMBAS 25

/** Representação programática de um sistema de bombeamento. Em um
sistema real, os valores das variáveis estados (das bombas), luzAmarela
e luzVermelha seriam usados para controlar os respectivos dispositivos
mecânicos, através de uma interface controlador -> dispositivo. */
typedef struct {
    // Número de bombas totais.
    int totais;
    // Número de bombas ativas.
    int ativas;
    // Nos itens seguintes, true = ativa, false = inativa.
    // Array que representa o estado de cada bomba.
    bool *estados;
    // Ativada quando ativas > 0.
    bool luzAmarela;
    // Ativada quando o botão é pressionado.
    bool luzVermelha;
} sistemaDeBombeamento;

/** Protótipos das funções públicas, utilizadas pelo controlador
principal. */

/* Cria e inicializa um sistema de bombeamento. No início, todas as
bombas estão ativas. */
sistemaDeBombeamento* Bombas(int num_bombas);

/* Mostra o estado de todos os componentes de um sistema de
bombeamento no terminal. */
void estadoDoBombeamento(sistemaDeBombeamento* bombas);

/* Altera o número de bombas ativas de um sistema de bombeamento,
atualizando ao mesmo tempo o estado de cada série de bombas e da luz
indicadora. */
void alterarBombasAtivas(sistemaDeBombeamento* bombas, int ativas);

/* Ativa o estado de emergência das bombas. */
void emergenciaDoBombeamento(sistemaDeBombeamento* bombas);

/* Desativa o estado de emergência das bombas. */
void normalizacaoDoBombeamento(sistemaDeBombeamento* bombas);

/* Remove o sistema de bombeamento da memória. */
void removerBombeamento(sistemaDeBombeamento* bombas);
