#include <CMRI.h>
// CMRI Settings
#define CMRI_ADDR 2 //CMRI node address in JMRI

// Define CMRI connection with 24 inputs and 24 outputs
CMRI cmri(CMRI_ADDR, 24, 24);

#include <VarSpeedServo.h>  //servo library that enables servo speed control and operation without delaying the main loop
#define numSensitivity 800

// create servo object to control a servo
VarSpeedServo myservo1;
VarSpeedServo myservo2;
VarSpeedServo myservo3;
VarSpeedServo myservo4;

// create constants to read Light Dependant Resistor measurement by analogue ports

int const ldrPin1 = A0; // analog pin used to connect to LDR sensor1
int const ldrPin2 = A1; // analog pin used to connect to LDR sensor2
int const ldrPin3 = A2; // analog pin used to connect to LDR sensor3
int const ldrPin4 = A3; // analog pin used to connect to LDR sensor4
int const ldrPin5 = A4; // analog pin used to connect to LDR sensor5
int const ldrPin6 = A5; // analog pin used to connect to LDR sensor6
int const ldrPin7 = A6; // analog pin used to connect to LDR sensor7
int const ldrPin8 = A7; // analog pin used to connect to LDR sensor8

int RED1 = 2;
int GREEN1 = 4;
int RED2 = 8;
int GREEN2 = 10;
int POWER = 12;


// Variables

long previousMillis1 = 0;
long previousMillis2 = 0;
long previousMillis3 = 0;
long previousMillis4 = 0;
long previousMillis5 = 0;

long interval = 3500;  // interval at which to change signal lights and half interval to close 2nd gates to traffic


int ldrVal1;  // variable to read the value from the analog pin A0
int ldrVal2;  // variable to read the value from the analog pin A1
int ldrVal3;  // variable to read the value from the analog pin A2
int ldrVal4;  // variable to read the value from the analog pin A3
int ldrVal5;  // variable to read the value from the analog pin A4
int ldrVal6;  // variable to read the value from the analog pin A5
int ldrVal7;  // variable to read the value from the analog pin A6
int ldrVal8;  // variable to read the value from the analog pin A7

int ldrVal1a;  // variable to read the value from the analog pin A0
int ldrVal2a;  // variable to read the value from the analog pin A1
int ldrVal3a;  // variable to read the value from the analog pin A2
int ldrVal4a;  // variable to read the value from the analog pin A3
int ldrVal5a;  // variable to read the value from the analog pin A4
int ldrVal6a;  // variable to read the value from the analog pin A5
int ldrVal7a;  // variable to read the value from the analog pin A6
int ldrVal8a;  // variable to read the value from the analog pin A7

int ldrVal1i;  // variable to store the initial value from the analog pin A0
int ldrVal2i;  // variable to store the initial value from the analog pin A1
int ldrVal3i;  // variable to store the initial value from the analog pin A2
int ldrVal4i;  // variable to store the initial value from the analog pin A3
int ldrVal5i;  // variable to store the initial vaule from the analog pin A4
int ldrVal6i;  // variable to store the initial value from the analog pin A5
int ldrVal7i;  // variable to store the initial vaule from the analog pin A6
int ldrVal8i;  // variable to store the initial value from the analog pin A7

bool lineinnerclear;
bool lineouterclear;
bool outer2;
bool outer3;
bool outer4;
bool outerstop;
bool inner4;
bool inner5;
bool inner6;
bool innerstop;


int LEF = 145;
int RIG = 45;

int SPEED1 = 25;
int SPEED2 = 25;



void setup() {

  // initialize serial port:
  // Start the serial connection to JMRI computer
  Serial.begin(19200);  //Baud rate of 19200 usually used, so ensure this matches the baud rate in JMRI,
  // using a faster rate can make processing faster but can also result in incomplete data

  pinMode(RED1, OUTPUT);
  pinMode(RED2, OUTPUT);
  pinMode(GREEN1, OUTPUT);
  pinMode(GREEN2, OUTPUT);
  pinMode(POWER, OUTPUT);
  digitalWrite(POWER, HIGH);
  digitalWrite(GREEN1, HIGH);
  digitalWrite(RED1, LOW);
  digitalWrite(GREEN2, HIGH);
  digitalWrite(RED2, LOW);
  // initialise servo ports:
  myservo1.attach(3);  // connect servo 1 to pin 3
  myservo2.attach(5);  // connect servo 2 to pin 5
  myservo3.attach(6);  // connect servo 3 to pin 6
  myservo4.attach(9);  // connect servo 4 to pin 9

  // take initial readings for LDR's
  ldrVal1i = analogRead(ldrPin1); // read the value of LDR Sensor 1
  ldrVal2i = analogRead(ldrPin2); // read the value of LDR Sensor 2
  ldrVal3i = analogRead(ldrPin3); // read the value of LDR Sensor 3
  ldrVal4i = analogRead(ldrPin4); // read the value of LDR Sensor 4
  ldrVal5i = analogRead(ldrPin5); // read the value of LDR Sensor 5
  ldrVal6i = analogRead(ldrPin6); // read the value of LDR Sensor 6
  ldrVal7i = analogRead(ldrPin7); // read the value of LDR Sensor 7
  ldrVal8i = analogRead(ldrPin8); // read the value of LDR Sensor 8

  outer2 = false;
  outer3 = false;
  outer4 = false;
  outerstop = false;
  inner4 = false;
  inner5 = false;
  inner6 = false;
  innerstop = false;


  myservo1.write(LEF, SPEED1);
  myservo2.write(RIG, SPEED2);
  myservo3.write(LEF, SPEED2);
  myservo4.write(RIG, SPEED2);
  myservo1.wait();
  myservo2.wait();

  lineinnerclear = true;
  lineouterclear = true;

  cmri.set_bit(0, outer2);  //Bit 0 = address 2001 in JMRI as this is set to CMRI Node 2
  cmri.set_bit(1, outer3);
  cmri.set_bit(2, outer4);
  cmri.set_bit(3, outerstop);
  cmri.set_bit(4, inner4);
  cmri.set_bit(5, inner5);
  cmri.set_bit(6, inner6);
  cmri.set_bit(7, innerstop);
  cmri.process();
}

// Setup states for inner and outerlines

enum LINESTATEinner
{
  inner_ENTER,
  inner_STOP,
  inner_SIGNAL,
  inner_EXIT,
};
enum LINESTATEouter
{
  outer_ENTER,
  outer_STOP,
  outer_SIGNAL,
  outer_EXIT,
};
//  Initial states for inner and outer lines

LINESTATEinner lineinnerState = inner_ENTER;
LINESTATEouter lineouterState = outer_ENTER;

// Main Loop

void loop() {
  //  cmri.process();   // only process when something changes

  int LEF = 145;
  int RIG = 45;

  int SPEED1 = 15;
  int SPEED2 = 15;
  ldrVal1 = analogRead(ldrPin1); // read the value of LDR Sensor 1
  ldrVal2 = analogRead(ldrPin2); // read the value of LDR Sensor 2
  ldrVal3 = analogRead(ldrPin3); // read the value of LDR Sensor 3
  ldrVal4 = analogRead(ldrPin4); // read the value of LDR Sensor 4
  ldrVal5 = analogRead(ldrPin5); // read the value of LDR Sensor 5
  ldrVal6 = analogRead(ldrPin6); // read the value of LDR Sensor 6
  ldrVal7 = analogRead(ldrPin7); // read the value of LDR Sensor 7
  ldrVal8 = analogRead(ldrPin8); // read the value of LDR Sensor 8

  ldrVal1a = analogRead(ldrPin1); // read the value of LDR Sensor 1
  ldrVal2a = analogRead(ldrPin2); // read the value of LDR Sensor 2
  ldrVal3a = analogRead(ldrPin3); // read the value of LDR Sensor 3
  ldrVal4a = analogRead(ldrPin4); // read the value of LDR Sensor 4
  ldrVal5a = analogRead(ldrPin5); // read the value of LDR Sensor 5
  ldrVal6a = analogRead(ldrPin6); // read the value of LDR Sensor 6
  ldrVal7a = analogRead(ldrPin7); // read the value of LDR Sensor 7
  ldrVal8a = analogRead(ldrPin8); // read the value of LDR Sensor 8

  ldrVal1 = ldrVal1 + ldrVal1a;
  ldrVal2 = ldrVal2 + ldrVal2a;
  ldrVal3 = ldrVal3 + ldrVal3a;
  ldrVal4 = ldrVal4 + ldrVal4a;
  ldrVal5 = ldrVal5 + ldrVal5a;
  ldrVal6 = ldrVal6 + ldrVal6a;
  ldrVal7 = ldrVal7 + ldrVal7a;
  ldrVal8 = ldrVal8 + ldrVal8a;


  cmri.set_bit(0, outer2);  //Bit 0 = address 2001 in JMRI
  cmri.set_bit(1, outer3);
  cmri.set_bit(2, outer4);
  cmri.set_bit(3, outerstop);
  cmri.set_bit(4, inner4);
  cmri.set_bit(5, inner5);
  cmri.set_bit(6, inner6);
  cmri.set_bit(7, innerstop);
  cmri.process();


  // print out the value to the Serial Monitor whilst debugging only
  //        Serial.print("ldrVal8: ");
  //        Serial.print(ldrVal8);
  //        Serial.print("   ldrVal7: ");
  //        Serial.print(ldrVal7);
  //        Serial.print("  ldrVal6: ");
  //        Serial.print(ldrVal6);
  //        Serial.print("   ldrVal5: ");
  //        Serial.print(ldrVal5);
  //        Serial.print("  ldrVal4: ");
  //        Serial.print(ldrVal4);
  //        Serial.print("   ldrVal3: ");
  //        Serial.print(ldrVal3);
  //        Serial.print("   ldrVal2: ");
  //        Serial.print(ldrVal2);
  //        Serial.print("   ldrVal1: ");
  //        Serial.print(ldrVal1);
  //        Serial.print("  innerline: ");
  //        Serial.print(lineinnerState);
  //        Serial.print("  ");
  //        Serial.print(lineinnerclear);
  //        Serial.print("   outerline: ");
  //        Serial.print(lineouterState);
  //        Serial.print("  ");
  //        Serial.println(lineouterclear);

  switch (lineinnerState)  // inner track
  {
    case inner_ENTER:
      lineinnerenter(ldrVal1, ldrVal2, ldrVal3, ldrVal8);
      break;
    case inner_STOP:
      lineinnerstop(ldrVal1, ldrVal2, ldrVal3, ldrVal8);
      break;
    case inner_SIGNAL:
      lineinnersignal(ldrVal1, ldrVal2, ldrVal3, ldrVal8);
      break;
    case inner_EXIT:
      lineinnerexit(ldrVal1, ldrVal2, ldrVal3, ldrVal8);
      break;
  }
  switch (lineouterState)  // outer track
  {
    case outer_ENTER:
      lineouterenter(ldrVal4, ldrVal5, ldrVal6, ldrVal7);
      break;
    case outer_STOP:
      lineouterstop(ldrVal4, ldrVal5, ldrVal6, ldrVal7);
      break;
    case outer_SIGNAL:
      lineoutersignal(ldrVal4, ldrVal5, ldrVal6, ldrVal7);
      break;
    case outer_EXIT:
      lineouterexit(ldrVal4, ldrVal5, ldrVal6, ldrVal7);
      break;
  }
}
// End of main loop

// What to do with the results for inner and outer lines

void lineinnerexit(int ldrVal1, int ldrVal2, int ldrVal3, int ldrVal8) {
  if (ldrVal3 > (ldrVal3i + numSensitivity)) {
    lineinnerclear = true;
    inner6 = true;
    inner5 = false;
    cmri.set_bit(5, inner5);
    cmri.set_bit(6, inner6);

    if (lineinnerclear and lineouterclear) {
      //      Serial.println(" open gates to traffic ");
      myservo1.write(LEF, SPEED1);
      myservo2.write(RIG, SPEED2);
      myservo3.write(LEF, SPEED2);
      myservo4.write(RIG, SPEED2);
    }
    lineinnerState = inner_ENTER;
  }
}
void lineinnerenter(int ldrVal1, int ldrVal2, int ldrVal3, int ldrVal8) {
  if (ldrVal1 > (ldrVal1i + numSensitivity)) {
    //    Serial.println(" closing gates to traffic ");
    lineinnerclear = false;
    inner4 = true;
    inner6 = false;
    cmri.set_bit(4, inner4);
    cmri.set_bit(6, inner6);

    myservo1.write(RIG, SPEED1);
    myservo3.write(RIG, SPEED2);

    lineinnerState = inner_STOP;
    previousMillis1 = millis();

  }
}
void lineinnerstop(int ldrVal1, int ldrVal2, int ldrVal3, int ldrVal8) {
  unsigned long currentMillis1 = millis();
  if (currentMillis1 - previousMillis1 > (interval / 2)) {
    myservo2.write(LEF, SPEED1);
    myservo4.write(LEF, SPEED2);
  }
  if (currentMillis1 - previousMillis1 > interval) {
    previousMillis1 = currentMillis1;
    digitalWrite(GREEN1, LOW);
    digitalWrite(RED1, HIGH); // turn signal from red to green
  }
  if (ldrVal8 > (ldrVal8i + numSensitivity)) {  // should be ldrVal8

    inner4 = false;
    innerstop = true;
    cmri.set_bit(4, inner4);
    cmri.set_bit(7, innerstop);

    lineinnerState = inner_SIGNAL;
    previousMillis1 = millis();
  }
}

void lineinnersignal(int ldrVal1, int ldrVal2, int ldrVal3, int ldrVal8) {

  if (ldrVal2 > (ldrVal2i + numSensitivity)) {
    lineinnerState = inner_EXIT;
    inner5 = true;
    innerstop = false;
    cmri.set_bit(5, inner5);
    cmri.set_bit(7, innerstop);
    digitalWrite(GREEN1, HIGH);
    digitalWrite(RED1, LOW); // turn signal red
  }
}

// outer line commands
void lineouterexit(int ldrVal4, int ldrVal5, int ldrVal6, int ldrVal7) {
  if (ldrVal6 > (ldrVal6i + numSensitivity)) {
    lineouterclear = true;
    outer4 = true;
    outer3 = false;
    cmri.set_bit(2, outer4);
    cmri.set_bit(1, outer3);

    if (lineinnerclear and lineouterclear) {
      //      Serial.println(" open gates to traffic ");
      myservo1.write(LEF, SPEED1);
      myservo2.write(RIG, SPEED2);
      myservo3.write(LEF, SPEED2);
      myservo4.write(RIG, SPEED2);
    }
    lineouterState = outer_ENTER;
  }
}
void lineouterenter(int ldrVal4, int ldrVal5, int ldrVal6, int ldrVal7) {
  if (ldrVal4 > (ldrVal4i + numSensitivity)) {
    //    Serial.println(" closing gates to traffic ");
    lineouterclear = false;
    outer4 = false;
    outer2 = true;
    cmri.set_bit(0, outer2);  //Bit 0 = address 2001 in JMRI
    cmri.set_bit(2, outer4);

    myservo1.write(RIG, SPEED1);
    myservo3.write(RIG, SPEED2);

    lineouterState = outer_STOP;
    previousMillis2 = millis();
  }
}

void lineouterstop(int ldrVal4, int ldrVal5, int ldrVal6, int ldrVal7) {
  unsigned long currentMillis2 = millis();
  if (currentMillis2 - previousMillis2 > (interval / 2)) { // start other gates moving
    myservo2.write(LEF, SPEED1);
    myservo4.write(LEF, SPEED2);
  }
  if (currentMillis2 - previousMillis2 > interval) {
    previousMillis2 = currentMillis2;
    digitalWrite(GREEN2, LOW);
    digitalWrite(RED2, HIGH); // turn signal from red to green
  }

  if (ldrVal7 > (ldrVal7i + numSensitivity)) {
    lineouterState = outer_SIGNAL;
    outerstop = true;
    outer2 = false;
    cmri.set_bit(3, outerstop);
    cmri.set_bit(0, outer2);
  }
}
void lineoutersignal(int ldrVal4, int ldrVal5, int ldrVal6, int ldrVal7) {

  if (ldrVal5 > (ldrVal5i + numSensitivity)) {
    lineouterState = outer_EXIT;
    outer3 = true;
    outerstop = false;
    cmri.set_bit(3, outerstop);
    cmri.set_bit(1, outer3);

    digitalWrite(GREEN2, HIGH);
    digitalWrite(RED2, LOW); // turn signal red
  }
}
