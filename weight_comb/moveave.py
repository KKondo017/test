import csv
import datetime
import numpy as np
import os
import serial
from decimal import Decimal,ROUND_HALF_UP

num = 5         # number of weignt averaging
numd = 2        # number for delta weignt calc
numave = 5      # number of delta weught averaging

a = []          # initial a. a = weight stock list.
b = []          # initial b. b = averaged weight stock list.
c = []          # initial c. c = delta weight stock list.

inj = False     # initial inj = False. inj = input judgement.
injw = 5        # injw = 5 [g]. injw = threshold of input jedgement weight.
convw = 0.5     # convw = 1. convw = threshold of convergence weight.
convc = 0       # initial convc = 0. convc = cout of convergence.
convcj = 6      # convcj = 4. convcj = number of convergence judgement counts - 1.
fave = None     # initial fave = None. fave = fixed average value.

com = serial.Serial('/dev/ttyACM0', 9600)
dt_today = datetime.datetime.today()
# %Y:年 %m:月 %d:日 %H:時 %M:分 %S:秒
filedir = '/home/pi/Desktop/test/classify/'
filename = dt_today.strftime('%Y%m%d') + '.csv'
filepath = filedir + filename

while True:
    with open(filepath, 'a', newline='') as f:
        writer = csv.writer(f)
        weight = float(com.readline().rstrip().decode(encoding='utf-8'))
        a.insert(0, weight)
        arrlen = len(a) - 1
        if arrlen == num:
            a.pop()
        
        ave = Decimal(str(np.mean(a))).quantize(Decimal('0.1'), rounding=ROUND_HALF_UP)
        b.insert(0, ave)
        blen =len(b) - 1
        if blen == numd:
            b.pop()
            dweight = b[0] - b[1]
            
            c.insert(0, dweight)
            clen = len(c) - 1
            if clen == numave:
                c.pop()
        
            dave = Decimal(str(np.mean(c))).quantize(Decimal('0.1'), rounding=ROUND_HALF_UP)
            
            if abs(dave) > injw:
                inj = True
            
            if inj ==True and abs(dave) < convw:
                convc = convc + 1
            else:
                convc = 0
            
            if convc == convcj:
                fave = ave
                inj = False
            else:
                fave = None
        
            now_com = datetime.datetime.now()
                
            data = [now_com.strftime('%H:%M:%S.%f'), weight, ave, dave,fave, filepath]
            print(data)
            writer.writerow(data)
            f.close()
        
    
com.close()