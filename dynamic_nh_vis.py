import matplotlib.pyplot as plt

#
# This script is for visualizing the rutimes of the dynamic value heuristic and saves it as out.pdf
# This file needs some manual setting up
#

values_15 = []
values_30 = []
values_45 = []
values_e  = []
values_s  = []

# information about the tests
min_size = 6
max_size = 66
increment = 6

repititions = 3


# read runtimes
file = open(".snh_runtime_0.150000.txt", "r")
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
        values_15.append(tmp)
        tmp = 0
file.close()

# read runtimes
file = open(".snh_runtime_0.300000.txt", "r")
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
file = open(".snh_runtime_0.450000.txt", "r")
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
        values_45.append(tmp)
        tmp = 0
file.close()

# read runtimes
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
        values_s.append(tmp)
        tmp = 0
file.close()

# read runtimes
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
        values_e.append(tmp)
        tmp = 0
file.close()

# for scale
scaler = []
for i in range(min_size, max_size+1, increment):
    scaler.append(i)

vs , = plt.plot(scaler, values_s )
ve , = plt.plot(scaler, values_e )
v15, = plt.plot(scaler, values_15)
v30, = plt.plot(scaler, values_30)
v45, = plt.plot(scaler, values_45)

plt.xlabel('graph size')
plt.yscale('log', base=2)
plt.ylabel('runtime in seconds')

plt.xlim(min_size, max_size)

# names of the values
vs.set_label('schuster')
ve.set_label('eppstein')
v15.set_label('schuster (h=15%)')
v30.set_label('schuster (h=30%)')
v45.set_label('schuster (h=45%)')
plt.legend()

plt.savefig('out.pdf')

plt.show()
