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

#include <stdlib.h>
#include <stdio.h>

#include "bombas.h"

#ifndef _CONTROLE_PRINCIPAL
/* Se esse arquivo for compilado diretamente, sem o sistema de
controle principal, cria um sistema de bombas padrão, mostra seu
estado e fecha. */
int main(void)
{
    sistemaDeBombeamento* b = Bombas(NUM_BOMBAS);
    estadoDoBombeamento(b);
    removerBombeamento(b);
    return 0;
}
#endif // _CONTROLE_PRINCIPAL

/* Cria e inicializa um sistema de bombeamento. No início, todas as
bombas estão ativas. */
sistemaDeBombeamento* Bombas(int num_bombas)
{
    sistemaDeBombeamento* bombas = malloc(sizeof(sistemaDeBombeamento));
    bombas->totais = num_bombas;
    bombas->ativas = bombas->totais;
    bombas->estados = malloc(bombas->totais * sizeof(bool));
    // Tenta criar uma lista para guardar os estados. Se isso falhar,
    // retorna um apontador nulo.
    if (bombas->estados == NULL)
    {
        return NULL;
    }
    for (int i = 0; i < bombas->totais; i++)
    {
        bombas->estados[i] = true;
    }
    bombas->luzAmarela = true;
    bombas->luzVermelha = false;
    return bombas;
}

/* Mostra ATIVA se o estado for verdadeiro, INATIVA se for
falso. Utilizado por estadoDoBombeamento. Função local. */
void mostrarEstado(bool estado)
{
    if (estado)
    {
        printf("ATIVA\n");
    }
    else
    {
        printf("INATIVA\n");
    }
}

/* Mostra o estado de todos os componentes de um sistema de
bombeamento no terminal. */
void estadoDoBombeamento(sistemaDeBombeamento* bombas)
{
    printf("Bombas totais: %d\n", bombas->totais);
    printf("Bombas ativas: %d\n", bombas->ativas);
    printf("  Estados das séries de bombas:\n");
    for (int i = 0; i < bombas->totais; i++)
    {
        printf("  | Série %02d: ", i+1);
        mostrarEstado(bombas->estados[i]);
    }
    printf("Luz amarela: ");
    mostrarEstado(bombas->luzAmarela);
    printf("Luz vermelha: ");
    mostrarEstado(bombas->luzVermelha);
}

/* Altera o número de bombas ativas de um sistema de bombeamento,
atualizando ao mesmo tempo o estado de cada série de bombas e da luz
indicadora. */
void alterarBombasAtivas(sistemaDeBombeamento* bombas, int ativas)
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
void emergenciaDoBombeamento(sistemaDeBombeamento* bombas)
{
    alterarBombasAtivas(bombas, 0);
    bombas->luzVermelha = true;
}

/* Desativa o estado de emergência das bombas. */
void normalizacaoDoBombeamento(sistemaDeBombeamento* bombas)
{
    bombas->luzVermelha = false;
}

/* Remove o sistema de bombeamento da memória. */
void removerBombeamento(sistemaDeBombeamento* bombas)
{
    if (bombas->estados != NULL)
    {
        free(bombas->estados);
    }
    free(bombas);
}
