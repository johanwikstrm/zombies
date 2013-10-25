#! /usr/bin/python

import csv
import sys
import matplotlib.pyplot as plt

def getColumn(filename, column):
    results = csv.reader(open(filename), delimiter=" ")
    return [result[column] for result in results]


filename = sys.argv[1]
empty = getColumn(filename,0)[2:]
humans = getColumn(filename,1)[2:]
zombies = getColumn(filename,2)[2:]
infected = getColumn(filename,3)[2:]
days = range(len(humans))

plt.figure("Populations")
plt.xlabel('Days')
plt.ylabel('Population')
plt.plot(days,humans,label='Humans')
plt.hold(True)
plt.plot(days,zombies,label='Zombies')
plt.plot(days,infected,label='Infected')
plt.legend(loc='upper left')
plt.hold(False)
plt.savefig(filename+'.png',bbox_inches='tight')
plt.show()