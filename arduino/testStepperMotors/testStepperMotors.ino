// More information about stepper motor control can be found here: 
// https://learn.adafruit.com/adafruit-motor-shield-v2-for-arduino/using-stepper-motors

#include <Wire.h>
#include <Adafruit_MotorShield.h>

// ----- PARAMS TO TWEAK ----- //
const int _portBaud = 9600; // baud rate of the serial port
int _nbSteps = 200; // number of steps to perform
int _defaultNbStepsStepperMotor = 200; // number of steps / revolution of the stepper motors

// ----- INTERNAL PARAMS ----- //
// creates the motor shield object with the default I2C address 0x60
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); //write an adress if different
// connects a stepper motor with A and B steps per revolution
// to motor port #1 (M1 and M2)
Adafruit_StepperMotor *stepperMotorA = AFMS.getStepper(_defaultNbStepsStepperMotor, 1); // A = 200 steps/revolution
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *stepperMotorB = AFMS.getStepper(_defaultNbStepsStepperMotor, 2); // B = 200 steps/revolution


void setup() 
{
	// initializes the serial port
	Serial.begin(_portBaud);
	
	// initializes the connection with the motor shield 
	AFMS.begin();  
}

void loop() 
{
	// Motor A complete revolution FORWARD
  	Serial.println("Motor A --> Forward");
	for (int l_step=0; l_step < _nbSteps; l_step++)
		stepperMotorA->onestep(FORWARD,SINGLE); 
	delay(2000);
	
	// Motor B complete revolution BACKWARD
	Serial.println("Motor B --> Backward");
	for (int l_step=0; l_step < _nbSteps; l_step++)
		stepperMotorB->onestep(BACKWARD,SINGLE); 
	delay(2000);
	
	// Motor A complete revolution BACKWARD
	Serial.println("Motor A --> Backward");
	for (int l_step=0; l_step < _nbSteps; l_step++)
		stepperMotorA->onestep(BACKWARD,SINGLE); 
	delay(2000);
	
	// Motor B complete revolution FORWARD
	Serial.println("Motor B --> Forward");
	for (int l_step=0; l_step < _nbSteps; l_step++)
		stepperMotorB->onestep(FORWARD,SINGLE); 
	delay(2000);
	
}
