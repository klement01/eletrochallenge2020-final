/** Emite comandos para os módulos de controle das bombas e dos
 *  guindastes. Faz isso baseado em cálculos de consumo de energia.
 *  Conta com um modo interativo, que permite simular situações reais
 *  de ativação e desativação de componentes, carregamento de navios
 *  e o modo de emergência das bombas.
 *  O modo interativo pode ser acesso abrindo o programa sem nenhum
 *  argumento ou com o argumento opcional -t, que permite escolher um
 *  horário inicial diferente do padrão (12:00).
 *  Além disso, pode ser aberto no modo custo, que calcula o custo
 *  diário e mensal de operação da plataforma, de acordo com as
 *  especificaçes do desafio.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#include "energia.h"

int main(int argc, char **argv)
{
    // Variáveis que dependem dos argumentos.
    int hora = 12, minuto = 0, segundo = 0;
    /* --------------------------------------------------------------
    ----------------- ARGUMENTOS ------------------------------------
    -------------------------------------------------------------- */
    // Se o programa for aberto com nenhum argumento, entra no modo
    // interativo com o horário padrão (12:00).
    if (argc == 1)
    {
        // Modo interativo padrão.
    }
    // Se o programa for aberto com 1 argumento:
    else if (argc == 2)
    {
        // Modo simulação: calcula o gasto de energia diário e mensal
        // (30 dias), em situações ideais.
        if (!strcmp(argv[1], "custo"))
        {
            hora = 0, minuto = 0;
            // Cria uma plataforma padrão, com 25 séries de bombas e
            // 10 guindastes.
            Bombas *bombas = CriarBombas(NUM_BOMBAS);
            Guindastes *guindastes = CriarGuindastes(NUM_GUINDASTES);
            if (bombas == NULL || guindastes == NULL)
            {
                // Remove as bombas e guindastes da memória.
                removerBombeamento(bombas);
                removerGuindastes(guindastes);
                return 2;
            }
            // Cria um navio com capacidade extrema, simulando uma
            // situação em que a troca de navios é instantânea.
            atualizarNavio(guindastes, INT_MAX);
            // Dá 60*24*60*30 passos (1 mês), registrando o custo
            // durante o processo.
            double custoMensal = passosN(60 * 24 * 60 * 30, bombas, guindastes,
                                         &hora, &minuto, &segundo, false);
            // Mostra os custos calculados no terminal.
            printf("Condições ideais (operação contínua):\n");
            printf("Custo diário: R$ %.3lf\n", custoMensal / 30);
            printf("Custo mensal: R$ %.3lf\n", custoMensal);
            // Remove as bombas e guindastes da memória.
            removerBombeamento(bombas);
            removerGuindastes(guindastes);
            return 0;
        }
        // Ajuda: mostra os comandos possíveis no terminal.
        else if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))
        {
            ajudaDoPrograma();
            return 0;
        }
        // Comando não identificado: instruções básicas.
        else
        {
            printf("Use 'plataforma --help' para obter ajuda.\n");
            return 1;
        }
    }
    // Se o programa for aberto com 3 argumentos:
    else if (argc == 4)
    {
        if (strcmp(argv[1], "-t"))
        {
            printf("Use 'plataforma --help' para obter ajuda.\n");
            return 1;
        }
        // Comando -t: permite inserir um horário inicial qualquer
        // para o modo interativo.
        // Se alguma das entradas não for um número: instruções de
        // uso do comando -t.
        if (!strNumerica(argv[2]) || !strNumerica(argv[3]))
        {
            printf("Uso: plataforma -t horas minutos\n");
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
        printf("Use 'plataforma --help' para obter ajuda.\n");
        return 1;
    }

    /* --------------------------------------------------------------
    -------------- MODO INTERATIVO ----------------------------------
    -------------------------------------------------------------- */

    // Cria uma plataforma padrão, com 25 séries de bombas, 10
    // guindastes e um navio com capacidade padrão.
    Bombas *bombas = CriarBombas(NUM_BOMBAS);
    Guindastes *guindastes = CriarGuindastes(NUM_GUINDASTES);
    if (bombas == NULL || guindastes == NULL)
    {
        // Remove as bombas e guindastes da memória.
        removerBombeamento(bombas);
        removerGuindastes(guindastes);
        return 2;
    }
    atualizarNavio(guindastes, CAPACIDADE_DO_NAVIO);
    // mostrarFracao: true se o usuário quer ver o uso da termelétrica
    // a cada passo, false se não.
    bool mostrarFracao = false;

    printf("----- MODO INTERATIVO -----\n");
    printf("Digite 'h' para obter ajuda\n");
    printf("---------------------------\n\n");
    double custoTotal = 0;
    while (true)
    {
        // Mostra informações resumidas.
        printf("Horário: %02d:%02d.%02d\n", hora, minuto, segundo);
        printf("Bombas ativas: %d de %d\n", bombas->ativas, bombas->totais);
        printf("Guindastes ativos: %d de %d\n", guindastes->ativos, guindastes->totais);
        printf("Capacidade do navio: %d barris\n", guindastes->estadoDoNavio);
        // Solicita e executa um comando.
        while (true)
        {
            // Solicita um char do usuário.
            printf("-> ");
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
                    n = getNum(0, 24*60*60);
                    custo = passosN(n, bombas, guindastes, &hora,
                                    &minuto, &segundo, mostrarFracao);
                    printf("\nCusto: R$ %.3lf\n", custo);
                    custoTotal += custo;
                    break;
                // Comando 'p': avança a simulação um passo.
                case 'p':
                    custo = passosN(1, bombas, guindastes, &hora,
                                    &minuto, &segundo, mostrarFracao);
                    printf("\nCusto: R$ %.3lf\n", custo);
                    custoTotal += custo;
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
                    continue;
                // Comando 'g': mostra o estado dos guindastes.
                case 'g':
                    estadoDosGuindastes(guindastes);
                    continue;
                // Comando 'B': altera o número de bombas ativas.
                case 'B':
                    n = getNum(0, bombas->totais);
                    alterarBombasAtivas(bombas, n);
                    continue;
                // Comando 'b': mostra o estado das bombas.
                case 'b':
                    estadoDoBombeamento(bombas);
                    continue;
                // Comando 'N': avança a simulação até o navio
                // atual estar cheio.
                case 'N':
                    custo = passosNavio(bombas, guindastes, &hora,
                                        &minuto, &segundo, mostrarFracao);
                    printf("\nCusto: R$ %.3lf\n", custo);
                    custoTotal += custo;
                    break;
                // Comando 'n': significa a chegada de um navio.
                case 'n':
                    // Se não há um navio já atracado, continua.
                    if(atualizarNavio(guindastes, CAPACIDADE_DO_NAVIO))
                    {
                        printf("Navio atracado.\n");
                        printf("Capacidade do navio: %d barris\n",
                               guindastes->estadoDoNavio);
                        continue;
                    }
                    // Se há, pede um comando novo.
                    else
                    {
                        printf("Já há um navio atracado.\n");
                        continue;
                    }
                // Comando 'T/t': decide se a demanda da termelétrica
                // será mostrada na tela durante o modo interativo.
                case 'T':
                case 't':
                    mostrarFracao = !mostrarFracao;
                    if (mostrarFracao)
                    {
                        printf("Demanda da termelétrica será mostrada.\n");
                    }
                    else
                    {
                        printf("Demanda da termelétrica não será mostrada.\n");
                    }
                    continue;
                // Comando 'H/h': mostra ajuda do modo interativo.
                case 'H':
                case 'h':
                    ajudoDoModoInterativo();
                    continue;
                // Comando 'Q/q': sai do programa.
                case 'Q':
                case 'q':
                    // Remove as bombas e guindastes da memória.
                    removerBombeamento(bombas);
                    removerGuindastes(guindastes);
                    return 0;
                // Se o comando não é reconhecido, pede um novo.
                default:
                    printf("Comando inválido.\n");
                    continue;
            }
            break;
        }
        printf("Custo total: R$ %.3lf\n\n", custoTotal);
    }

    // Remove as bombas e guindastes da memória.
    removerBombeamento(bombas);
    removerGuindastes(guindastes);
    return 0;
}

/* Dá uma quantidade pré-determinada de passos, e retorna o custo
total dessas etapas. */
double passosN(int passos, Bombas *bombas, Guindastes *guindastes,
               int *hora, int *minuto, int *segundo,
               bool mostrarFracao)
{
    double fracaoDaTermeletrica;
    double custo = 0;
    for (int i = 0; i < passos; i++)
    {
        passo(bombas, guindastes, hora, minuto, segundo,
              &fracaoDaTermeletrica,
              mostrarFracao);
        custo += fracaoDaTermeletrica * P_TERMELETRICA *
                 C_TERMELETRICA / 3600;
    }
    return custo;
}

/* Funciona como passosN, mas em vez de dar uma quantidade pré-
-determinada de passos, avança a simulação até o navio atracado
na plataforma atingir sua capacidade. */
double passosNavio(Bombas *bombas, Guindastes *guindastes, int *hora,
                   int *minuto, int *segundo, bool mostrarFracao)
{
    if (guindastes->estadoDoNavio == 0)
    {
        return 0.0;
    }
    double fracaoDaTermeletrica;
    double custo = 0;
    while (passo(bombas, guindastes, hora, minuto, segundo,
           &fracaoDaTermeletrica, mostrarFracao))
    {
        custo += fracaoDaTermeletrica * P_TERMELETRICA *
                 C_TERMELETRICA / 3600;
    }
    return custo;
}

/* Simula um passo (um minuto) de operação da plataforma. Retorna
true se há um navio na plataforma, false se não. A fração da
capacidade da termelétrica que é demandada pela plataforma é
colocada no endereço de memória especificado. */
bool passo(Bombas *bombas, Guindastes *guindastes, int *hora,
           int *minuto, int *segundo, double *fracaoDaTermeletrica,
           bool mostrarFracao)
{
    // Acresce o tempo em um segundo.
    (*segundo)++;
    if (*segundo >= 60)
    {
        *segundo = 0;
        (*minuto)++;
    }
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
    // Mostra a fração da energia usada em um determinado horário.
    if (mostrarFracao)
    {
        printf("\n(%02d:%02d.%02d) %.2lf %%", *hora, *minuto, *segundo,
               *fracaoDaTermeletrica * 100);
    }
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
    /** Em um sistema real, uma interface programa -> dispositivo
    seria utilizada para ajustar a demanda de energia de acordo com
    esse valor. */
    return subdemanda / P_TERMELETRICA;
}

/* Calcula a potência que deve ser fornecida pela termelétrica, dado
um horário do dia e uma demanda total, em kW. */
double demandaDaTermeletrica(double demandaTotal, int horario)
{
    double demanda = demandaTotal - potenciaDasTurbinas(horario);
    // Se a potência das turbinas é suficiente para suprir a demanda.
    if (demanda < 0)
    {
        return 0;
    }
    return demanda / E_INVERSORES;
}

/* Calcula a potência gerada pelas turbinas eólicas, em kW, em um
certo horário do dia. */
double potenciaDasTurbinas(int horario)
{
    if (horario > 7 && horario < 22)
    {
        // 80 kW = potência quando v = 6 m/s.
        return 80 * NUM_TURBINAS * E_INVERSORES;
    }
    // 70 kW = potência quando v = 10 m/s.
    return 70 * NUM_TURBINAS * E_INVERSORES;
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

/* Mostra as instruções de uso do programa e do modo interativo,
respectivamente. */
void ajudaDoPrograma(void)
{
    printf("Uso:\n");
    // Modo de uso: interativo.
    printf("\tplataforma [opções]\n");
    printf("\tAbre o programa no modo interativo.\n\n");
    // Modo de uso: custo.
    printf("\tplataforma custo\n");
    printf("\tAbre o programa no modo custo.\n\n");
    // Opções.
    printf("\tOpções:\n");
    printf("\t\t-h --help\n\t\t\tExibe este menu de ajuda\n");
    printf("\t\t-t [hora] [minuto]\n\t\t\tAltera o horário inicial ");
    printf("do modo interativo.\n");
}
void ajudoDoModoInterativo(void)
{
    printf("Os comandos que AVANÇAM a simulação mostram o custo do avanço");
    printf(" atual, o custo total desde o início do programa, e um resumo");
    printf(" do estado da simulação.\n");
    printf("COMANDOS:\n");
    printf("b : exibe o estado de todo o sistema de bombeamento.\n");
    printf("B : permite alterar o número de bombas ativas.\n");
    printf("e : ativa o estado de emergência das bombas.\n");
    printf("E : desativa o esta do emergência das bombas.\n");
    printf("g : exibe o estado de todo o sistema de guindastes.\n");
    printf("G : permite alterar o número máximo de guindastes ativos.\n");
    printf("h : exibe este menu de ajuda.\n");
    printf("H : exibe este menu de ajuda.\n");
    printf("n : tenta atracar um navio.\n");
    printf("N : AVANÇA a simulação até o navio atracado estiver cheio.\n");
    printf("p : AVANÇA a simulação um passo.\n");
    printf("P : AVANÇA a simulação um número arbitrário de passos.\n");
    printf("q : fecha o programa.\n");
    printf("Q : fecha o programa.\n");
    printf("t : decide se a demanda da termelétrica em cada horário será mostrada.\n");
    printf("T : decide se a demanda da termelétrica em cada horário será mostrada.\n");
}
