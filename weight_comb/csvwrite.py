import serial
import csv
import datetime

com = serial.Serial('/dev/ttyACM0', 115200)
dt_today = datetime.datetime.today()
# %Y:年 %m:月 %d:日 %H:時 %M:分 %S:秒
filedir = '/home/pi/Desktop/test/classify/'
filename = dt_today.strftime('%Y%m%d') + '.csv'
    
while True:
    with open(filedir + filename, 'a', newline='') as f:
        writer = csv.writer(f)
        strweight = str(com.readline().rstrip().decode(encoding='utf-8'))
        weightlist = strweight.split(",")
        weight1 = float(weightlist[0])
        weight2 = float(weightlist[1])
        now_com = datetime.datetime.now()
    
        data = [now_com.strftime('%H:%M:%S.%f'), weight1, weight2]
        print(data)
        writer.writerow(data)
        f.close()
        
com.close()
