#include <SoftwareSerial.h>
#include <Servo.h>

#define RxD 6
#define TxD 7

#define PIN_LEFTSERVO 9
#define PIN_RIGHTSERVO 10

#define FORWARD 1;
#define BACK -1;

#define LEFT_SENSOR A4
#define RIGHT_SENSOR A3
#define FRONT_SENSOR A2

SoftwareSerial blueToothSerial(RxD,TxD);
Servo left_servo;
Servo right_servo;

String inString;

void setupBlueToothConnection() {
    blueToothSerial.begin(57600);
    blueToothSerial.print("\r\n+STWMOD=0\r\n");
    blueToothSerial.print("\r\n+STNA=RobotServer\r\n");
    blueToothSerial.print("\r\n+STOAUT=1\r\n");
    blueToothSerial.print("\r\n+STAUTO=0\r\n");
    delay(2000);
    blueToothSerial.print("\r\n+INQ=1\r\n");
    delay(2000);
    blueToothSerial.flush();
}

void turnMotor(Servo servo, int velocity) {
    servo.writeMicroseconds(1500 + velocity);
}

int readSensor(int pin) {
    return analogRead(pin);
}

void motorCallback(String command) {
    int sepIndex = command.indexOf(' ');
    String subcommand = command.substring(0, sepIndex);
    int velocity = command.substring(sepIndex + 1).toInt();

    Servo servo;
    int direction;
    if (subcommand.equals("LF")) {
        servo = left_servo;
        direction = FORWARD;
    } else if (subcommand.equals("RF")) {
        servo = right_servo;
        direction = BACK;
    } else if (subcommand.equals("LB")) {
        servo = left_servo;
        direction = BACK;
    } else if (subcommand.equals("RB")) {
        servo = right_servo;
        direction = FORWARD;
    }

    turnMotor(servo, direction * velocity);
}

int sensorCallback(String sensor) {
    switch (sensor.charAt(0)) {
        case 'F':
            blueToothSerial.println(readSensor(FRONT_SENSOR));
            break;
        case 'L':
            blueToothSerial.println(readSensor(LEFT_SENSOR));
            break;
        case 'R':
            blueToothSerial.println(readSensor(RIGHT_SENSOR));
            break;
        default:
            blueToothSerial.println(readSensor(FRONT_SENSOR));
    }
}

void setup() {
    Serial.begin(9600);
    
    pinMode(RxD, INPUT);
    pinMode(TxD, OUTPUT);
    setupBlueToothConnection();

    left_servo.attach(PIN_LEFTSERVO);
    right_servo.attach(PIN_RIGHTSERVO);
    //turnMotor(left_servo, 0);
    //turnMotor(right_servo, 0);

    pinMode(FRONT_SENSOR, INPUT);
    pinMode(LEFT_SENSOR, INPUT);
    pinMode(RIGHT_SENSOR, INPUT);

    inString = "";
}


void loop() {
    while (1) {  
        if (blueToothSerial.available() > 0) {
            int inChar = blueToothSerial.read();
    
            if (inChar == '\n') {
    
                // int sepIndex = inString.indexOf(' ');
                // String command = inString.substring(0, sepIndex);
                // String subcommand = inString.substring(sepIndex + 1);
                // if (command.equals("MOTOR")) {
                //     motorCallback(subcommand);
                // }
                // else if (command.equals("SENSOR")) {
                //     sensorCallback(subcommand);
                // }
                // inString = "";
                if (inString.charAt(0) == 'M') {
                    if (inString.charAt(1) == 'F') {

                    }
                    else if (inString.charAt(1) == 'B') {
                        motorCallback("LF 200");
                        motorCallback("RF 200");  
                    }
                    
                }
                else if (inString.charAt(0) == '0') {
                    motorCallback("LF 0");
                    motorCallback("RF 0");
                }
                inString = "";
            }
            else {
                inString += char(inChar);    
            }
        }
    }
}