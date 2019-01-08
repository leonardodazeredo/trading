# a = zeros(10)
# f(x,y)= 1
# @parallel f for i = 1:10
#     a[i] = rand()
# end
#
# print(a)

include("../pycallon.jl")
include("trading.jl")

sort!(resultado,by=x->x[1])
println(resultado[end])
