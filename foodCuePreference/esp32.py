import serial

class ESP32:
    # Class contructor
    def __init__(self, port, baudRate):
        self.port = port
        self.baudRate = baudRate

        # Instantiate a serial port without opening it
        self.ser = serial.Serial()

        # Configure the serial port
        self.ser.port = port
        self.ser.baudrate = baudRate
        self.ser.timeout = .2
        # self.ser.parity = serial.PARITY_EVEN


    # Method to support managing the port by using a "with" statement    
    def __enter__(self):
        if (self.ser.isOpen() == False):
            self.ser.open()
        else:
            print("Serial connection already opened.")

       
    # Method to support managing the port by using a "with" statement    
    def __exit__(self, exc_type, exc_value, tb):
        if (self.ser.isOpen() == True):
            self.ser.close()
        else:
            print("Serial Connection already closed.")

    def dataWaiting(self):
        return self.ser.in_waiting

    def read(self):
        msg = self.ser.readline()
        msg = msg.decode('utf-8').rstrip()
        return msg

    def write(self):
        pass





if (__name__  == "__main__"):

    # Instantiates an object
    esp = ESP32("COM3", 115200)

    with esp:
        while True:
            if esp.dataWaiting():
                message = esp.read()
                print(message)