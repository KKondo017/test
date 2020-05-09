import sys
import serial
import csv
import datetime

sys.path.append('/home/pi/.local/lib/python3.7/site-packages')
import MySQLdb

com = serial.Serial('/dev/ttyACM2', 9600)
dt_today = datetime.datetime.today()
# %Y:年 %m:月 %d:日 %H:時 %M:分 %S:秒

def main(ctime, mass1, mass2):
    conn = MySQLdb.connect(
        user='root',
        passwd='password',
        host='localhost',
        db='test'
    )
    
    c = conn.cursor()
    sql ='update weight set date = %s, weight1 = %s, weight2 = %s;'
    c.execute(sql,(ctime, mass1, mass2))
    conn.commit()
    
while True:
    strmass = str(com.readline().rstrip().decode(encoding='utf-8'))
    masslist = strmass.split(",")
    mass1 = float(masslist[0])
    mass2 = float(masslist[1])
    now_com = datetime.datetime.now()
    ctime = now_com.strftime('%H:%M:%S')
    main(ctime, mass1, mass2)
    
    data = [ctime, mass1, mass2]
    
    
com.close()