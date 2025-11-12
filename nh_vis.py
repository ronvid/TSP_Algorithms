import matplotlib.pyplot as plt

values_10 = []
values_20 = []
values_30 = []
values_40 = []
values_50 = []
values_60 = []

# read stats file
file = open(".stats.txt", "r")
min_size = int(file.readline())
max_size = int(file.readline())
repititions = int(file.readline())

# padding
for i in range(min_size):
    values_10.append(0)
    values_20.append(0)
    values_30.append(0)
    values_40.append(0)
    values_50.append(0)
    values_60.append(0)

# read runtimes
file = open(".snh_runtime_10.txt", "r")
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
        values_10.append(tmp)
        tmp = 0
file.close()

# read runtimes
file = open(".snh_runtime_20.txt", "r")
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
        values_20.append(tmp)
        tmp = 0
file.close()

# read runtimes
file = open(".snh_runtime_30.txt", "r")
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
        values_30.append(tmp)
        tmp = 0
file.close()

# read runtimes
file = open(".snh_runtime_40.txt", "r")
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
        values_40.append(tmp)
        tmp = 0
file.close()

# read runtimes
file = open(".snh_runtime_50.txt", "r")
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
        values_50.append(tmp)
        tmp = 0
file.close()

# read runtimes
file = open(".snh_runtime_60.txt", "r")
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
        values_60.append(tmp)
        tmp = 0
file.close()

v10, = plt.plot(values_10)
v20, = plt.plot(values_20)
v30, = plt.plot(values_30)
v40, = plt.plot(values_40)
v50, = plt.plot(values_50)
v60, = plt.plot(values_60)

plt.yscale('log', base=2)
plt.ylabel('some numbers')

plt.xlim(min_size-1, max_size)
#plt.xlim(10, 35)
#plt.xlim(10, 16)
# plt.ylim(plt.ylim())

v10.set_label('schuster (h=10)')
v20.set_label('schuster (h=20)')
v30.set_label('schuster (h=30)')
v40.set_label('schuster (h=40)')
v50.set_label('schuster (h=50)')
v60.set_label('schuster (h=60)')
plt.legend()

plt.show()
