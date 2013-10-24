#! /usr/bin/python

import csv
import matplotlib.pyplot as plt

def getColumn(filename, column):
    results = csv.reader(open(filename), delimiter=" ")
    return [result[column] for result in results]


empty = getColumn("out.csv",0)[1:]
humans = getColumn("out.csv",1)[1:]
zombies = getColumn("out.csv",2)[1:]
infected = getColumn("out.csv",3)[1:]
days = range(len(humans))

plt.figure("Populations")
plt.xlabel('Days')
plt.ylabel('Population')
plt.plot(days,humans,label='Humans')
plt.hold(True)
plt.plot(days,zombies,label='Zombies')
plt.plot(days,infected,label='Infected')
plt.legend(loc='lower left')
plt.hold(False)
plt.show()