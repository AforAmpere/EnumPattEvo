# filter-epe.py

import sys
import re

if len(sys.argv) == 1:
    print('Usage: python filter-epe.py <Output file.txt>')
    print()
    sys.exit()

file = sys.argv[1]
uniqShips = set()
uniqOscs = set()

with open(file) as Fin:
    for line in Fin:
        #res = shipRE.match(line.strip())
        #print(line)
        res=[i.strip() for i in line.strip().split(', ')]
        if res:
            try:
                pop,rule,dx,dy,period,RLE = res
            except:
                continue
            dx=int(dx)
            dy=int(dy)
            period=int(period)
            speed = (dx, dy, period)
            if abs(dx) > 0 or abs(dy) > 0: uniqShips.add(speed)
            else: uniqOscs.add(period)

print()            
print('Found ', len(uniqOscs), ' oscillator periods')
oscPeriods = map(str, sorted(uniqOscs))
print(', '.join(oscPeriods))

print()
print('Found ', len(uniqShips), ' spaceship speeds')
shipSpeeds = sorted(uniqShips, key=lambda x: ((abs(float(x[0]))+abs(float(x[1])))/abs(x[2]),max(abs(float(x[0])),abs(float(x[1]))),min(abs(float(x[0])),abs(float(x[1])))))
#shipSpeeds = sorted(uniqShips, key=lambda x: (x[2],x[0],x[1]))
#shipSpeeds = sorted(uniqShips, key=lambda x: (abs(max(abs(x[0]),abs(x[1])))/(abs(min(abs(x[0]),abs(x[1]))+0.001))))
#shipSpeeds = sorted(uniqShips, key=lambda x: (x[0]/x[2], x[1]/x[2],x[2]))

for speed in shipSpeeds:
    dx, dy, period = speed
    if abs(dy) > 0 or abs(dx) > 0:
        color = ''
        outstr = '({},{})/{}'.format(dx, dy, period,max(abs(float(dx)),abs(float(dy)))/abs(period))
        if max(abs(dx),abs(dy))/period>3/4:
            color = '\x1b[1;35m'
        elif max(abs(dx),abs(dy))/period>2/3:
            color = '\x1b[1;33m'
        elif(2*max(abs(dx),abs(dy))+min(abs(dx),abs(dy))>period):
            color = '\x1b[1;31m'
        elif(2*abs(dx)+2*abs(dy)>period):
            color = '\x1b[1;32m'
        else:
            color = '\x1b[1;34m'
        #color=''
        #if color=='\x1b[1;32m':
        print(color+outstr)
