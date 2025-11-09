import matplotlib.pyplot as plt

values = []
for i in range(0,100):
    values.append(0)

# read values runtimes
file = open(".six_cycles.txt", "r")
line = file.readline()
while line:
    values[int(line)] += 1
    line = file.readline()
file.close()

# remove unused values
while True:
    if values[-1] == 0:
        values.pop()
    else:
        break

# for v in values:
#    print(v)
plt.plot(values)
plt.ylabel('some numbers')

plt.show()
