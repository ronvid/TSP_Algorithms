import matplotlib.pyplot as plt

e_values = []
#b_values = []
s_values = []
snh_values = []

# read stats file
#file = open(".stats.txt", "r")
#min_size = int(file.readline())
#max_size = int(file.readline())
#repititions = int(file.readline())

path = "results/hh_less22/"

min_size = 6
max_size = 66
increment = 6

repititions = 3

# padding
# for i in range(min_size):
#     e_values.append(0)
#     b_values.append(0)
#     s_values.append(0)
#     snh_values.append(0)

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

# read bruteforce runtimes
# file = open(path + "b_runtime.txt", "r")
# line = file.readline()
# count = 0
# tmp = 0
# while line:
#     tmp += float(line.strip())
#     line = file.readline()
#     count += 1
#     if count == repititions:
#         count = 0
#         tmp = tmp/repititions
#         b_values.append(tmp)
#         tmp = 0
# file.close()

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

# for v in values:
#    print(v)

# for scale
scaler = []
for i in range(min_size, max_size+1, increment):
    scaler.append(i)


#b, = plt.plot(b_values)
s, = plt.plot(scaler, s_values)
e, = plt.plot(scaler, e_values)
snh, = plt.plot(scaler, snh_values)

plt.xlabel('graph size')
plt.yscale('log', base=2)
plt.ylabel('runtime in seconds')

#plt.xticks(range(3,90,10))


plt.xlim(min_size, max_size)
#plt.xlim(5,17)
# plt.ylim(plt.ylim())


#b.set_label('bruteforce')
s.set_label('schuster')
snh.set_label('schuster (<22)')
e.set_label('eppstein')
plt.legend()

plt.savefig('out.pdf')

plt.show()
