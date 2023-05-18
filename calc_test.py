import matplotlib.pyplot as plt

core_clock = 9 * 10**6

Hz = list(range(2, 32000))
a = []
b = []
c = 0
# psc = a * b
for i in range(len(Hz)):
    j = 0
    psc = core_clock // Hz[i]
    if psc > 0xFFFF * 2:
        a += [psc ]
        b += []
print(c)
plt.plot(Hz, a)
plt.plot(Hz, b)
plt.show()

