//TODO: documentação da classe.

#include <stdio.h>

#include "guindastes.h"

int main(void)
{
    Guindastes *g = CriarGuindastes(10);
    estadoDosGuindastes(g);
    removerGuindastes(g);
    return 0;
}

/* Cria e inicializa um grupo de guindastes. No início, todos os
guindastes estão inativos e prontos para carregar um barril. */
Guindastes *CriarGuindastes(int num_guindastes)
{
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
    guindastes->estados = malloc(num_guindastes * sizeof(int));
    if (guindastes->progressos == NULL || guindastes->estados == NULL)
    {
        return NULL;
    }
    for (int i = 0; i < num_guindastes; i++)
    {
        guindastes->progressos[i] = 0;
        guindastes->estados[i] = -1;
    }
    // Inicializa o restante das variáveis.
    guindastes->totais = num_guindastes;
    guindastes->ativos = 0;
    guindastes->carregando = 0;
    guindastes->estadoDoNavio = 0;
    return guindastes;
}

/* Mostra ATIVO se o estado for verdadeiro, INATIVO se for
falso. Utilizado por estadoDosGuindastes. Função local. */
void mostrarEstadoDoGuindaste(bool estado)
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
    printf("  Guindastes: estado (progresso)\n");
    for (int i = 0; i < guindastes->totais; i++)
    {
        printf("  | Guindaste %02d: ", i+1);
        mostrarEstadoDoGuindaste(guindastes->estados[i]);
        printf(" (%02d)\n", guindastes->progressos[i]);
    }
}

/* Avança o estado de todos os componentes do grupo de guindastes
em um minuto. A alteração dos estados depende do horário, já que os
guindastes não funcionam 24 h por dia. O horário é dado em horas. */
void atualizarGuindastes(Guindastes *guindastes, int horario);

/* Altera o número de guindastes ativos de um grupo de guindastes,
atualizando ao mesmo tempo o estado de cada guindaste. Tem preferência
por desativar guindastes com menor progresso, e reativas guindastes
com maior progresso, economizando energia a longo prazo. */
void alterarGuindastesAtivos(Guindastes *guindastes, int ativos);

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
};

/* Remove o grupo de guindastes da memória. */
void removerGuindastes(Guindastes *guindastes)
{
    free(guindastes->progressos);
    free(guindastes->estados);
    free(guindastes);
}
