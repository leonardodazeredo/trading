import datetime
import numpy as np
import matplotlib.colors as colors
import matplotlib.finance as finance
import matplotlib.dates as mdates
import matplotlib.ticker as mticker
import matplotlib.mlab as mlab
import matplotlib.pyplot as pyplot
import matplotlib.font_manager as font_manager


startdate = datetime.date(2006, 1, 1)
today = enddate = datetime.date.today()

ticker = 'YHOO'

fh = finance.fetch_historical_yahoo(ticker, startdate, enddate)
# a numpy record array with fields: date, open, high, low, close, volume, adj_close)

r = mlab.csv2rec(fh)
fh.close()

r.sort()

print(r)

def moving_average(x, n, type='simple'):
    """
    compute an n period moving average.

    type is 'simple' | 'exponential'

    """
    x = np.asarray(x)
    if type == 'simple':
        weights = np.ones(n)
    else:
        weights = np.exp(np.linspace(-1., 0., n))

    weights /= weights.sum()

    a = np.convolve(x, weights, mode='full')[:len(x)]
    a[:n] = a[n]
    # print(len(a))
    return a

pyplot.rc('axes', grid=True)
pyplot.rc('grid', color='0.75', linestyle='-', linewidth=0.5)

textsize = 9
left, width = 0.1, 0.8

rect = [left, 0.3, width, 0.4]


fillcolor = 'darkgoldenrod'

fig = pyplot.figure(facecolor='white')
axescolor = '#f6f6f6'  # the axes background color

# ax1 = fig.add_axes(rect1, axisbg=axescolor)  # left, bottom, width, height
axe = fig.add_axes(rect, axisbg=axescolor)
axet = axe.twinx()
# ax3 = fig.add_axes(rect3, axisbg=axescolor, sharex=ax1)

prices = r.adj_close

# plot the price and volume data
dx = r.adj_close - r.close
low = r.low + dx
high = r.high + dx

deltas = np.zeros_like(prices)
deltas[1:] = np.diff(prices)

up = deltas > 0

axe.vlines(r.date[up], low[up], high[up], color='black', label='_nolegend_')
axe.vlines(r.date[~up], low[~up], high[~up], color='black', label='_nolegend_')

ma20 = moving_average(prices, 20, type='simple')

axe.plot(r.date, ma20, color='blue', lw=2, label='MA (20)')

lastHist = r[-1]
text = '%s O:%1.2f | H:%1.2f | L:%1.2f | C:%1.2f | V:%1.1fM | Chg:%+1.2f' % (
                                                                today.strftime('%d-%b-%Y'),
                                                                lastHist.open, lastHist.high,
                                                                lastHist.low, lastHist.close,
                                                                lastHist.volume*1e-6,
                                                                lastHist.close - lastHist.open)

axe.text(0.3, 0.9, text, transform=axe.transAxes, fontsize=textsize)

props = font_manager.FontProperties(size=10)
leg = axe.legend(loc='center left', shadow=True, fancybox=True, prop=props)
leg.get_frame().set_alpha(0.5)


volume = (r.close*r.volume)/1e6  # dollar volume in millions
vmax = volume.max()

axet.fill_between(r.date, volume, 0, label='Volume', facecolor=fillcolor, edgecolor=fillcolor)
axet.set_ylim(0, 5*vmax)
axet.set_yticks([])

# turn off upper axis tick labels, rotate the lower ones, etc
for ax in axe, axet:
    for label in ax.get_xticklabels():
        label.set_rotation(30)
        label.set_horizontalalignment('right')

    ax.fmt_xdata = mdates.DateFormatter('%Y-%m-%d')

# axe.yaxis.set_major_locator(mticker.MaxNLocator(5, prune='both'))

pyplot.show()