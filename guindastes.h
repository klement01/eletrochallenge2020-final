#ifndef _GUINDASTES
#define _GUINDASTES

#include <stdbool.h>

/* Número de guindastes, definido pelo desafio. */
#define NUM_GUINDASTES 10
/* Potência de cada guindaste, definida pela equipe, em kW por
guindaste ativo. */
#define P_GUINDASTE 50 // TODO: Calcular potências dos guindastes.

/* Tempo necessário para um guindaste coletar um novo barril após
ter carregado outro barril, em minutos. Definido pela equipe */
#define TEMPO_DE_COLETA 2 // TODO: Calcular tempo de coleta.
/* Tempo necessário para um guindaste carregar um barril no navio após
ele ser coletado, em minutos. Definido pela equipe. */
#define TEMPO_DE_CARREGAMENTO 3 // TODO: Calcular tempo de carregamento.
/* Capacidade do navio, determinado pela equipe. */
#define CAPACIDADE_DO_NAVIO 1000 // TODO: Calcular capacidade do navio.

/** Representação programática de um grupo de guindastes. Em um
sistema real, os valores da variável estados (dos guindastes) seriam
usados para controlar os respectivos dispositivos mecânicos, através
de uma interface controlador -> dispositivo. */
typedef struct {
    // Número de guindastes totais.
    int totais;
    // Número de guindastes ativos.
    int ativos;
    // Número máximo de guindastes ativos, definido pelo suprimento de
    // energia.
    int ativosMax;
    // Número de guindastes que estão, no momento, carregando um
    // barril no navio.
    int carregando;
    // Número que representa o estado do navio atracado junto aos
    // guindastes. Quando igual a zero, não há um navio atracado.
    // Quando positivo, indica o número de barris que o navio atracado
    // ainda pode comportar.
    int estadoDoNavio;
    // Número que representa o progresso de um determinado guindaste,
    // no processo de carregamento de um barril ou coleta de um novo
    // barril. Quando negativo, o guindaste está procurando um novo
    // barril; quando positivo, o guindaste está carregando um barril
    // no navio.
    int *progressos;
    // Lista que representa o estado de cada guindaste, onde
    // true = ativo, false = inativo.
    bool *estados;
} Guindastes;

/** Protótipos das funções públicas, utilizadas pelo controlador
principal. */

/* Cria e inicializa um grupo de guindastes. No início, todos os
guindastes estão inativos e prontos para carregar um barril. */
Guindastes *CriarGuindastes(int num_guindastes);

/* Mostra o estado de todos os componentes de um grupo de guindastes
no terminal. */
void estadoDosGuindastes(Guindastes *guindastes);

/* Avança o estado de todos os componentes do grupo de guindastes
em um minuto. A alteração dos estados depende do horário, já que os
guindastes não funcionam 24 h por dia. O horário é dado em horas.
Retorna true se o navio atracado ainda tiver capacidade após o
carregamento, false se não houver um navio atracado ou se o navio
atracado estiver cheio. */
bool atualizarGuindastes(Guindastes *guindastes, int horario);

/* Tenta atualizar o valor da capacidade do navio de um grupo de
guindastes quando um novo navio cehga. A capacidade é um valor
inteiro, e representa a quantidade de barris que o novo navio ainda
pode comportar. Retorna true se o novo navio pôde ser atracado, falso
se não, ou seja, quando outro navio ainda estava no porto. */
bool atualizarNavio(Guindastes *guindastes, int capacidade);

/* Remove o grupo de guindastes da memória. */
void removerGuindastes(Guindastes *guindastes);

#endif // _GUINDASTES
