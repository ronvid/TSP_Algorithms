import matplotlib.pyplot as plt

values_15 = []
values_30 = []
values_45 = []
values_40 = []
values_50 = []
values_60 = []
values_70 = []
values_80 = []
values_90 = []
values_e  = []
values_s  = []

# read stats file
# file = open(".stats.txt", "r")
# min_size = int(file.readline())
# max_size = int(file.readline())
# repititions = int(file.readline())

min_size = 6
max_size = 66
increment = 6

repititions = 3

# padding
# for i in range(min_size):
#     values_10.append(0)
#     values_20.append(0)
#     values_30.append(0)
#     values_40.append(0)
#     values_50.append(0)
#     values_60.append(0)
#     values_70.append(0)
#     values_80.append(0)
#     values_90.append(0)
#     values_e.append(0)
#     values_s.append(0)

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
file = open(".snh_runtime_0.400000.txt", "r")
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
file = open(".snh_runtime_0.500000.txt", "r")
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
file = open(".snh_runtime_0.600000.txt", "r")
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

# read runtimes
file = open(".snh_runtime_0.700000.txt", "r")
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
        values_70.append(tmp)
        tmp = 0
file.close()

# read runtimes
file = open(".snh_runtime_0.800000.txt", "r")
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
        values_80.append(tmp)
        tmp = 0
file.close()

# read runtimes
file = open(".snh_runtime_0.900000.txt", "r")
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
        values_90.append(tmp)
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
#v40, = plt.plot(values_40)
#v50, = plt.plot(values_50)
#v60, = plt.plot(values_60)
#v70, = plt.plot(values_70)
#v80, = plt.plot(values_80)
# v90, = plt.plot(values_90)

plt.xlabel('graph size')
plt.yscale('log', base=2)
plt.ylabel('runtime in seconds')

plt.xlim(min_size, max_size)
#plt.xlim(10, 35)
#plt.xlim(10, 16)
# plt.ylim(plt.ylim())

vs.set_label('schuster')
ve.set_label('eppstein')
v15.set_label('schuster (h=15%)')
v30.set_label('schuster (h=30%)')
v45.set_label('schuster (h=45%)')
#v40.set_label('schuster (h=40%)')
#v50.set_label('schuster (h=50%)')
#v60.set_label('schuster (h=60%)')
#v70.set_label('schuster (h=70%)')
#v80.set_label('schuster (h=80%)')
#v90.set_label('schuster (h=90%)')
plt.legend()

plt.savefig('out.pdf')

plt.show()
