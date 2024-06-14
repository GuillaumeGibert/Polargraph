// More information about servo motor control can be found here: 
// https://learn.adafruit.com/adafruit-motor-shield-v2-for-arduino/using-rc-servos

#include <Servo.h>

// ----- PARAMS TO TWEAK ----- //
const int _portBaud = 9600; // baud rate of the serial port
const int _servoPinPWM = 9; // pin (PWM) where servo motor is connected to (either 9 or 10 on this shield)

// ----- INTERNAL PARAMS ----- //
// creates a servo object to control a servo motor
Servo servoMotor;

void setup() 
{
	// initializes the serial port
	Serial.begin(_portBaud);

	// attach the servo motor to the PWM pin
	servoMotor.attach(_servoPinPWM); 
}


void loop() 
{
	Serial.println("Servo motor --> [0°]");
	servoMotor.write(0); 
	delay(1000); // wait for 1000 ms
	
	
	Serial.println("Servo motor --> [90°]");
	servoMotor.write(90); 
	delay(1000); // wait for 1000 ms
}
