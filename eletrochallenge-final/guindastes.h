#ifndef _GUINDASTES
#define _GUINDASTES
#endif

#include <stdlib.h>
#include <stdbool.h>

// Tempo necessário para um guindaste coletar um novo barril após
// ter carregado outro barril, em minutos. Definido pela equipe.
#define TEMPO_DE_COLETA 2
// Tempo necessário para um guindaste carregar um barril no navio após
// ele ser coletado, em minutos. Definido pela equipe.
#define TEMPO_DE_CARREGAMENTO 3

/** Representação programática de um grupo de guindastes. Em um
sistema real, os valores da variável estados (dos guindastes) seriam
usados para controlar os respectivos dispositivos mecânicos, através
de uma interface controlador -> dispositivo. */
typedef struct {
    // Número de guindastes totais.
    int totais;
    // Número de guindastes ativos.
    int ativos;
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
    // -1 = inativo, aguardando oportunidade, 0 = inativo,
    // 1 = ativo.
    int *estados;
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
guindastes não funcionam 24 h por dia. O horário é dado em horas. */
void atualizarGuindastes(Guindastes *guindastes, int horario);

/* Altera o número de guindastes ativos de um grupo de guindastes,
atualizando ao mesmo tempo o estado de cada guindaste. Tem preferência
por desativar guindastes com menor progresso, e reativas guindastes
com maior progresso, economizando energia a longo prazo. */
void alterarGuindastesAtivos(Guindastes *guindastes, int ativos);

/* Tenta atualizar o valor da capacidade do navio de um grupo de
guindastes quando um novo navio cehga. A capacidade é um valor
inteiro, e representa a quantidade de barris que o novo navio ainda pode
comportar. Retorna true se o novo navio pôde ser atracado, falso se
não, ou seja, quando outro navio ainda estava no porto. */
bool atualizarNavio(Guindastes *guindastes, int capacidade);

/* Remove o grupo de guindastes da memória. */
void removerGuindastes(Guindastes *guindastes);
