
import sys

file_path = sys.argv[1]

file_name = sys.argv[2]

num = [2,4,6,8,10,15,20,30,50,80,100,150,200,300,500,1000]

time_old = [0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0]
time_new = []

for num_i in num:
    file = open(file_path+'/'+file_name+'-'+str(num_i)+'.csv', 'r')
    i = -2
    time_new_single = []
    for file_i in file:
        if file_i == ',\n': continue
        if i >= 0:
            time = file_i[:-1].split(',')
            time_old[i] += float(time[0])
            time_new_single.append(float(time[1]))
        i+=1
    time_new.append(time_new_single)
time_old2 = []
for time_old_i in time_old:
    time_old2.append(time_old_i/len(time_old))

file_out = open(file_path + '/all.csv','w')
file_out.write('old,2,4,6,8,10,15,20,30,50,80,100,150,200,300,500,1000\n')
j = 0

# for aa in time_new:
#     for aaa in aa:
#         print(aaa)
#     print('\n')


while j < len(time_old):
    f = str(time_old2[j]) + ','
    for time_new_i in time_new:
        f = f + str(time_new_i[j]) + ','
    file_out.write(f[:-1] + '\n')
    j+=1

