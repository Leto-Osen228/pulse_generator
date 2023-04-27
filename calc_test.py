import matplotlib.pyplot as plt

core_clock = 9 * 10**6

Hz = list(range(2, 32000))
a = []
b = []
c = 0
# psc = a * b
for i in range(len(Hz)):
    j = 32
    psc = core_clock // Hz[i]
    #print(i, psc, Hz[i])
    while (psc >> j) & 1 == 0:
        j+=1
    try:
        a += [psc >> j]
        b += [float(1 << j)]
    except OverflowError:
        print(j)
    if psc >> j > 2**16:
        c += 1
print(c)
plt.plot(Hz, a)
#plt.plot(Hz, b)
plt.show()

