import serial, string, time
ser = serial.Serial('/dev/ttyACM1', 9600) #change port to whatever port the arduinio you plug in uses first one is generally ACM0 and second is ACM1
import datetime, time
import gspread
from oauth2client.service_account import ServiceAccountCredentials
 
scope = ['https://spreadsheets.google.com/feeds','https://www.googleapis.com/auth/drive']
creds = ServiceAccountCredentials.from_json_keyfile_name('/path to your json file', scope)  #paste your json file path in here
client = gspread.authorize(creds)
sheet = client.open("Sheet_name_here").sheet1 #name of sheet you shared with the cloud console
 
while True:
      if ser.in_waiting > 0:
           rawserial = ser.readline()
           cookedserial = rawserial.decode('utf-8').strip('\r\n')
           datasplit = cookedserial.split(',')
           temperature = datasplit[0].strip('<')
           humidity = datasplit[1].strip('?')
           time = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S.%f')
           moisture = datasplit[2].strip('>')
           print(temperature)
           print(humidity)     
           print(time)
           print(moisture)
           data = temperature
           data2 = humidity
           data3 = moisture
           values =[time,data,data2,data3]
           sheet.append_row(values)
