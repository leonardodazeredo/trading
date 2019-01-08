# Makefile
# general
PROJECT_PATH = /home/leo/git/trading

# programs
CXX     = g++
PY      = python3
JL      = julia

# C++ compiler
CXXFLAGS = -std=c++11 -O0 -g3
LIBS = -lpthread

recuperarDados:
	$(PY) historicoHandler/manipulaHistorico.py recuperarDados criarClosePricesBin

createBins:
	$(PY) historicoHandler/manipulaHistorico.py criarClosePricesBin

deepclean:
	rm -rf *~ *.o dadosHistoricos/*.bin *.bin *.exe *.stackdump __pycache__ *.npy

clean:
	rm -rf *~ *.o *.exe *.stackdump __pycache__

# run: runC++
#
# runC++: bollingerTraderTester.exe
# 	./bollingerTraderTester.exe
# compile:
# 	$(CXX) $(CXXFLAGS) code/trader/bollingerTraderTester.cpp -o bollingerTraderTester.exe $(LIBS)
#
# initPython:
# 	export PYTHONPATH=$PYTHONPATH:$(PROJECT_PATH)/Trading/code/historicoHandler
# 	export PYTHONPATH=$PYTHONPATH:$(PROJECT_PATH)/Trading/code/utils
#
# runJulia: initPython
# 	export JULIA_NUM_THREADS=4
#
# 	$(JL) code/trader/runJuliaTrader.jl
#
# runPython: initPython
# 	$(PY) code/trader/trading.py
