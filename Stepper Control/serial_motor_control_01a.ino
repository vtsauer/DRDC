/* Code was written by VTK Sauer, Wyvern */

/* It was adapted from: */
/* Simple Serial ECHO script : Written by ScottC 03/07/2012 */
/* https://arduinobasics.blogspot.com/2012/07/arduino-basics-simple-arduino-serial.html */

/* and had other help from: 
// https://www.arduino.cc/en/Tutorial.StringToIntExample
// Python control info can be found
// https://forum.arduino.cc/index.php?topic=396450
*/

/*Here's how it works. In the serial command window with "Newline" selected at the bottom type in a command and press enter.
 *The commands follow the following format: [s][1,2,3,4,a][f,b,d,r][s,d,i,m][X]. Each square braket represents a character.
 *At this point only the middle three characters switch the case structures below. Character 2 selects the stepper to use. Facing
 *the steppers and moving clockwise Stepper 1 is at the top and Stepper 2 is to its right, etc.. 'a' represents moving all the
 *steppers. In Character 3 'f' refers to forward and moves the secondary mirror toward the primary (increases cable tension), 
 *and 'b' refers to reverse moving the secondary away from the primary (decreases cable tension). 'r', and 'd' refer to retract 
 *and deploy and are only valid for the all steppers command 'a'. The last character is a placeholder for future potential work 
 *of moving the stepper X amount of steps.
*/

/*Things to manually change are:
 * deploysteps = 5; // The number of steps for deployment needs to be manually determined and set below
 */

#include <Wire.h>
#include <Adafruit_MotorShield.h>


/* Use a variable called byteRead to temporarily store
   the data coming from the computer */
byte byteRead;
const byte numChars = 6;
int receivedChars[numChars];   // an array to store the received data
int  inputvalue;
int count;
int counttotal=4;  //used for testing
int numofsteps=1; // each command will step once
int deploysteps=5; //number of steps for retracting/deploying secondary
byte stepdirection = FORWARD; //FORWARD or BACKWARD
byte steptype = DOUBLE; // SINGLE, DOUBLE. INTERLEAVE or MICROSTEP.
    /*"Single" means single-coil activation, "double" means 2 coils are activated at once (for higher torque) and "interleave" 
    means that it alternates between single and double to get twice the resolution (but of course its half the speed). 
    "Microstepping" is a method where the coils are PWM'd to create smooth motion between steps*/
int motorspeed=10; // 10 rpm


boolean newData = false;

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS_0 = Adafruit_MotorShield(); 
// Or, create it with a different I2C address (A0 bridged)
Adafruit_MotorShield AFMS_1 = Adafruit_MotorShield(0x61); 

// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #1 and #2 on each MotorShield
Adafruit_StepperMotor *myMotor_1 = AFMS_0.getStepper(200, 1);
Adafruit_StepperMotor *myMotor_2 = AFMS_1.getStepper(200, 1);
Adafruit_StepperMotor *myMotor_3 = AFMS_0.getStepper(200, 2);
Adafruit_StepperMotor *myMotor_4 = AFMS_1.getStepper(200, 2);


void setup() {                
// Turn the Serial Protocol ON
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(8, OUTPUT);  
  Serial.println("<Arduino is ready>");

  AFMS_0.begin();  // create with the default frequency 1.6KHz
  AFMS_1.begin();  // create with the default frequency 1.6KHz
  
  myMotor_1->setSpeed(motorspeed);  // 10 rpm
  myMotor_2->setSpeed(motorspeed);  // 10 rpm
  myMotor_3->setSpeed(motorspeed);  // 10 rpm
  myMotor_4->setSpeed(motorspeed);  // 10 rpm

  //energize stepper motors
  myMotor_1->step(1, FORWARD, DOUBLE);
  myMotor_2->step(1, FORWARD, DOUBLE);
  myMotor_3->step(1, FORWARD, DOUBLE);
  myMotor_4->step(1, FORWARD, DOUBLE);
  myMotor_1->step(1, BACKWARD, DOUBLE);
  myMotor_2->step(1, BACKWARD, DOUBLE);
  myMotor_3->step(1, BACKWARD, DOUBLE);
  myMotor_4->step(1, BACKWARD, DOUBLE);
}


//Run main code with selected functions
void loop() {
    recvWithEndMarker();  // receiving data with newline end of line marker '\n'
    //showNewData();      // testing code
    caseWithData();       // operating code
    //caseWithData_2();     // refined operating code
}


// receiving data function
void recvWithEndMarker() {
    static byte ndx = 0;
    char endMarker = '\n';
    char rc;
   
    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (rc != endMarker) {
            receivedChars[ndx] = rc;
            ndx++;
            if (ndx >= numChars) {
                ndx = numChars - 1;
            }
        }
        else {
            receivedChars[ndx] = '\0'; // terminate the string
            ndx = 0;
            newData = true;
        }
    }
}

// testing function
void showNewData() {
    if (newData == true) {
        Serial.print("This just in ... ");
        //Serial.println(receivedChars);
        Serial.println(receivedChars[0]);
        Serial.println(receivedChars[1]);
        Serial.println(receivedChars[2]);
        Serial.println(receivedChars[3]);
        //int n = atoi(receivedChars);
        //inputvalue = atoi(receivedChars);
        //Serial.println(n);
        newData = false;
    }
}


// Action code!
/*command format: [s][1,2,3,4,a][f,b,d,r][s,d,i,m][X]. Each square braket represents a character.
  At this point only the middle three characters switch the case structures below. 
  Character 2 selects the stepper to use. 
  Character 3 'f' refers to forward and moves the secondary mirror toward the primary (increases cable tension), 
  and 'b' refers to reverse moving the secondary away from the primary (decreases cable tension). 'r', and 'd' refer to retract 
  and deploy and are only valid for the all steppers command 'a'. 
  Character 4 sets the step type of the stepper motor (single, double, interleave, microstep)
  The last character is a placeholder for future potential work of moving the stepper X amount of steps.
*/
/*
void caseWithData_2() {
    if (newData == true) {
      newData = false;
      switch (receivedChars[3]) {
        case 115: // 's' input 
          steptype = SINGLE;
          break;

        case 100: // 'd' input
          steptype = DOUBLE;
          break;

        case 105: // 'i' input
          steptype = INTERLEAVE;
          break;

        case 109: // 'm' input
          steptype = MICROSTEP;
          break;
      }
    }
}
*/

void caseWithData() {
    if (newData == true) {
        newData = false;
        switch (receivedChars[3]) {
          case 115: // 's' input 
            steptype = SINGLE;
          break;

          case 100: // 'd' input
            steptype = DOUBLE;
          break;

          case 105: // 'i' input
            steptype = INTERLEAVE;
          break;

          case 109: // 'm' input
            steptype = MICROSTEP;
          break;
        }
        switch (receivedChars[1]) {
          case 49: // selection '1'; stepper 1
            //do something when var equals 49
            digitalWrite(8, HIGH);   // turn the green LED on (HIGH is the voltage level)
            digitalWrite(13, LOW);  // turn red LED to off
           
            if (receivedChars[2]==102) {
            // forward rotor motion, 102 is ASCII for 'f'
            Serial.println("Stepper 1 forward");
            stepdirection = FORWARD;
                for (int i=0; i<numofsteps; i++) {
                myMotor_1->step(1, stepdirection, steptype);
                }
               
            }
            else if (receivedChars[2]==98) {
            // reverse rotor motion, 98 is ASCII for 'b'
            Serial.println("Stepper 1 backward");
            stepdirection = BACKWARD;
                for (int i=0; i<numofsteps; i++) {
                myMotor_1->step(1, stepdirection, steptype);
                }
            }
            else {
            // no motion
            Serial.print("Not a valid command ");
            } 
          break;
            
          case 50: // selection '2'; stepper 2
            //do something when var equals 50
            digitalWrite(8, LOW);   // turn the LED on (HIGH is the voltage level)
            digitalWrite(13, HIGH);
            if (receivedChars[2]==102) {
            // forward rotor motion
            Serial.println("Stepper 2 forward");
            stepdirection = FORWARD;
                for (int i=0; i<numofsteps; i++) {
                myMotor_2->step(1, stepdirection, steptype);                
                }
               
            }
            else if (receivedChars[2]==98) {
            // reverse rotor motion
            Serial.println("Stepper 2 backward");
            stepdirection = BACKWARD;
                for (int i=0; i<numofsteps; i++) {
                myMotor_2->step(1, stepdirection, steptype);
                }
            }
            else {
            // no motion
            Serial.print("Not a valid command ");
            } 
            
          break;

          case 51: // selection '3'; stepper 3
            //do something when var equals 51
            digitalWrite(8, HIGH);   // turn the LED on (HIGH is the voltage level)
            digitalWrite(13, HIGH);
            if (receivedChars[2]==102) {
            // forward rotor motion
            Serial.println("Stepper 3 forward");
            stepdirection = FORWARD;
                for (int i=0; i<numofsteps; i++) {
                myMotor_3->step(1, stepdirection, steptype);
                }
               
            }
            else if (receivedChars[2]==98) {
            // reverse rotor motion
            Serial.println("Stepper 3 backward");
            stepdirection = BACKWARD;
                for (int i=0; i<numofsteps; i++) {
                myMotor_3->step(1, stepdirection, steptype);
                }
            }
            else {
            // no motion
            Serial.print("Not a valid command ");
            } 
          break;
            
          case 52: // selection '4'; stepper 4
            //do something when var equals 52
            digitalWrite(8, LOW);   // turn the LED on (HIGH is the voltage level)
            digitalWrite(13, LOW);
            if (receivedChars[2]==102) {
            // forward rotor motion
            Serial.println("Stepper 4 forward");
            stepdirection = FORWARD;
                for (int i=0; i<numofsteps; i++) {
                myMotor_4->step(1, stepdirection, steptype);
                }
               
            }
            else if (receivedChars[2]==98) {
            // reverse rotor motion
            Serial.println("Stepper 4 backward");
            stepdirection = BACKWARD;
                for (int i=0; i<numofsteps; i++) {
                myMotor_4->step(1, stepdirection, steptype);
                }
            }
            else {
            // no motion
            Serial.print("Not a valid command ");
            } 
          break;

          case 97: // selection 'a'; all the steppers
            //do something when var equals 97
            /*count = counttotal; 
            while (count >0) {
              // put your main code here, to run repeatedly:
              digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
              digitalWrite(8, HIGH);
              delay(1000);               // wait for 2 second
              digitalWrite(13, LOW);   // turn the LED on (HIGH is the voltage level)
              digitalWrite(8, LOW);
              delay(1000);               // wait for a 0.5 second
              count--;
            }
            */
            if (receivedChars[2]==102) {
            // forward rotor motion
            Serial.println("Stepper All forward and blinking!");
            stepdirection = FORWARD;
                for (int i=0; i<numofsteps; i++) {
                myMotor_1->step(1, stepdirection, steptype);
                myMotor_2->step(1, stepdirection, steptype);
                myMotor_3->step(1, stepdirection, steptype);
                myMotor_4->step(1, stepdirection, steptype);
                }
               
            }
            else if (receivedChars[2]==98) {
            // reverse rotor motion
            Serial.println("Stepper All backward and blinking!");
            stepdirection = BACKWARD;
                for (int i=0; i<numofsteps; i++) {
                myMotor_1->step(1, stepdirection, steptype);
                myMotor_2->step(1, stepdirection, steptype);
                myMotor_3->step(1, stepdirection, steptype);
                myMotor_4->step(1, stepdirection, steptype);
                }
            }
            else if (receivedChars[2]==100) {
            // Deploy secondary
            Serial.println("Secondary Deploy!");
            stepdirection = BACKWARD;
            numofsteps=deploysteps;
                for (int i=0; i<numofsteps; i++) {
                myMotor_1->step(1, stepdirection, steptype);
                myMotor_2->step(1, stepdirection, steptype);
                myMotor_3->step(1, stepdirection, steptype);
                myMotor_4->step(1, stepdirection, steptype);
                }
            }
            else if (receivedChars[2]==114) {
            // Retract secondary
            Serial.println("Secondary Retract!");
            stepdirection = FORWARD;
            numofsteps=deploysteps;
                for (int i=0; i<numofsteps; i++) {
                myMotor_1->step(1, stepdirection, steptype);
                myMotor_2->step(1, stepdirection, steptype);
                myMotor_3->step(1, stepdirection, steptype);
                myMotor_4->step(1, stepdirection, steptype);
                }
            }
            else {
            // no motion
            Serial.print("Not a valid command ");
            } 
          break;
       // newData = false;
    }
    }
}
