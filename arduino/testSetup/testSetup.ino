#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <Servo.h>

// ----- PARAMS TO TWEAK ----- //
// verbose
bool _verboseMode = true;
// baud rate of the serial port
const long int _portBaud = 115200; 
// fixed point positions in mm
const float _xA = -675.0;
const float _yA = 520.0;
const float _xB = 675.0;
const float _yB = 520.0;
// gear radii in mm
const float _gearARadius = 16.45;//16.82;
const float _gearBRadius = 16.45; //16.94;
// motor increment factor (# of ° per increment)
const float _motorAIncrementCalibrationFactor = 1.8;
const float _motorBIncrementCalibrationFactor = 1.8;
// pin (PWM) where servo motor is connected to
const int _servoPinPWM = 9;
// ratio handling 
const int _ratioFlag = 2; // =0 if ratio not taken into account, =1 if it is, =2 if rest of steps taken into account


// ----- INTERNAL PARAMS ----- //
float _ACp = 0.0; // previous value of AC to compute delta
float _BCp = 0.0; // previous value of BC to compute delta
String _inputString = "";         // a String to hold incoming data
bool _stringComplete = false;  // whether the string is complete
bool _startProcessingInputs = false;
// initializes internal variables
float _xCoord  = 0.0;
float _yCoord  = 0.0;
int _servoFlag = 0;
float _motorAIncrement = 0.0;
float _motorBIncrement = 0.0;
float _ratioMotorAB = 1.0;
float _restMotorAB = 0.0;
int _compensationIncrementMotorAB = 0;
bool _motorAIncrementGreater = false;
int _motorAIncrementAbsInt = 0;
int _motorBIncrementAbsInt = 0;
// creates a servo object to control a servo motor
Servo _servoMotor;
// creates the motor shield object with the default I2C address 0x60
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); //write an adress if different
// connects a stepper motor with A and B steps per revolution
// to motor port #1 (M1 and M2)
Adafruit_StepperMotor *stepperMotorA = AFMS.getStepper(360/_motorAIncrementCalibrationFactor, 1); // A = 200 steps/revolution
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *stepperMotorB = AFMS.getStepper(360/_motorAIncrementCalibrationFactor, 2); // B = 100 steps/revolution


void setup() 
{
	// initializes the serial port
	Serial.begin(_portBaud);

	// initializes the connection with the motor shield 
	AFMS.begin();  
	
	// attach servo motor to PWM pin
	_servoMotor.attach(_servoPinPWM); 

	// moves the target point to the center of the board
	_ACp = targetCoord2ChainLength(0.0, 0.0, _xA, _yA);
	_BCp = targetCoord2ChainLength(0.0, 0.0, _xB, _yB);
}

void loop() 
{
	// a new command has been received
	if (_stringComplete) 
	{
		// checks if it is a start command
		if (_inputString.equals("Start\n"))
		{
			_startProcessingInputs = true;
			if (_verboseMode)
				Serial.println("Sensor:|11.1|22.2|"); 
			// clears the string:
			_inputString = "";
			_stringComplete = false;
			return;
		}
      
		// checks if it is a pause command
		if (_inputString.equals("Pause\n"))
		{
			if (_verboseMode)
				Serial.println("Sensor:|88.8|99.9|"); 
			// clears the string:
			_inputString = "";
			_stringComplete = false;
			return;
		}

		// checks if it is a stop command
		if (_inputString.equals("Stop\n"))
		{
			_startProcessingInputs = false;
			//Serial.print("The program will stop processing inputs");
			if (_verboseMode)
				Serial.println("Sensor:|00.0|00.0|");
			// clears the string:
			_inputString = "";
			_stringComplete = false;
			// moves the drawing head to the center of the board
			_xCoord = 0.0;
			_yCoord = 0.0;
			_servoFlag = 0;
			computeMotorIncrement();
			estimateMotorIncrementRatio();
			sendMotorCommands();
				      
			return;
		}

		// a new set of target points has been received
		if (_startProcessingInputs)
		{
			// retrieves the new target point
			retrieveTargetPosition();

			// clears the string:
			_inputString = "";
			_stringComplete = false;
      
			// computes motor increment
			computeMotorIncrement();
			
			// estimates the ratio between the A and B motors
			estimateMotorIncrementRatio();
			
			// sends the command to the motors
			sendMotorCommands();
		}
	}
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() 
{
	while (Serial.available()) 
	{
		// gets the new byte:
		char inChar = (char)Serial.read();
		// adds it to the _inputString:
		_inputString += inChar;
		// if the incoming character is a newline, set a flag so the main loop can
		// does something about it:
		if (inChar == '\n') 
		{
			_stringComplete = true;
		}
	}
}

float targetCoord2ChainLength(float targetX, float targetY, float fixedX, float fixedY)
{
	float distance = sqrt(targetX*targetX + targetY*targetY + fixedY*fixedY + fixedX*fixedX -2*fixedX*targetX - 2*fixedY*targetY);
	return distance;
}

float chainLengthDelta2MotorIncrement(float distanceDelta, float gearRadius, float motorIncrementFactor)
{
	float angle = distanceDelta / gearRadius * 180.0 / PI;
	float motorIncrement = angle / motorIncrementFactor;
	return motorIncrement;
}

void retrieveTargetPosition()
{
	int headerEndIndex = _inputString.indexOf(':');
	int semiColonIndex = _inputString.indexOf(';');
	int comaIndex = _inputString.indexOf(',');
	int eolIndex = _inputString.indexOf('\n');
	_xCoord = (_inputString.substring(headerEndIndex+1, semiColonIndex)).toFloat();
	_yCoord = (_inputString.substring(semiColonIndex+1, comaIndex)).toFloat();
	_servoFlag = (_inputString.substring(comaIndex+1, eolIndex)).toInt();

	if (_verboseMode)
	{
		Serial.print("Sensor:|"); Serial.print(_xCoord);
		Serial.print("|"); Serial.print(_yCoord);Serial.print("|");Serial.print(_servoFlag);
	}
}

void computeMotorIncrement()
{
	// estimates the distance of the segment AC and BC
	float AClength = targetCoord2ChainLength(_xCoord, _yCoord, _xA, _yA);
	float BClength = targetCoord2ChainLength(_xCoord, _yCoord, _xB, _yB);
  
	if (_verboseMode)
	{
		Serial.print(AClength);
		Serial.print("|"); Serial.print(BClength);Serial.print("|");
	}

	// computes the number of increment for each motor
	float distanceACDelta = AClength - _ACp;
	float distanceBCDelta = BClength - _BCp;

	if (_verboseMode)
	{
		Serial.print(distanceACDelta);
		Serial.print("|"); Serial.print(distanceBCDelta);Serial.print("|");
	}

	_motorAIncrement = chainLengthDelta2MotorIncrement(distanceACDelta, _gearARadius, _motorAIncrementCalibrationFactor);
	_motorBIncrement = chainLengthDelta2MotorIncrement(distanceBCDelta, _gearBRadius, _motorBIncrementCalibrationFactor);

	if (_verboseMode)
	{
		Serial.print(_motorAIncrement);
		Serial.print("|"); Serial.print(_motorBIncrement);Serial.print("|");
	}
	
	// saves the new AC and BC values
	_ACp = AClength;
	_BCp = BClength;
}

void estimateMotorIncrementRatio()
{
	// estimates the ratio between the A and B motors
	float motorAIncrementAbs = abs(_motorAIncrement);
	float motorBIncrementAbs =  abs(_motorBIncrement);

	if (motorAIncrementAbs == motorBIncrementAbs) 
	{
		_ratioMotorAB = 1;
	}
	else
	{
		if(motorAIncrementAbs > motorBIncrementAbs) 
		{ 
			if (motorBIncrementAbs != 0)
			{
				_ratioMotorAB = motorAIncrementAbs/motorBIncrementAbs;
				_restMotorAB = motorAIncrementAbs - floor(_ratioMotorAB) * motorBIncrementAbs;
				_compensationIncrementMotorAB = motorBIncrementAbs / _restMotorAB;
			}
			else
			{
				_ratioMotorAB = 1;
			}
			
			_motorAIncrementGreater = true; 
		}
		else 
		{  
			if (motorAIncrementAbs != 0)
			{
				_ratioMotorAB = motorBIncrementAbs/motorAIncrementAbs;
				_restMotorAB = motorBIncrementAbs - floor(_ratioMotorAB) * motorAIncrementAbs;
				_compensationIncrementMotorAB = motorAIncrementAbs / _restMotorAB;
			}
			else
			{
				_ratioMotorAB = 1;
			}
			
			_motorAIncrementGreater = false;  
		}
	}

	_motorAIncrementAbsInt = floor(motorAIncrementAbs);
	_motorBIncrementAbsInt = floor(motorBIncrementAbs);
  
	if (_verboseMode)
	{
		Serial.print(_ratioMotorAB);
		Serial.print("|");
		Serial.print(floor(_ratioMotorAB));
		Serial.print("|");Serial.print(_motorAIncrementGreater);
		Serial.print("|");Serial.print(_restMotorAB);
		Serial.print("|");Serial.print(_compensationIncrementMotorAB);
		Serial.print("|");Serial.print(_motorAIncrementAbsInt);
		Serial.print("|");Serial.print(_motorBIncrementAbsInt);
		Serial.println("|");
	}

}

void sendMotorCommandsToMotorA()
{
	if (_motorAIncrementAbsInt > 0)
	{
		_motorAIncrementAbsInt--;
		if (_motorAIncrement < 0)
			stepperMotorA->onestep(BACKWARD,SINGLE); 
		else
			stepperMotorA->onestep(FORWARD,SINGLE);
	} 
}

void sendMotorCommandsToMotorB()
{
	if (_motorBIncrementAbsInt > 0)
	{
		_motorBIncrementAbsInt--;
		if (_motorBIncrement < 0)
			stepperMotorB->onestep(BACKWARD,SINGLE);
		else
			stepperMotorB->onestep(FORWARD,SINGLE);
	}
}

void sendMotorCommands()
{
	bool goOn = true;
	int count = 0;
  
	if (_servoFlag==1)
		_servoMotor.write(0);
	else
		_servoMotor.write(90);
    
	while (goOn)
	{
		if ((_motorBIncrementAbsInt == 0) && (_motorAIncrementAbsInt == 0))
			goOn = false;

		if (_ratioFlag == 0)
		{
			sendMotorCommandsToMotorA(); // one motor step for A
			sendMotorCommandsToMotorB(); // one motor step for B
		}
		else if (_ratioFlag == 1 || _ratioFlag == 2)
		{
			if (_ratioMotorAB == 1)
			{
				sendMotorCommandsToMotorA(); // one motor step for A
				sendMotorCommandsToMotorB(); // one motor step for B
			}
			else
			{
				if (_motorAIncrementGreater)
				{
					// several step for motor A
					for (int l_increment = 0; l_increment < floor(_ratioMotorAB); l_increment++)
					{
						sendMotorCommandsToMotorA();
					}
					// one motor step for B
					sendMotorCommandsToMotorB();

					if (_ratioFlag == 2 && count % _compensationIncrementMotorAB == 0)
						sendMotorCommandsToMotorA();
				}
				else
				{
					// several step for motor B
					for (int l_increment = 0; l_increment < floor(_ratioMotorAB); l_increment++)
					{
						sendMotorCommandsToMotorB();
					}
					// one motor step for A
					sendMotorCommandsToMotorA();

					if (_ratioFlag == 2 && count % _compensationIncrementMotorAB == 0)
						sendMotorCommandsToMotorB();
				}
			count++;
			}
   		}
	}

}
