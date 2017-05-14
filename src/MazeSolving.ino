#include <Servo.h>
#include "IO_Pins.h"

#define RIGHT +1
#define LEFT  -1
#define READING_COUNT 4
#define BORDER 300

Servo left_servo;
Servo right_servo;

void turnMotor(Servo &servo, int velocity) {
    servo.writeMicroseconds(1500 + velocity);
}

int isSensorDetect(int senpin, int ledpin, int distance=BORDER) {
    return readSensor(senpin, ledpin) > distance;
}

float readSensor(int senpin, int ledpin) {
    float distance;
    for (int i = 0; i < READING_COUNT; i++) {
        digitalWrite(ledpin, LOW);
        delay(1);
        float ambient = analogRead(senpin);

        digitalWrite(ledpin, HIGH);
        delay(1);
        float obstacle = analogRead(senpin);

        distance += ambient - obstacle;
    }

    return distance / READING_COUNT;
}

void turn_90(char dir) {
    turnMotor(left_servo, dir*70);
    turnMotor(right_servo, dir*70);

    delay(700);

    turnMotor(left_servo, 0);
    turnMotor(right_servo, 0);
}

void turn_180() {
    turn_90(RIGHT);
    turn_90(RIGHT);
}

void step_forward() {
    if (!isSensorDetect(FRONT_SENSOR_PIN, FRONT_LED_PIN)) {
        turnMotor(left_servo, 70);
        turnMotor(right_servo, -70);
    
        delay(1100);
    
        turnMotor(left_servo, 0);
        turnMotor(right_servo, 0);
    }  
}

void setup() {
    Serial.begin(9600);

    left_servo.attach(LEFT_SERVO_PIN);
    right_servo.attach(RIGHT_SERVO_PIN);

    pinMode(FRONT_LED_PIN, OUTPUT);
    pinMode(LEFT_LED_PIN, OUTPUT);
    pinMode(RIGHT_LED_PIN, OUTPUT);

    pinMode(FRONT_SENSOR_PIN, INPUT);
    pinMode(LEFT_SENSOR_PIN, INPUT);
    pinMode(RIGHT_SENSOR_PIN, INPUT);

    // turn_90(RIGHT);
    // turnMotor(left_servo, 200);
    // turnMotor(right_servo, -200);

}

void loop() {
    float rightDistance = readSensor(RIGHT_SENSOR_PIN, RIGHT_LED_PIN);
    float leftDistance = readSensor(LEFT_SENSOR_PIN, LEFT_LED_PIN);
    float frontDistance = readSensor(FRONT_SENSOR_PIN, FRONT_LED_PIN);
    // Serial.println(frontDistance);

    // if (rightDistance > 400) {
    //     turnMotor(left_servo, 50);
    //     turnMotor(right_servo, 0);
    // }
    // else if (frontDistance > 300) {
    //     turnMotor(left_servo, 0);
    //     turnMotor(right_servo, -50);
    // }
    // else if ((rightDistance > 200) && (rightDistance < 600)) {
    //     turnMotor(left_servo, 50);
    //     turnMotor(right_servo, -50);
    // }
    // else {
    //     turnMotor(left_servo, 50);
    //     turnMotor(right_servo, 0); 
    // }

    if (rightDistance < 100) {
        turn_90(RIGHT);
        step_forward();
    }
    else if (frontDistance < 200) {
        turnMotor(left_servo, 100);
        turnMotor(right_servo, -100);  
    }
    else if (leftDistance < 100) {
        turn_90(LEFT);
        step_forward();
    }
    else {
        turn_180();
        step_forward();
    }
}