import serial
import time
import csv
from datetime import datetime

def read_from_serial(port, baudrate):
    ser = serial.Serial(port, baudrate, timeout=1)
    time.sleep(2)  # Wait for the connection to establish
    return ser

def generate_filename_from_arduino(serial_data):
    date_time_str = serial_data.split(',')[0]  # Extract date and time part
    date_time_obj = datetime.strptime(date_time_str, "%Y/%m/%d %H:%M:%S")
    formatted_date_time = date_time_obj.strftime("%Y-%m-%d_%H-%M-%S")
    return f"hallData_{formatted_date_time}.csv"

def main():
    port = 'COM6'  # Replace with your port
    baudrate = 57600  # Replace with your baudrate

    ser = read_from_serial(port, baudrate)

    while True:
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').strip()
            if line:
                sensor_values = line.split(',')
                filename = generate_filename_from_arduino(line)
                with open(filename, 'w', newline='') as csvfile:
                    csvwriter = csv.writer(csvfile)
                    csvwriter.writerow(['Date', 'Time', 'Sensor1', 'Sensor2', 'Sensor3', 'Sensor4', 'Sensor5', 'Sensor6'])
                    break

    while True:
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').strip()
            if line:
                sensor_values = line.split(',')
                date_time = sensor_values[0]
                date_time_obj = datetime.strptime(date_time, "%Y/%m/%d %H:%M:%S")
                date_str = date_time_obj.strftime("%Y-%m-%d")
                time_str = date_time_obj.strftime("%H:%M:%S")
                with open(filename, 'a', newline='') as csvfile:
                    csvwriter = csv.writer(csvfile)
                    csvwriter.writerow([date_str, time_str] + sensor_values[1:])
                    print(f"Data written to {filename}: {date_str}, {time_str}, {sensor_values[1:]}")

if __name__ == '__main__':
    main()