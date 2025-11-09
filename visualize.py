import matplotlib.pyplot as plt

e_values = []
b_values = []
s_values = []
snh_values = []

# read stats file
file = open(".stats.txt", "r")
min_size = int(file.readline())
max_size = int(file.readline())
repititions = int(file.readline())

# padding
for i in range(min_size):
    e_values.append(0)
    b_values.append(0)
    s_values.append(0)
    snh_values.append(0)

# read eppstein runtimes
file = open(".e_runtime.txt", "r")
line = file.readline()
count = 0
tmp = 0
while line:
    tmp += float(line.strip())
    line = file.readline()
    count += 1
    if count == repititions:
        count = 0
        tmp = tmp/repititions
        e_values.append(tmp)
        tmp = 0
file.close()

# read bruteforce runtimes
file = open(".b_runtime.txt", "r")
line = file.readline()
count = 0
tmp = 0
while line:
    tmp += float(line.strip())
    line = file.readline()
    count += 1
    if count == repititions:
        count = 0
        tmp = tmp/repititions
        b_values.append(tmp)
        tmp = 0
file.close()

# read schuster runtimes
file = open(".s_runtime.txt", "r")
line = file.readline()
count = 0
tmp = 0
while line:
    tmp += float(line.strip())
    line = file.readline()
    count += 1
    if count == repititions:
        count = 0
        tmp = tmp/repititions
        s_values.append(tmp)
        tmp = 0
file.close()

# read schuster runtimes
file = open(".snh_runtime.txt", "r")
line = file.readline()
count = 0
tmp = 0
while line:
    tmp += float(line.strip())
    line = file.readline()
    count += 1
    if count == repititions:
        count = 0
        tmp = tmp/repititions
        snh_values.append(tmp)
        tmp = 0
file.close()

# for v in values:
#    print(v)
e, = plt.plot(e_values)
b, = plt.plot(b_values)
s, = plt.plot(s_values)
snh, = plt.plot(snh_values)
plt.yscale('log', base=2)
plt.ylabel('some numbers')

plt.xlim(min_size-1, max_size)
plt.xlim(10, 16)
# plt.ylim(plt.ylim())

e.set_label('eppstein')
b.set_label('bruteforce')
s.set_label('schuster')
snh.set_label('schuster (new heuristic)')
plt.legend()

plt.show()
