// TODO: Descrição do módulo de energia.

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#include "energia.h"

int main(int argc, char **argv)
{
    int hora, minuto;

    // Cria uma plataforma padrão, com 25 séries de bombas e 10
    // guindastes.
    Bombas *bombas = CriarBombas(NUM_BOMBAS);
    Guindastes *guindastes = CriarGuindastes(NUM_GUINDASTES);

    /* --------------------------------------------------------------
    ----------------- ARGUMENTOS ------------------------------------
    -------------------------------------------------------------- */
    // Se o programa for aberto com nenhum argumento, entra no modo
    // interativo com o horário padrão (12:00).
    if (argc == 1)
    {
        hora = 12;
        minuto = 0;
    }
    // Se o programa for aberto com 1 argumento:
    else if (argc == 2 )
    {
        // Modo simulação: calcula o gasto de energia diário e mensal
        // (30 dias), em situações ideais.
        if (!strcmp(argv[1], "custo"))
        {
            hora = 0, minuto = 0;
            // Cria um navio com capacidade extrema, simulando uma
            // situação em que a troca de navios é instantânea.
            atualizarNavio(guindastes, INT_MAX);
            // Dá 24 * 60 passos (1440 minutos = 1 dia), registrando
            // o custo durante o processo.
            double custoDiario = passosN(24 * 60, bombas, guindastes,
                                         &hora, &minuto);
            // Mostra os custos calculados no terminal.
            printf("Condições ideais (operação contínua):\n");
            printf("Custo diário: R$ %.3lf\n", custoDiario);
            printf("Custo mensal: R$ %.3lf\n", custoDiario * 30);
            return 0;
        }
        // Ajuda: mostra os comandos possíveis no terminal.
        else if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))
        {
            // TODO: Ajuda.
            return 0;
        }
        // Comando não identificado: instruções básicas.
        else
        {
            printf("Use --help para obter ajuda.\n", argv[0]);
            return 1;
        }
    }
    // Se o programa for aberto com 3 argumentos:
    else if (argc == 4)
    {
        if (strcmp(argv[1], "-t"))
        {
            printf("Use --help para obter ajuda.\n", argv[0]);
            return 1;
        }
        // Comando -t: permite inserir um horário inicial qualquer
        // para o modo interativo.
        // Se alguma das entradas não for um número: instruções de
        // uso do comando -t.
        if (!strNumerica(argv[2]) || !strNumerica(argv[3]))
        {
            printf("Uso: %s -t horas minutos\n", argv[0]);
            return 1;
        }
        // Se ambas as entradas são números: os insere como horário
        // inicial, limitando os valores de hora e minuto para 0-23
        // e 0-59, respectivamente.
        char *p; // Variável não usada, necessária para strtol.
        minuto = (int)(strtol(argv[3], &p, 10) % 60);
        hora = (int)((strtol(argv[3], &p, 10) / 60
               + strtol(argv[2], &p, 10)) % 24);
    }
    // Outras quantidades de argumentos: instruções básicas.
    else
    {
        printf("Use %s --help para obter ajuda.\n", argv[0]);
        return 1;
    }

    /* --------------------------------------------------------------
    -------------- MODO INTERATIVO ----------------------------------
    -------------------------------------------------------------- */

    // Atraca um navio com capacidade padrão.
    atualizarNavio(guindastes, CAPACIDADE_DO_NAVIO);

    printf("--- MODO INTERATIVO ---\n");
    printf("Digite 'h' para obter ajuda.\n\n");
    while (true)
    {
        // Mostra informações resumidas.
        printf("Horário: %02d:%02d\n", hora, minuto);
        printf("Bombas ativas: %d\n", bombas->ativas);
        printf("Guindastes ativos: %d\n", guindastes->ativos);
        printf("Capacidade do navio: %d\n", guindastes->estadoDoNavio);
        // Solicita e executa um comando.
        while (true)
        {
            // Solicita um char do usuário.
            printf("-> ");
            // Gambiarra para evitar um bug causado pelo buffering
            // do stdin, não sei direito como funciona :/
            char comando;
            scanf(" %c", &comando);
            // Cria uma variável para o custo e para um int qualquer.
            double custo;
            int n;
            switch (comando)
            {
                // Comando 'P': avança a simulação em algum número de
                // passos (até um dia).
                case 'P':
                    n = getNum(0, 24*60);
                    custo = passosN(n, bombas, guindastes,
                                    &hora, &minuto);
                    printf("Custo: R$ %.3lf\n", custo);
                    break;
                // Comando 'p': avança a simulação um passo.
                case 'p':
                    custo = passosN(1, bombas, guindastes,
                                    &hora, &minuto);
                    printf("Custo: R$ %.3lf\n", custo);
                    break;
                // Comando 'E': desativa o modo de emergência das
                // bombas.
                case 'E':
                    normalizacaoDoBombeamento(bombas);
                    break;
                // Comando 'e': ativa o modo de emergência das bombas.
                case 'e':
                    emergenciaDoBombeamento(bombas);
                    break;
                // Comando 'G': altera o número máximo de guindastes
                // ativos.
                case 'G':
                    n = getNum(0, guindastes->totais);
                    guindastes->ativosMax = n;
                    break;
                // Comando 'g': mostra o estado dos guindastes.
                case 'g':
                    estadoDosGuindastes(guindastes);
                    continue;
                // Comando 'B': altera o número de bombas ativas.
                case 'B':
                    n = getNum(0, bombas->totais);
                    alterarBombasAtivas(bombas, n);
                    break;
                // Comando 'b': mostra o estado das bombas.
                case 'b':
                    estadoDoBombeamento(bombas);
                    continue;
                // Comando 'N': avança a simulação até o navio
                // atual estar cheio.
                case 'N':
                    custo = passosNavio(bombas, guindastes,
                                        &hora, &minuto);
                    printf("Custo: R$ %.3lf\n", custo);
                    break;
                // Comando 'n': significa a chegada de um navio.
                case 'n':
                    // Se não há um navio já atracado, continua.
                    if(atualizarNavio(guindastes, CAPACIDADE_DO_NAVIO))
                    {
                        printf("Navio atracado.\n");
                        break;
                    }
                    // Se há, pede um comando novo.
                    else
                    {
                        printf("Já há um navio atracado.\n");
                        break;
                    }
                // Comando 'H/h': mostra ajuda do modo interativo.
                case 'H':
                case 'h':
                    // TODO: Ajuda do modo interativo.
                    continue;
                // Comando 'Q/q': sai do programa.
                case 'Q':
                case 'q':
                    return 0;
                // Se o comando não é reconhecido, pede um novo.
                default:
                    printf("Comando inválido.\n");
                    continue;
            }
            break;
        }
        putchar('\n');
    }

    return 0;
}

/* Dá uma quantidade pré-determinada de passos, e retorna o custo
total dessas etapas. */
double passosN(int passos, Bombas *bombas, Guindastes *guindastes,
               int *hora, int *minuto)
{
    double fracaoDaTermeletrica;
    double custo = 0;
    for (int i = 0; i < passos; i++)
    {
        passo(bombas, guindastes, hora, minuto, &fracaoDaTermeletrica);
        custo += fracaoDaTermeletrica * P_TERMELETRICA *
                 C_TERMELETRICA / 60;
    }
    return custo;
}

/* Funciona como passosN, mas em vez de dar uma quantidade pré-
-determinada de passos, avança a simulação até o navio atracado
na plataforma atingir sua capacidade. */
double passosNavio(Bombas *bombas, Guindastes *guindastes,
                   int *hora, int *minuto)
{
    if (guindastes->estadoDoNavio <= 0)
    {
        return 0.0;
    }
    double fracaoDaTermeletrica;
    double custo = 0;
    while (passo(bombas, guindastes, hora, minuto,
           &fracaoDaTermeletrica))
    {
        custo += fracaoDaTermeletrica * P_TERMELETRICA *
                 C_TERMELETRICA / 60;
    }
    return custo;
}

/* Simula um passo (um minuto) de operação da plataforma. Retorna
true se há um navio na plataforma, false se não. A fração da
capacidade da termelétrica que é demandada pela plataforma é
colocada no endereço de memória especificado. */
bool passo(Bombas *bombas, Guindastes *guindastes,
           int *hora, int *minuto, double *fracaoDaTermeletrica)
{
    // Acresce o tempo em um minuto.
    (*minuto)++;
    if (*minuto >= 60)
    {
        *minuto = 0;
        (*hora)++;
        *hora %= 24;
    }
    // Avança a posição dos guindastes.
    bool estadoDoNavio = atualizarGuindastes(guindastes, *hora);
    // Calcula a distribuição de energia.
    *fracaoDaTermeletrica = ajustarDemanda(bombas, guindastes, *hora);
    // Retorna o estado do navio.
    return estadoDoNavio;
}

/* Calcula a porcentagem da demanda da termelétrica que deve ser
direcionada para a plataforma de petróleo para a sua operação e, se
necessário, ajusta a quantidade de guindastes que podem ser
ativados. Retorna a fração da potência fornecida pela termelétrica
que deve ser direcionada à plataforma. */
double ajustarDemanda(Bombas *bombas, Guindastes *guindastes, int horario)
{
    // Primeiro, calcula a demanda total de energia no momento.
    double demandaTotal = P_AUXILIAR;
    demandaTotal += bombas->ativas * P_BOMBA;
    demandaTotal += guindastes->ativos * P_GUINDASTE;
    // Então, calcula quanta dessa energia deve vir da termelétrica.
    double subdemanda = demandaDaTermeletrica(demandaTotal, horario);
    // Se a demanda da termelétrica é menor que a sua capacidade de
    // fornecimente, desliga primeiro os guindastes e depois as
    // bombas, até que a energia demandada possa ser fornecida pela
    // usina.
    while (subdemanda > P_TERMELETRICA && guindastes->ativos > 0)
    {
        guindastes->ativos--;
        subdemanda -= P_GUINDASTE / E_INVERSORES;
    }
    while (subdemanda > P_TERMELETRICA && bombas->ativas > 0)
    {
        alterarBombasAtivas(bombas, bombas->ativas - 1);
        subdemanda -= P_BOMBA / E_INVERSORES;
    }
    // Se a demanda da termelétrica ainda e maior que ela é capaz de
    // fornecer, solicita toda a potência da usina.
    if (subdemanda > P_TERMELETRICA)
    {
        return 1.0;
    }
    // Se não, calcula a fração da capacidade da usina que deve ser
    // direcionada à plataforma.
    return subdemanda / P_TERMELETRICA;
}

/* Calcula a potência que deve ser fornecida pela termelétrica, dado
um horário do dia e uma demanda total, em kW. */
double demandaDaTermeletrica(double demandaTotal, int horario)
{
    double demanda = demandaTotal - potenciaDasTurbinas(horario);
    return demanda / E_INVERSORES;
}

/* Calcula a potência gerada pelas turbinas eólicas, em kW, em um
certo horário do dia. */
double potenciaDasTurbinas(int horario)
{
    if (horario > 7 || horario < 22)
    {
        // 80 kW = potência quando v = 6 m/s.
        return 80 * NUM_TURBINAS * E_INVERSORES;
    }
    // 70 kW = potência quando v = 10 m/s.
    return 70 * NUM_TURBINAS * E_INVERSORES; // TODO: Confirmar esse valor (70).
}

/* Solicita um número do usuário dentro de um limite. */
int getNum(int minimo, int maximo)
{
    int numero;
    printf("Entre um número entre %d e %d:\n", minimo, maximo);
    do
    {
        printf("-> ");
        scanf("%d", &numero);
    }
    while (numero < minimo || numero > maximo);
    return numero;
}

/* Retorna true se str for uma string numérica, false se não. */
bool strNumerica(char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (!isdigit(str[i]))
        {
            return false;
        }
    }
    return true;
}
