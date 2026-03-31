from sklearn import metrics
import sys
# labels_true = [0, 0, 0, 0,0, 1, 1,1,2,2,2,2]
# labels_pred = [0, 0, -1,0, 0, 0,0,0,2,2,2,2]
#
# # 基本用法
# score = metrics.adjusted_rand_score(labels_true, labels_pred)
# print(score)  # 0.24242424242424246
#
#
# labels_true = [0, 0, 0, 0, 0,1, 1,1,2,2,2,2]
# labels_pred = [3,3,3,3,7, 7, 7, 7,2,2,2,2]
#
# # 基本用法
# score = metrics.adjusted_rand_score(labels_true, labels_pred)
# print(score)  # 0.24242424242424246

file_path = sys.argv[1]
# file_path = '/Users/milk/xcode project/index_new2_approximate/index_new2_approximate/output'


li = ['2','4','6','8','10','15','20','30','50','80','100','150','200','300','500','1000']
ari = [0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0]
lii = 1
while lii<=14:
    j = 0
    for i in li:
        file1 = open(
            file_path + '/' + str(lii) + '.000000-'+str(lii) + '.000000-1-' + i + '.txt',
            'r')
        file2 = open(
            file_path + '/' + str(lii) + '.000000-'+str(lii) + '.000000-2-' + i + '.txt',
            'r')

        a = {}
        b = {}
        max = 0
        f_i = 0
        for f in file1:
            f = f[0:-1]
            f2 = f.split(' ')
            a[int(f2[1])] = int(f2[2])
            if int(f2[1]) > max: max = int(f2[1])
            if int(f2[2]) > max: max = int(f2[2])
            if f_i>585:
                break
            f_i+=1
        f_i = 0
        for f in file2:
            f = f[0:-1]
            f2 = f.split(' ')
            b[int(f2[1])] = int(f2[2])
            if int(f2[1]) > max: max = int(f2[1])
            if int(f2[2]) > max: max = int(f2[2])
            if f_i>585:
                break
            f_i+=1
        all_num = max
        a2 = []
        b2 = []

        i = 0
        # print(max)
        while i < all_num:
            if i in a:
                a2.append(a[i])
                # a2.append(i)
            else:
                a2.append(i)
            if i in b:
                b2.append(b[i])
                # b2.append(i)
            else:
                b2.append(i)
            i+=1

        score = metrics.adjusted_rand_score(a2, b2)
        # print(str(lii) + '-'+i)
        print(score)
        # file_out.write(str(score)+'\n')
        ari[j] += score
        j+=1



    lii = lii + 1
    print()
    

file_out = open(file_path+'/all.csv','w')
for ari_i in ari:
    file_out.write(str(ari_i/14)+'\n')
file_out.close()




