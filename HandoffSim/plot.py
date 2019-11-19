import matplotlib.pyplot as plt
import numpy as np
y1= np.loadtxt('best.txt', unpack=True)
y2= np.loadtxt('threshold.txt', unpack=True)
y3= np.loadtxt('entropy.txt', unpack=True)
y4= np.loadtxt('self.txt', unpack=True)
x=np.arange(0,86401)
line1, = plt.plot(x, y1, color = 'red', linewidth = 1, label = 'Best')             
line2, = plt.plot(x, y2, color = 'blue', linewidth = 1, label = 'Threshold')
line3, = plt.plot(x, y3, color = 'green', linewidth = 1, label = 'Entropy')
line4, = plt.plot(x, y4, color = 'orange', linewidth = 1, label = 'Self Method')
plt.legend(handles = [line1, line2, line3, line4], loc='lower right')
plt.savefig("result.png",dpi=300,format="png")