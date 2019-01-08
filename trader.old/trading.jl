using PyCall
@pyimport manipulaHistorico as mhist
@pyimport constants

include("indices.jl")

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

frange(ini,fim,step) = collect(ini:step:fim)

function executar(historico,w,d)
    # historico = historico[:adj_close]

    capital = zeros(length(historico))
    # retorno = ones(length(historico))
    # retorno_acumulado = ones(length(historico))

    dia_corrente = 2
    dia_anterior = 1

    capital[dia_corrente] = capital[dia_anterior] = CAPITAL_INICIAL

    num_acoes = ACOES_INICIAL

    pontos_decisao = bollinger_bands(historico,w,d)
    # i= 1
    # for x in pontos_decisao
    #   println(i," ",x)
    #   i+=1
    # end

    # return

    while dia_corrente<=(length(historico))
        capital[dia_corrente] = capital[dia_anterior]

        num_acoes_compra = trunc(Int,capital[dia_corrente] / historico[dia_corrente])

        if pontos_decisao[dia_corrente] == -1 && num_acoes > 0
            capital[dia_corrente]+= (historico[dia_corrente] * num_acoes)
            # print("vendi",num_acoes)

            num_acoes = 0

        elseif pontos_decisao[dia_corrente] == 1 && num_acoes_compra > 0
            num_acoes+= num_acoes_compra

            capital[dia_corrente]-= (historico[dia_corrente] * num_acoes_compra)
            # print("comprei",num_acoes_compra)
        end

        # retorno[dia_corrente] = capital[dia_corrente] / capital[dia_anterior]
        # retorno_acumulado[dia_corrente] = prod(retorno)

        dia_anterior+=1
        dia_corrente+=1
    end

    # return capital[end]
    return (capital[end],w,d)
end

# println(executar(mhist.recuperarDados("YHOO"),3,1.4))
# executar(mhist.recuperarDados("YHOO"),3,1.4)

number_of_traders = trunc(Int,((TAMANHO_MAXIMO - TAMANHO_INICIAL)/TAMANHO_INCREMENTO + 1) *
                    ((DESVIOS_MAXIMO - DESVIOS_INICIAL)/DESVIOS_INCREMENTO + 1))

parametros = []

for w in frange(TAMANHO_INICIAL,TAMANHO_MAXIMO + TAMANHO_INCREMENTO,TAMANHO_INCREMENTO)
    for d in frange(DESVIOS_INICIAL,DESVIOS_MAXIMO + DESVIOS_INCREMENTO,DESVIOS_INCREMENTO)
      push!(parametros,[trunc(Int,w),d])
    end
end

resultado = [(0.0,0,0.0) for x in 1:number_of_traders]

function testar()
  for ticker in constants.tickers
      historico = mhist.recuperarDados(ticker)
      historico = historico[:adj_close]

      # println("Numero de traders:",number_of_traders)

      Threads.@threads for i = 1:number_of_traders
        resultado[i] = executar(historico,parametros[i][1],parametros[i][2])
      end

      # sort!(resultado,by=x->x[1])
      # println(ticker, " ", resultado)
  end
end

# function testar()
#   for ticker in constants.tickers
#       resultado = []
#       historico = mhist.recuperarDados(ticker)
#
#       number_of_traders = ((TAMANHO_MAXIMO - TAMANHO_INICIAL)/TAMANHO_INCREMENTO + 1) *
#                           ((DESVIOS_MAXIMO - DESVIOS_INICIAL)/DESVIOS_INCREMENTO + 1)
#
#       println("Numero de traders:",number_of_traders)
#
#       futures = []
#
#       secao = number_of_traders/4
#
#       n = 1
#       for w in frange(TAMANHO_INICIAL,TAMANHO_MAXIMO + TAMANHO_INCREMENTO,TAMANHO_INCREMENTO)
#           for d in frange(DESVIOS_INICIAL,DESVIOS_MAXIMO + DESVIOS_INCREMENTO,DESVIOS_INCREMENTO)
#               n+=1
#               push!(futures,remotecall(executar, trunc(Int,n/secao) + 1, historico,w,d))
#           end
#       end
#
#       for f in futures
#         wait(f)
#         push!(resultado, fetch(f))
#       end
#
#       sort!(resultado,by=x->x[1])
#       println(ticker, " ", resultado[end])
#   end
# end
#
# function testar()
#   for ticker in constants.tickers
#       resultado = []
#       historico = mhist.recuperarDados(ticker)
#
#       number_of_traders = ((TAMANHO_MAXIMO - TAMANHO_INICIAL)/TAMANHO_INCREMENTO + 1) *
#                           ((DESVIOS_MAXIMO - DESVIOS_INICIAL)/DESVIOS_INCREMENTO + 1)
#
#       println("Numero de traders:",number_of_traders)
#
#       for w in frange(TAMANHO_INICIAL,TAMANHO_MAXIMO + TAMANHO_INCREMENTO,TAMANHO_INCREMENTO)
#           for d in frange(DESVIOS_INICIAL,DESVIOS_MAXIMO + DESVIOS_INCREMENTO,DESVIOS_INCREMENTO)
#               push!(resultado,executar(historico,w,d))
#           end
#       end
#
#       sort!(resultado,by=x->x[1])
#       println(ticker, " ", resultado[end])
#   end
# end

testar()
