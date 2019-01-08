def frange(start, stop, step):
    r = stop/step
    for x in range(int(start*r),int(stop*r),int(step*r)):
        yield float('%.1f'%(x/r))
