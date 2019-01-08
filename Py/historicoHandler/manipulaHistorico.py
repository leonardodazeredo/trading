'''
@author: leo
'''
import os,csv,sys
from array import array
import datetime
import numpy as np
import matplotlib.finance as finance
import matplotlib.mlab as mlab

from utils.constants import *

def recuperarDados(ticker, startdate=None, enddate=None):
    if startdate is None and enddate is None:
        records = np.load(dadosPathMask % (ticker) + ".npy")
    else:
        if startdate is None:
            startdate=datetime.date(2007, 1, 1)
        if enddate is None:
            enddate=datetime.date.today()

        with finance.fetch_historical_yahoo(ticker, startdate, enddate) as fh:
            records = mlab.csv2rec(fh)
        records.sort()

        np.save(dadosPathMask % (ticker),records)

    return records

def criarClosePricesBin(ticker,dadosBinPath,records=None):
    if records is None:
        records = recuperarDados(ticker)

    closePrices = []
    for record in records:
        closePrices.append(record[6])

    with open(dadosBinPath, 'wb') as dadosHistoricosBin:
        size_array = array('i', [len(records)])
        size_array.tofile(dadosHistoricosBin)

        float_array = array('f', closePrices)
        float_array.tofile(dadosHistoricosBin)

if __name__ == "__main__":
    if sys.argv[1] == 'recuperarDados' and sys.argv[2] == 'criarClosePricesBin':
        for ticker in tickers:
            records = recuperarDados(ticker,startdate=startdate,enddate=enddate)

            dadosBinPath = dadosBinPathMask % (ticker)
            criarClosePricesBin(ticker=ticker,dadosBinPath=dadosBinPath,records=records)

    elif sys.argv[1] == 'criarClosePricesBin':
        for ticker in tickers:
            dadosBinPath = dadosBinPathMask % (ticker)
            criarClosePricesBin(ticker=ticker,dadosBinPath=dadosBinPath,records=None)
