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

# avg values (assuming 10 runs)
#for i in range(0, len(values)):
#    values[i] = values[i]/200

# for v in values:
#    print(v)
plt.plot(values)
plt.ylabel('6-cycle branches')
plt.xlabel('instance size')


plt.savefig('avg.pdf')
plt.show()
