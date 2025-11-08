import matplotlib.pyplot as plt

e_values = []
b_values = []
s_values = []

# read eppstein runtimes
file = open(".e_runtime.txt", "r")
line = file.readline()
while line:
    e_values.append(float(line.strip()))
    line = file.readline()
file.close()

# read bruteforce runtimes
file = open(".b_runtime.txt", "r")
line = file.readline()
while line:
    b_values.append(float(line.strip()))
    line = file.readline()
file.close()

# read schuster runtimes
file = open(".s_runtime.txt", "r")
line = file.readline()
while line:
    s_values.append(float(line.strip()))
    line = file.readline()
file.close()


# for v in values:
#    print(v)
plt.plot(e_values)
plt.plot(b_values)
plt.plot(s_values)
plt.yscale('log', base=2)
plt.ylabel('some numbers')

plt.show()
