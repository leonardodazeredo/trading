using PyCall

unshift!(PyVector(pyimport("sys")["path"]), "")
unshift!(PyVector(pyimport("sys")["path"]), "/home/leo/git/trading/Trading/code")
unshift!(PyVector(pyimport("sys")["path"]), "/home/leo/git/trading/Trading/code/utils")
unshift!(PyVector(pyimport("sys")["path"]), "/home/leo/git/trading/Trading/code/trader")
unshift!(PyVector(pyimport("sys")["path"]), "/home/leo/git/trading/Trading/code/historicoHandler")
