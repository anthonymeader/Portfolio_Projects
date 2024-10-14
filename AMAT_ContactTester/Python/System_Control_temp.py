#--------------------------------------------
# Writen by: Anthony Meader
#--------------------------------------------
import serial
import time
import queue
import csv
import os
from datetime import datetime

# Set up the serial port, UART to MCU
ser = serial.Serial(port="COM7", baudrate=9600, bytesize=8, timeout=2,
                    stopbits=serial.STOPBITS_ONE) #FIX ME make com port & boad Rate a user input
pws = serial.Serial('COM1', 9600, timeout=1) #FIX ME make com port & BOAD Rate a suer input

data_queue = queue.Queue() #Define Queue
#------------------------------------------------------------------------------------------------------
#------------------------------------------------------------------------------------------------------

#------------------------------------------------------------------------------------------------------
# Functions to Define and Toggle PWS
def to_ascii(str0):
    return bytes(str0, 'ascii')
def define_power(current, voltage):
    to_vol = 'VSET1:' + str(int(voltage / 1000)) + '.' + "{:0>3d}".format(int(voltage - 1000 * int(voltage / 1000))) + '\n'
    pws.write(to_ascii(to_vol))
    time.sleep(0.1)
    to_cur = 'ISET1:' + str(int(current / 1000)) + '.' + "{:0>3d}".format(int(current - 1000 * int(current / 1000))) + '\n'
    pws.write(to_ascii(to_cur))
def pws_on():
    pws_on = 'OUT1' + '\n'
    pws.write(to_ascii(pws_on))
def pws_off():
    pws_off = 'OUT0' + '\n'
    pws.write(to_ascii(pws_off))
#------------------------------------------------------------------------------------------------------
#------------------------------------------------------------------------------------------------------

#------------------------------------------------------------------------------------------------------
# Function to step motor and read pressure sensor
def read_pressure():
    ser.reset_input_buffer()  # Clear input buffer, TEST
    ser.write('A'.encode('Ascii'))  # Get Resistance
    while True:
        if ser.in_waiting > 0:  # Wait until data is available
            receive = ser.read(size=2)
            #if len(receive) == 2:  
            high_byte = receive[0]
            low_nibble = receive[1]
            combined_value = (high_byte << 4) | (low_nibble & 0x0F)  # Combine high byte and low nibble
            return combined_value
        else:
            time.sleep(0.01)  # Short sleep to avoid busy-waiting
#------------------------------------------------------------------------------------------------------
#------------------------------------------------------------------------------------------------------


#------------------------------------------------------------------------------------------------------
# Function to read resistance on DUT
def read_resistance():
    ser.reset_input_buffer()  # Clear input buffer, TEST
    ser.write('B'.encode('Ascii'))  # Get Resistance
    while True:
        if ser.in_waiting > 0:  # Wait until data is available
            receive = ser.read(size=2)
            #if len(receive) == 2:  #TEST
            high_byte = receive[0]
            low_nibble = receive[1]
            combined_value = (high_byte << 4) | (low_nibble & 0x0F)  # Combine high byte and low nibble
            return combined_value
        else:
            time.sleep(0.01)  # Short sleep to avoid busy-waiting
#------------------------------------------------------------------------------------------------------
#------------------------------------------------------------------------------------------------------

#------------------------------------------------------------------------------------------------------
# Orchestrate MCU Communication Function
def cycle(test_name,total_cycles,current_cycles,freq,current, current_on,distance,mount,stop_event):
    data_array = []
    start_data =datetime.now().strftime("%Y-%m-%d_%H:%M:%S") #Start of test, used in CSV
    folder_name = datetime.now().strftime("%Y%m%d") #Start dat of test, used for folder name
    folder_name = test_name+"_"+folder_name
    frequency = 0

    define_power(current, 30000) 

    ser.write('D'.encode('Ascii'))  # Define actuation distance command
    time.sleep(2)
    ser.write(chr(distance).encode('Ascii'))  # Define actuation distance

    ser.write('E'.encode('Ascii'))
    time.sleep(2)
    ser.write(chr(mount).encode('Ascii'))


    while current_cycles < total_cycles:

        pressure = read_pressure()
        if pressure >= 1000:  # Check pressure threshold
            frequency = frequency + 1
            pws_on()
            time.sleep(.1) #Init Delay
            time.sleep(current_on) #Current on Time
            pws_off()
            time.sleep(.1) #Init Delay

            if frequency == freq:
                resistance = read_resistance() 
                print(resistance)
                voltage = (3.28/4095*resistance)-(0.005*5.95) #CHANGE BASED ON FINAL CALIBRATION
                resistor = 20.43/(5.004-(voltage/5.95))*(voltage/5.95) #Equation to get reistance CHANGE BASED ON FINAL CALIBRATION
                resistor = round(resistor,4) #round to nearest hund
                frequency = 0

                # Add data to the queue
                data_queue.put({"cycle": current_cycles+1, "pressure": pressure, "voltage": round(voltage,3), "resistance": resistor})
                data_storage(data_array,current_cycles+1,pressure,round(voltage,2),resistor)
            current_cycles = current_cycles + 1
            if resistance > 3370: #fail at 2 ohms, stop test, CHANGE BASED ON FINAL CALIBRATION
                current_cycles = total_cycles
                break # ADDED this without testing FIXME
        if stop_event.is_set():
            print("Stopping the test...")
            current_cycles = total_cycles
            break
    if current_cycles == total_cycles:
        current_cycles = 0
        ser.write('C'.encode('Ascii'))  # Bring back actuator
        write_csv(data_array,test_name,folder_name,total_cycles,freq,current,current_on,start_data)
#------------------------------------------------------------------------------------------------------
#------------------------------------------------------------------------------------------------------

#------------------------------------------------------------------------------------------------------
# Function to store data, might change, TEST
def data_storage(data_array, col1, col2, col3, col4):
    row = [col1, col2, col3, col4]
    data_array.append(row)
#------------------------------------------------------------------------------------------------------
#------------------------------------------------------------------------------------------------------
def write_csv(data_array,test_name,folder_name,total_cycles,freq,current,current_on,start_data):
    os.makedirs(folder_name, exist_ok=True)
    filename = os.path.join(folder_name, test_name+".csv")

    header = ['Cycle #', 'Pressure', 'ADC Measurement (V)', 'Resistance (Ohms)',"Total Cycles", "Measurement Frequency",
             "Current Applied (Amps)", "Current Time (S)", "Start Date/Time", "End Date/Time"]
    ui_inputs = ["", "", "", "", str(total_cycles), str(freq), str(current), str(current_on), str(start_data), str(datetime.now().strftime("%Y-%m-%d_%H:%M:%S"))]
    with open(filename, 'w', newline='') as csvfile:
        csvwriter = csv.writer(csvfile)
        csvwriter.writerow(header)
        csvwriter.writerow(ui_inputs)
        for row in data_array: #Populate CSV with data, test up to 500k
            csvwriter.writerow(row)








