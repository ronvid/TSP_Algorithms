import matplotlib.pyplot as plt

#
# This script is for visualizing the rutimes of the fixed value heuristic and saves it as out.pdf
# This file needs some manual setting up
#

values_10 = []
values_20 = []
values_30 = []
values_e  = []
values_s  = []

# information about the tests
min_size = 6
max_size = 66
increment = 6

repititions = 3


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
ve , = plt.plot(scaler, values_e)
v10, = plt.plot(scaler, values_10)
v20, = plt.plot(scaler, values_20)
v30, = plt.plot(scaler, values_30)


plt.xlabel('graph size')
plt.yscale('log', base=2)
plt.ylabel('runtime in seconds')

plt.xlim(min_size, max_size)

# setting the names of the values
vs.set_label('schuster')
v10.set_label('schuster (h=10)')
ve.set_label('eppstein')
v20.set_label('schuster (h=20)')
v30.set_label('schuster (h=30)')

plt.legend()

plt.savefig('out.pdf')

plt.show()
