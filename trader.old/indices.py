import datetime
import numpy as np
import matplotlib.finance as finance
import matplotlib.dates as mdates
import matplotlib.mlab as mlab
import manipulaHistorico as mhist

def decisao_bollinger_bands(historico, upper_band, lower_band, dia_de_referencia):
    # Estrategia de decisao
    if historico[dia_de_referencia] >= upper_band[dia_de_referencia]: #VENDE
        return -1
    elif historico[dia_de_referencia] <= lower_band[dia_de_referencia]: #COMPRA
        return 1
    else:
        return 0

# Idices de analise tecnica
def bollinger_bands(historico, w, d):
    """
    Calcula as bandas para dos dados historicos com janela de tamanho inteiro w e fator de multiplicacao d para o desvio padrao
    """

    historico = np.asarray(historico)
    media_movel = np.zeros(len(historico))
    upper_band = np.zeros(len(historico))
    lower_band = np.zeros(len(historico))

    pontos_decisao = np.zeros(len(historico))

    dia_corrente = w
    dia_anterior = w - 1

    while dia_corrente<(len(historico)):
        media_movel[dia_anterior] = np.mean(historico[dia_corrente - w: dia_corrente]);
        desvio = np.std(historico[dia_corrente - w: dia_corrente]);

        upper_band[dia_anterior] = media_movel[dia_anterior] + (d)*desvio;
        lower_band[dia_anterior] = media_movel[dia_anterior] - (d)*desvio;

        # Estrategia de decisao
        pontos_decisao[dia_corrente] = decisao_bollinger_bands(historico, upper_band, lower_band, dia_anterior)

        dia_anterior+=1
        dia_corrente+=1

    media_movel[:w] = media_movel[w]
    upper_band[:w] = upper_band[w]
    lower_band[:w] = lower_band[w]

    return pontos_decisao


# Indices de risco

def sharpe_ratio():
    pass
