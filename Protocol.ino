#include <SoftwareSerial.h>
#include <Servo.h>

#define RxD 6
#define TxD 7

#define PIN_LEFTSERVO 9
#define PIN_RIGHTSERVO 10

#define FORWARD 1;
#define BACK -1;

#define LEFT_SENSOR A4
#define RIGHT_SENSOR A5
#define FRONT_SENSOR A3

SoftwareSerial blueToothSerial(RxD,TxD);
Servo left_servo;
Servo right_servo;

String inString;

void setupBlueToothConnection() {
    blueToothSerial.begin(57600);
    blueToothSerial.print("\r\n+STWMOD=0\r\n");
    blueToothSerial.print("\r\n+STBD=57600\r\n");
    blueToothSerial.print("\r\n+STNA=RobotServer\r\n");
    blueToothSerial.print("\r\n+STOAUT=1\r\n");
    blueToothSerial.print("\r\n+STAUTO=0\r\n");
    delay(2000);
    blueToothSerial.print("\r\n+INQ=1\r\n");
    delay(2000);
    blueToothSerial.flush();
}

void turnMotor(Servo &servo, int velocity) {
    servo.writeMicroseconds(1500 + velocity);
}

int readSensor(int pin) {
    return analogRead(pin);
}

void forward() {
    turnMotor(left_servo, -200);
    turnMotor(right_servo, 200);
}

void back() {
    turnMotor(left_servo, 200);
    turnMotor(right_servo, -200);
}

void left() {
    turnMotor(left_servo, 200);
    turnMotor(right_servo, 200);
}

void right() {
    turnMotor(left_servo, -200);
    turnMotor(right_servo, -200);
}

void stop() {
    turnMotor(left_servo, 0);
    turnMotor(right_servo, 0);
}

void setup() {
    Serial.begin(9600);
    
    pinMode(RxD, INPUT);
    pinMode(TxD, OUTPUT);
    setupBlueToothConnection();

    left_servo.attach(PIN_LEFTSERVO);
    right_servo.attach(PIN_RIGHTSERVO);
    stop();

    pinMode(FRONT_SENSOR, INPUT);
    pinMode(LEFT_SENSOR, INPUT);
    pinMode(RIGHT_SENSOR, INPUT);

    inString = "";
}


void loop() {
    while (1) { 
        if (blueToothSerial.available() > 0) {
            char inChar = blueToothSerial.read();
            if (inChar == '\n') {
                if (inString.charAt(0) == 'M') {
                    if (inString.charAt(1) == 'F') {
                        forward();
                    }
                    else if (inString.charAt(1) == 'B') {
                        back();
                    }
                    else if (inString.charAt(1) == 'L') {
                        left();
                    }
                    else if (inString.charAt(1) == 'R') {
                        right();
                    }      
                }
                else if (inString.charAt(0) == '0') {
                    delay(50);
                    stop();
                }
                else if (inString.charAt(0) == 'S') {
                    if (inString.charAt(1) == 'F') {
                        blueToothSerial.println(readSensor(FRONT_SENSOR));
                    }
                    else if (inString.charAt(1) == 'L') {
                        blueToothSerial.println(readSensor(LEFT_SENSOR));
                    }
                    else if (inString.charAt(1) == 'R') {
                        blueToothSerial.println(readSensor(RIGHT_SENSOR));
                    } 
                }
                inString = "";

            }
            else {
                inString += char(inChar);    
            }
        }
    }
}