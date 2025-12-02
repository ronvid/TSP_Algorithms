import matplotlib.pyplot as plt

#
# This script is for visualizing the rutimes of the different algorithms and saves it as out.pdf
# This file needs some manual setting up
#

e_values = []
s_values = []
snh_values = []

path = "results/high_choice_hh/" # path where the test results are saved

# information about the tests
min_size = 6
max_size = 66
increment = 6

repititions = 3


# read eppstein runtimes
file = open(path + "e_runtime.txt", "r")
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

# read schuster runtimes
file = open(path + "s_runtime.txt", "r")
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
file = open(path + "snh_runtime.txt", "r")
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

# for scale
scaler = []
for i in range(min_size, max_size+1, increment):
    scaler.append(i)


s, = plt.plot(scaler, s_values)
e, = plt.plot(scaler, e_values)
snh, = plt.plot(scaler, snh_values)

plt.xlabel('graph size')
plt.yscale('log', base=2)
plt.ylabel('runtime in seconds')


plt.xlim(min_size, max_size)

# setting the names of the values
s.set_label('schuster')
snh.set_label('schuster (largest 6-cycle)')
e.set_label('eppstein')
plt.legend()

plt.savefig('out.pdf')

plt.show()
