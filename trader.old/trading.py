import datetime
import numpy as np
import matplotlib.finance as finance
import matplotlib.dates as mdates
import matplotlib.mlab as mlab
from multiprocessing import Pool

from constants import tickers
from indices import bollinger_bands
from util import frange

import manipulaHistorico as mhist


TAMANHO_PADRAO = 10
DESVIOS_PADRAO = 0.5

TAMANHO_MAXIMO = 90
DESVIOS_MAXIMO = 4

TAMANHO_INCREMENTO = 1
DESVIOS_INCREMENTO = 0.1

TAMANHO_INICIAL = 2
DESVIOS_INICIAL = 0.0

CAPITAL_INICIAL = 100000.0

ACOES_INICIAL = 0

class Trader(object):
    '''
    '''
    def __init__(self, historico,indice,w=None,d=None):

        self.historico = historico
        self.capital = CAPITAL_INICIAL
        self.num_acoes = ACOES_INICIAL

        self.indice = indice
        self.w = w
        self.d = d

    def executar(self):
        historico = self.historico.adj_close
        # print(type(historico))

        capital = np.zeros(len(historico))
        retorno = np.ones(len(historico))
        retorno_acumulado = np.ones(len(historico))

        dia_corrente = 1
        dia_anterior = 0

        capital[dia_corrente] = capital[dia_anterior] = self.capital

        num_acoes = self.num_acoes

        if self.indice == bollinger_bands:
            pontos_decisao = self.indice(historico,self.w,self.d)

        i = 1
        for x in pontos_decisao:
            print(i,x)
            i+=1

        while dia_corrente<(len(historico)):

            capital[dia_corrente] = capital[dia_anterior]

            num_acoes_compra = int(capital[dia_corrente] / historico[dia_corrente])

            if pontos_decisao[dia_corrente] == -1 and num_acoes > 0:

                capital[dia_corrente]+= (historico[dia_corrente] * num_acoes) # print("vendi",num_acoes)

                num_acoes = 0

            elif pontos_decisao[dia_corrente] == 1 and num_acoes_compra > 0:

                num_acoes+= num_acoes_compra

                capital[dia_corrente]-= (historico[dia_corrente] * num_acoes_compra) # print("comprei",num_acoes_compra)

            retorno[dia_corrente] = capital[dia_corrente] / capital[dia_anterior]
            retorno_acumulado[dia_corrente] = np.prod(retorno)

            dia_anterior+=1
            dia_corrente+=1

        self.capital = capital[-1]
        self.num_acoes = num_acoes

        return capital[len(capital)-50:-1]

        # print("\nCapital:",capital,"\nRetorno:",retorno,"\nRetorno acumulado (lucrou):", retorno_acumulado[-1], \
        #         "\nPercentual de lucro final", 100*(retorno_acumulado[-1]-1),"%")

def exc(arg):
    trader = Trader(arg[0],bollinger_bands,w=arg[1],d=arg[2])
    trader.executar()

    return (trader.capital, arg[1],arg[2])
    # return trader.capital

if __name__ == '__main__':

    t = Trader(mhist.recuperarDados("YHOO"),bollinger_bands,w=3,d=1.4)
    c = t.executar()
    # print(c)

    # pool = Pool()
    #
    # for ticker in tickers:
    #     historico = mhist.recuperarDados(ticker)
    #
    #     number_of_traders = ((TAMANHO_MAXIMO - TAMANHO_INICIAL)/TAMANHO_INCREMENTO + 1) *\
    #                           ((DESVIOS_MAXIMO - DESVIOS_INICIAL)/DESVIOS_INCREMENTO + 1)
    #
    #     print("Numero de traders:",number_of_traders)
    #
    #     parametros = []
    #
    #     for w in range(TAMANHO_INICIAL,TAMANHO_MAXIMO + TAMANHO_INCREMENTO,TAMANHO_INCREMENTO):
    #         for d in frange(DESVIOS_INICIAL,DESVIOS_MAXIMO + DESVIOS_INCREMENTO,DESVIOS_INCREMENTO):
    #             parametros.append([historico,w,d])
    #             # print(w,d)
    #
    #
    #     result = pool.map(exc,parametros)
    #
    #     # trader = Trader(historico,bollinger_bands,w=2,d=4)
    #     # trader.executar()
    #     # result = [(trader.capital,2,4)]
    #
    #     # print(result)
    #     # result = np.sort(result)
    #     # maxv = max(result)
    #     # print(result.index(maxv),maxv,ticker)
    #
    #     # for value in result:
    #     #     if value[1]==2:
    #     #         print("\n\n:",value,"\n\n")
    #     #         # break
    #
    #     print(sorted(result,key=lambda x: x[0])[-1], ticker)
    #
    #     # for i,value in enumerate(sorted(result,key=lambda x: x[0])[::-1]):
    #     #     print(value, ticker)
    #     #     if i == 40:
    #     #         break
