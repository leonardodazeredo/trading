function decisao_bollinger_bands(historico, upper_band, lower_band, dia_de_referencia)
    # Estrategia de decisao
    if historico[dia_de_referencia] >= upper_band[dia_de_referencia] #VENDE
        return -1
    elseif historico[dia_de_referencia] <= lower_band[dia_de_referencia] #COMPRA
        return 1
    else
        return 0
    end
end

# Idices de analise tecnica
function bollinger_bands(historico, w, d)
    """
    Calcula as bandas para dos dados historicos com janela de tamanho inteiro w e fator de multiplicacao d para o desvio padrao
    """

    w = trunc(Int,w)

    media_movel = zeros(length(historico))
    upper_band = zeros(length(historico))
    lower_band = zeros(length(historico))

    pontos_decisao = zeros(length(historico))
    dia_corrente = w + 1
    dia_anterior = dia_corrente - 1

    while dia_corrente<=(length(historico))
        media_movel[dia_anterior] = mean(historico[dia_corrente - w: dia_anterior]);
        desvio = stdm(historico[dia_corrente - w: dia_anterior],media_movel[dia_anterior],corrected=false);

        upper_band[dia_anterior] = media_movel[dia_anterior] + (d)*desvio;
        lower_band[dia_anterior] = media_movel[dia_anterior] - (d)*desvio;

        # Estrategia de decisao
        pontos_decisao[dia_corrente] = decisao_bollinger_bands(historico, upper_band, lower_band, dia_anterior)

        dia_anterior+=1
        dia_corrente+=1
    end

    return pontos_decisao
end
