import datetime
import numpy as np
import matplotlib.colors as colors
import matplotlib.finance as finance
import matplotlib.dates as mdates
import matplotlib.ticker as mticker
import matplotlib.mlab as mlab
import matplotlib.pyplot as pyplot
import matplotlib.font_manager as font_manager
import manipulaHistorico as mhist
import indices

#r = mhist.recuperarDados(ticker,enddate=datetime.date(2016, 5, 2))

def plotIndex(indexs, data):
    for index in indexs:
        pass

def plotBollingerBands(r,w,d,mostrar_pontos_decisao=False):
    pyplot.rc('axes', grid=True)
    pyplot.rc('grid', color='0.75', linestyle='-', linewidth=0.5)

    # left, bottom, width, height = .05, .1, .9, .8
    # rectP = [left, bottom, width, height]

    fillcolor = 'darkgoldenrod'

    left, width = 0.05, 0.9
    rectR = [left, 0.6, width, 0.3]
    rectP = [left, 0.1, width, 0.5]

    fig = pyplot.figure(facecolor='lightgray')#,figsize=(18,30))
    axescolor = '#f6f6f6'  # the axes background color

    axP = fig.add_axes(rectP, axisbg=axescolor)
    axV = axP.twinx()
    axR = fig.add_axes(rectR, axisbg=axescolor, sharex=axP)

    axR.set_ylim(-101, 500)
    # axR.set_yticks([-1,0,1])

    # plot the price and volume data
    prices = r.adj_close

    dx = r.adj_close - r.close
    low = r.low + dx
    high = r.high + dx

    deltas = np.zeros_like(prices)
    deltas[1:] = np.diff(prices)

    up = deltas > 0

    axP.vlines(r.date[up], low[up], high[up], color='black', label='_nolegend_',lw=1.1)
    axP.vlines(r.date[~up], low[~up], high[~up], color='black', label='_nolegend_',lw=1.1)

    media_movel,upper_band,lower_band,ponto_decisao,retornos = indices.bollinger_bands(prices,w,d) #calculando

    axP.plot(r.date, media_movel, color='blue', lw=2, label='MBB')
    axP.plot(r.date, upper_band, color='green', lw=1.1, label='UBB')
    axP.plot(r.date, lower_band, color='red', lw=1.1, label='LBB')


    axR.plot(r.date, (retornos - 1)*100, color='black', lw=1, label='')

    axR.axhline(0, color=fillcolor)

    print(np.asarray([x for x in ((retornos - 1)*100) if x != 0]))

    if mostrar_pontos_decisao:
        vendas = [i for i,p in enumerate(ponto_decisao) if p > 0]

        dias_venda = []
        precos_venda = []

        for i in vendas:
            dias_venda.append(r.date[i])
            precos_venda.append(upper_band[i] + 1)

        axP.plot(dias_venda, precos_venda, "gv")

        compras = [i for i,p in enumerate(ponto_decisao) if p < 0]

        dias_compras = []
        precos_compras = []

        for i in compras:
            dias_compras.append(r.date[i])
            precos_compras.append(lower_band[i] - 1)

        axP.plot(dias_compras, precos_compras, "r^")


    lastHist = r[-1]
    text = '%s O:%1.2f | H:%1.2f | L:%1.2f | C:%1.2f | V:%1.1fM | Chg:%+1.2f' % (
                                                                    lastHist.date.strftime('%d-%b-%Y'),
                                                                    lastHist.open, lastHist.high,
                                                                    lastHist.low, lastHist.close,
                                                                    lastHist.volume*1e-6,
                                                                    lastHist.close - lastHist.open)
    textsize = 9
    axP.text(0.3, 0.9, text, transform=axP.transAxes, fontsize=textsize)

    props = font_manager.FontProperties(size=10)
    leg = axP.legend(loc='center left', shadow=True, fancybox=True, prop=props)
    leg.get_frame().set_alpha(0.5)


    volume = (r.close*r.volume)/1e6  # dollar volume in millions
    vmax = volume.max()

    axV.fill_between(r.date, volume, 0, label='Volume', facecolor=fillcolor, edgecolor=fillcolor)
    axV.set_ylim(0, 5*vmax)
    axV.set_yticks([])

    # turn off upper axis tick labels, rotate the lower ones, etc
    for ax in axP, axV,axR:
        if ax != axP:
            for label in ax.get_xticklabels():
                label.set_visible(False)
        else:
            for label in ax.get_xticklabels():
                label.set_rotation(30)
                label.set_horizontalalignment('right')

        ax.fmt_xdata = mdates.DateFormatter('%Y-%m-%d')

    axP.yaxis.set_major_locator(mticker.MaxNLocator(5, prune='both'))

    pyplot.show()

if __name__ == "__main__":
    # ticker,w,d = 'YHOO',20,2
    # ticker,w,d = 'YHOO',69,.1
    # ticker,w,d = 'SPY',90,.0
    # ticker,w,d = 'YHOO',3,1.4
    ticker,w,d = 'SPY',27,2.1

    plotBollingerBands(ticker,w,d,mostrar_pontos_decisao=True)
