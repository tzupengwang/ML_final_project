import numpy as np
import matplotlib.pyplot as plt
from sklearn import svm, datasets
import math

def sigmoid(x):
  return 1 / (1 + math.exp(-x))

np.set_printoptions(threshold=np.nan)
ind , X , Y = [] , [] , []
ind2 , X2 , Y2 = [] , [] , []

with open( "./sample_train_x.csv" , "r" ) as f :
    line = f.readline()
    while True:
        line = f.readline()
        if len( line ) == 0: break
        a = [float(x) for x in line.split(',')]
        ind.append(int(a[0]))
        X.append(a[1:])

with open( "./truth_train.csv" , "r" ) as f :
    while True:
        line = f.readline()
        if len( line ) == 0: break
        a = [float(x) for x in line.split(',')]
        Y.append(1 if a[1] == 1 else -1)

X = np.array(X)
Y = np.array(Y)

with open( "./sample_test_x.csv" , "r" ) as f :
    line = f.readline()
    while True:
        line = f.readline()
        if len( line ) == 0: break
        a = [float(x) for x in line.split(',')]
        ind2.append(int(a[0]))
        X2.append(a[1:])
X2 = np.array( X2 )

CClg = [-6 , -4 , -2 , 0 , 2]
#CC = [0.01]
wlen = []

CC = [1e-6, 1e-4, 1e-2, 1, 100]
cc = CC[ 3 ]

lin_svm = svm.LinearSVC(C=cc).fit(X, Y)

print('done')

#  svidx = lin_svm.support_
#  svs = lin_svm.support_vectors_
#  [ dualco ] = lin_svm.dual_coef_

#print( len(svidx) )
#print( len(dualco) )
#print( dualco )
#  w = np.array( [ 0.0, 0.0 ] )
#  for i in range(len(svidx)):
    #  w[ 0 ] += dualco[ i ] * svs[ i ][ 0 ]
    #  w[ 1 ] += dualco[ i ] * svs[ i ][ 1 ]

Y2 = lin_svm.predict( X2 )
print(Y2)

with open( "./submission1.csv" , "w" ) as f :
    for i in range(len(Y2)):
        f.write( '%d,%f\n' % (ind2[ i ], sigmoid(Y2[ i ])) )
#  wlen.append( np.linalg.norm(w) )

#  plt.plot(CClg, wlen)
#  plt.xlabel('log10C')
#  plt.ylabel('||w||')
#  plt.show()
#  print(wlen)
'''
h = 0.01
x_min, x_max = X[:, 0].min() - 1, X[:, 0].max() + 1
y_min, y_max = X[:, 1].min() - 1, X[:, 1].max() + 1
xx, yy = np.meshgrid(np.arange(x_min, x_max, h),
                     np.arange(y_min, y_max, h))

Z = lin_svm.predict(np.c_[xx.ravel(), yy.ravel()])

# Put the result into a color plot
Z = Z.reshape(xx.shape)
plt.contourf(xx, yy, Z, cmap=plt.cm.Paired, alpha=0.8)

# Plot also the training points
plt.scatter(X[:, 0], X[:, 1], c=Y, cmap=plt.cm.Paired)
plt.xlabel('x1')
plt.ylabel('x2')
plt.xlim(xx.min(), xx.max())
plt.ylim(yy.min(), yy.max())

plt.show()
'''

