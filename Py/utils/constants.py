'''
@author: leo
'''
import datetime

startdate = datetime.date(2007, 1, 1)
enddate = datetime.date(2017, 1, 1)

tickers = ["YHOO", "SPY", "AAPL", "MSFT", "XOM", "JNJ", "AMZN", "INTC", "GE", "T"]

dadosPathMask = 'dadosHistoricos/dados_2007_2017/tabela_%s'
dadosBinPathMask = 'dadosHistoricos/dados_2007_2017/tabela_%s_bollinger.bin'

if __name__ == "__main__":
    pass
