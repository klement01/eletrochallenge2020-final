/** Controla os guindastes, de acordo com as especificaçes do desafio.
 *  A ativação e desativação dos guindastes é feita de modo a carregar
 *  o navio o mais rápido possível, respeitando os limites impostos
 *  pelo controlador de energia.
 *  Em seu estado padrão, todas os guindastes estão operando.
 *  Quando fora do horário de funcionamento (06:00 - 14:00 e 18:00 -
 *  00:00), ou quando não há um navio atracado, os guindastes são
 *  parados.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "guindastes.h"

/* Cria e inicializa um grupo de guindastes. No início, todos os
guindastes estão inativos, mas prontos para carregar um barril. */
Guindastes *CriarGuindastes(int num_guindastes)
{
    // Se o número de guindastes for inválido (menor que 1) retorna
    // um apontador nulo.
    if (num_guindastes < 1)
    {
        return NULL;
    }
    // Tenta reservar espaço para um grupo de guindastes. Se isso
    // falhar, retorn um apontador nulo.
    Guindastes *guindastes = malloc(sizeof(Guindastes));
    if (guindastes == NULL)
    {
        return NULL;
    }
    // Tenta criar listas para guardar os estados e os progressos.
    // Se isso falhar, retorna um apontador nulo.
    guindastes->progressos = malloc(num_guindastes * sizeof(int));
    guindastes->estados = malloc(num_guindastes * sizeof(bool));
    if (guindastes->progressos == NULL || guindastes->estados == NULL)
    {
        return NULL;
    }
    for (int i = 0; i < num_guindastes; i++)
    {
        guindastes->progressos[i] = -TEMPO_DE_COLETA;
        guindastes->estados[i] = false;
    }
    // Inicializa o restante das variáveis.
    guindastes->totais = num_guindastes;
    guindastes->ativos = 0;
    guindastes->ativosMax = num_guindastes;
    guindastes->carregando = 0;
    guindastes->estadoDoNavio = 0;
    return guindastes;
}

/* Mostra ATIVO se o estado for verdadeiro, INATIVO se for
falso. Utilizado por estadoDosGuindastes. Função local. */
static void mostrarEstadoDoGuindaste(bool estado)
{
    if (estado)
    {
        printf("ATIVO");
    }
    else
    {
        printf("INATIVO");
    }
}

/* Mostra o estado de todos os componentes de um grupo de guindastes
no terminal. */
void estadoDosGuindastes(Guindastes *guindastes)
{
    printf("Guindastes totais: %d\n", guindastes->totais);
    printf("Guindastes ativos: %d\n", guindastes->ativos);
    printf("Guindastes ativos (max): %d\n", guindastes->ativosMax);
    printf("Guindastes carregando: %d\n", guindastes->carregando);
    printf("  Guindastes: estado (progresso)\n");
    for (int i = 0; i < guindastes->totais; i++)
    {
        printf("  | Guindaste %02d: ", i+1);
        mostrarEstadoDoGuindaste(guindastes->estados[i]);
        printf(" (%02d)\n", guindastes->progressos[i]);
    }
    printf("Estado do navio: %d\n", guindastes->estadoDoNavio);
}

/* Desativa todos os guindastes. Função local. */
static void desativarTodosOsGuindastes(Guindastes *guindastes)
{
    guindastes->ativos = 0;
    for (int i = 0; i < guindastes->totais; i ++)
    {
        guindastes->estados[i] = false;
    }
}

/* Altera o número máximo de guindastes ativos de um grupo de
guindastes, atualizando ao mesmo tempo o estado de cada guindaste.
Tem preferência por desativar guindastes com menor progresso, e
reativar guindastes com maior progresso, economizando energia a longo
prazo. Função local. */
static void alterarGuindastesAtivos(Guindastes *guindastes)
{
    // Primeiro, desativa todos os guindastes.
    desativarTodosOsGuindastes(guindastes);
    // Então, reativa guindaste até o número de guindastes ativos
    // ser igual ao número de guindastes ativos máximo.
    while (guindastes->ativos != guindastes->ativosMax)
    {
        guindastes->ativos++;
        int maiorIndice, maiorProgresso;
        // Acha um guindaste inativo e guarda seu índice e progresso.
        for (int i = 0; i < guindastes->totais; i++)
        {
            if (!guindastes->estados[i])
            {
                maiorIndice = i;
                maiorProgresso = guindastes->progressos[i];
                break;
            }
        }
        // Então, tenta achar o guindaste inativo com maior progresso.
        for (int i = maiorIndice + 1; i < guindastes->totais; i++)
        {
            if (!guindastes->estados[i]
                && guindastes->progressos[i] > maiorProgresso)
            {
                maiorIndice = i;
                maiorProgresso = guindastes->progressos[i];
            }
        }
        // Finalmente, ativa o guindaste com maior progresso encontrado.
        guindastes->estados[maiorIndice] = true;
    }
}

/* Avança o estado de todos os componentes do grupo de guindastes
em um minuto. A alteração dos estados depende do horário, já que os
guindastes não funcionam 24 h por dia. O horário é dado em horas. */
bool atualizarGuindastes(Guindastes *guindastes, int horario)
{
    // Se o horário estiver fora dos horários de funcionamento dos
    // guindastes ou não houver um navio atracado, desativa todos
    // eles.
    if (guindastes->estadoDoNavio == 0
        || horario < 6
        || (horario > 14 && horario < 18)
        || horario > 24)
    {
        desativarTodosOsGuindastes(guindastes);
        return guindastes->estadoDoNavio != 0;
    }
    // Se o horário estiver dentro dos horários de funcionamento dos
    // guindastes e houver um navio atracado, atualiza o número de
    // guindastes ativos, para que seja igual ao número máximo de
    // guindastes ativos.
    alterarGuindastesAtivos(guindastes);
    // Atualiza a posição de cada guindaste.
    for (int i = 0; i < guindastes->totais; i++)
    {
        // Se um guindaste está inativo, ele continua parado.
        if (!guindastes->estados[i])
        {
            continue;
        }
        // Se o guindaste tiver chegado na posição original, decide
        // se vai carregar um barril ou ficar parado.
        if (guindastes->progressos[i] == 0)
        {
            // Se o número de guindastes carregando barris for o
            // suficiente para encher o barco, o guindaste fica
            // parado.
            if (guindastes->carregando >= guindastes->estadoDoNavio)
            {
                guindastes->ativos--;
                guindastes->estados[i] = false;
                continue;
            }
            // Se não, o guindaste começa a carregar um barril.
            else
            {
                guindastes->carregando++;
            }
        }
        // Se o guindaste tiver terminado de carregar um barril,
        // diminui o número de guindastes carregando barris e faz
        // eles começar a coletar outro.
        else if (guindastes->progressos[i] == TEMPO_DE_CARREGAMENTO - 1)
        {
            guindastes->carregando--;
            guindastes->progressos[i] = -1 - TEMPO_DE_COLETA;
            guindastes->estadoDoNavio--;
        }
        // A posição do guindaste é avançanda em um passo.
        /** Em um sistema real, a posição do guindaste poderia ser
        determinada empiricamente, em vez de ser simulada. */
        guindastes->progressos[i]++;
    }
    return true;
}

/* Atualiza o valor da capacidade do navio de um grupo de guindastes
quando um novo navio é atracado. A capacidade é um valor inteiro,
e representa a quantidade de barris que o novo navio ainda pode
comportar. */
bool atualizarNavio(Guindastes *guindastes, int capacidade)
{
    if (guindastes->estadoDoNavio == 0)
    {
        guindastes->estadoDoNavio = capacidade;
        return true;
    }
    return false;
}

/* Remove o grupo de guindastes da memória. */
void removerGuindastes(Guindastes *guindastes)
{
    free(guindastes->progressos);
    free(guindastes->estados);
    free(guindastes);
}
