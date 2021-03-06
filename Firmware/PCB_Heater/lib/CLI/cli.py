import argparse
import serial
import time

def main():
    print("PCB Heater\n")
    
    # Read arguments from input
    parser = argparse.ArgumentParser()
    parser.add_argument("-sr", required=True, help="Soak ramp value")
    parser.add_argument("-st", required=True, help="Soak temperature value")
    parser.add_argument("-stim", required=True, help="Soak time in seconds")
    parser.add_argument("-pr", required=True, help="Peak ramp value")
    parser.add_argument("-ptim", required=True, help="Peak time in seconds")
    parser.add_argument("-cr", required=True, help="Cooling ramp value")
    args = vars(parser.parse_args())
    
    print("Given parameters:")
    print((f"Soak ramp: {args['sr']}, Soak temperature: {args['st']}, Soak time: {args['stim']}, " 
        f"Peak ramp: {args['pr']}, Peak time: {args['ptim']}, Cool ramp: {args['cr']}"))

    # Initialize UART
    #ser = serial.Serial('/dev/ttyUSB0', 9600)
    ser = serial.Serial(port='COM3', baudrate=9600, bytesize=8)

    # Send parameters over UART
    for arg in args.values():
        ser.write(bytes(arg, 'utf-8'))
        time.sleep(0.05)
 
  


if __name__== "__main__":
    main()