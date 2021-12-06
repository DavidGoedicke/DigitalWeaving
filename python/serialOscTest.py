import serial
from pythonosc import udp_client


ser = serial.Serial('/dev/ttyUSB0')  # open serial port
print(ser.name)         # check which port was really used
client = udp_client.SimpleUDPClient("127.0.01", 12345)

while True:
    if ser.is_open:

        line = ser.readline()
        print(str(line))
        if (not line ==None )and 'p' in str(line):
            print("GotAButtonPress")
            client.send_message("/pattern", [0,0,1,1,2,2,3,3])
