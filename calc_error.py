import matplotlib.pyplot as plt

core_clock = 72 * 10**6
error = []
Hz = list(range(1, 32000))
for i in range(len(Hz)):
    psc = int(core_clock / Hz[i])
    error += [abs(core_clock / psc - Hz[i]) / Hz[i] * 100]

plt.plot(Hz, error)
plt.show()
