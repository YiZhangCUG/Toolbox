# -*- coding:utf-8 -*-
# 
import numpy as np

x,y,z = np.loadtxt("topo.txt",unpack=True)

for i in range(x.size):
	print("%lf %lf %lf " % (x[i],y[i],z[i]-10))
	pass