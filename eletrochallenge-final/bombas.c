/** Controla as bombas, de acordo com as especificaçes do desafio.
 *  Além disso, controla as luzes indicadoras de operação e de
 *  emergência.
 *  Em seu estado padrão, todas as bombas estão operando, e a luz
 *  amarela está acesa.
 *  Caso o botão de emergência seja pressionado, as bombas são
 *  desligadas, a luz vermelha é acessa, e a luz amarela é desligada.
 *  Ao receber comandos do controlador principal, o número de bombas
 *  ativas pode mudar, e a luz amarela é alterada de acordo.
 */

#include <stdio.h>

#include "bombas.h"

/*
int main(void)
{
    Bombas *b = CriarBombas(10);
    estadoDoBombeamento(b);
    removerBombeamento(b);
    return 0;
}
*/

/* Cria e inicializa um sistema de bombeamento. No início, todas as
bombas estão ativas. */
Bombas *CriarBombas(int num_bombas)
{
    // Se o número de bombas for inválido (menor que 1) retorna
    // um apontador nulo.
    if (num_bombas < 1)
    {
        return NULL;
    }
    // Tenta reservar espaço para um sistema de bombeamento. Se isso
    // falhar, retorn um apontador nulo.
    Bombas *bombas = malloc(sizeof(Bombas));
    if (bombas == NULL)
    {
        return NULL;
    }
    // Tenta criar uma lista para guardar os estados. Se isso falhar,
    // retorna um apontador nulo.
    bombas->estados = malloc(num_bombas * sizeof(bool));
    if (bombas->estados == NULL)
    {
        return NULL;
    }
    for (int i = 0; i < num_bombas; i++)
    {
        bombas->estados[i] = true;
    }
    // Inicializa o restante das variáveis.
    bombas->totais = num_bombas;
    bombas->ativas = num_bombas;
    bombas->luzAmarela = true;
    bombas->luzVermelha = false;
    return bombas;
}

/* Mostra ATIVA se o estado for verdadeiro, INATIVA se for
falso. Utilizado por estadoDoBombeamento. Função local. */
void mostrarEstadoDaBomba(bool estado)
{
    if (estado)
    {
        printf("ATIVA");
    }
    else
    {
        printf("INATIVA");
    }
    printf("\n");
}

/* Mostra o estado de todos os componentes de um sistema de
bombeamento no terminal. */
void estadoDoBombeamento(Bombas *bombas)
{
    printf("Bombas totais: %d\n", bombas->totais);
    printf("Bombas ativas: %d\n", bombas->ativas);
    printf("  Estados das séries de bombas:\n");
    for (int i = 0; i < bombas->totais; i++)
    {
        printf("  | Série %02d: ", i+1);
        mostrarEstadoDaBomba(bombas->estados[i]);
    }
    printf("Luz amarela: ");
    mostrarEstadoDaBomba(bombas->luzAmarela);
    printf("Luz vermelha: ");
    mostrarEstadoDaBomba(bombas->luzVermelha);
}

/* Altera o número de bombas ativas de um sistema de bombeamento,
atualizando ao mesmo tempo o estado de cada série de bombas e da luz
indicadora. */
void alterarBombasAtivas(Bombas *bombas, int ativas)
{
    // Bloqueia a ativação das bombas caso o modo de emergência
    // esteja ativado.
    if (bombas->luzVermelha)
    {
        return;
    }
    bombas->ativas = ativas;
    for (int i = 0; i < bombas->totais; i++)
    {
        bombas->estados[i] = i < ativas;
    }
    bombas->luzAmarela = ativas;
}

/* Ativa o estado de emergência das bombas. */
void emergenciaDoBombeamento(Bombas *bombas)
{
    alterarBombasAtivas(bombas, 0);
    bombas->luzVermelha = true;
}

/* Desativa o estado de emergência das bombas. */
void normalizacaoDoBombeamento(Bombas *bombas)
{
    bombas->luzVermelha = false;
}

/* Retorna o número de bombas ativas, para o cálculo da energia
consumida. */
int bombasAtivas(Bombas *bombas)
{
    return bombas->ativas;
}

/* Remove o sistema de bombeamento da memória. */
void removerBombeamento(Bombas *bombas)
{
    free(bombas->estados);
    free(bombas);
}
