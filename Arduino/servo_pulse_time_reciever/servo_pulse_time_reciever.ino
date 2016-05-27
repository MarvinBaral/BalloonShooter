//this is a firmware simply there to be able to control 3 Servos with serial communication from your PC
const unsigned short int PULSE_LENGTH = 20000; //microseconds
const unsigned short int MIN_PULSE = 500;
const unsigned short int MAX_PULSE = 1800;
const unsigned short int REPEATIONS_OF_NEW_PULSE_TIME = 10;

int pulseTimes[3] = {1150, 1500, 1200}; //in ms
int pulseTime;
int servoPins[3] = {9, 10, 11}; //also usable as PWM
char select;
unsigned long int startMicros;
unsigned long int timeInPulse;
unsigned short int ctr = 0;

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 3; i++) {
    pinMode(servoPins[i], OUTPUT);
  }
}


/*Pulses done by main loop (there are three of them in parallel):

pulseTimes[0]
|      /
|     |

+-----+                         
|     |                         
|     |                         
+     +--------------------------

      +-----+                      
      |     |                      
      |     |                      
------+     +--------------------

            +-----+              
            |     |              
            |     |              
------------+     +--------------

|                               |
|                                \
time defined in PULSE_LENGTH (20ms)

|
|
time = 0

*/

void loop() {
  if (ctr < REPEATIONS_OF_NEW_PULSE_TIME) {
    startMicros = micros();
    for (int pin = 0; pin < 3; pin++) {
      digitalWrite(servoPins[pin], HIGH);
      delayMicroseconds(pulseTimes[pin]);
      digitalWrite(servoPins[pin], LOW);
    }
    do {
      timeInPulse = micros() - startMicros;
    }
    while (timeInPulse < PULSE_LENGTH);
    ctr++;
  }
}

//The protocoll
//=============
//<char select {0-2}>;<int pulseTime {MIN_PULSE-MAX_PULSE}>;

//used in the same way as acknowledgement

void printErr() {
  Serial.print("Malformed expression. Right protocol usage: ");
  Serial.print("<char select {0-2}>;<int pulseTime {");
  Serial.print(MIN_PULSE);
  Serial.print('-');
  Serial.print(MAX_PULSE);
  Serial.print("}>");  
}

void serialEvent(){
  if (Serial.available()) {
    select = Serial.read();
    select -= '0'; //convert char to number
    if (select >= 0 && select <= 2 && Serial.find(';')) {
      pulseTime = Serial.parseInt();
      if (pulseTime >= MIN_PULSE && pulseTime <= MAX_PULSE && Serial.find(';')) {
        pulseTimes[select] = pulseTime;
        ctr = 0;
        Serial.print(char(select + '0')); //!!! modified char
        Serial.print(';');
        Serial.print(pulseTimes[select]);
        Serial.println(';');
      } else {
        printErr();
      }
    } else {
      if (select == ('a' - '0')) { //!!! modified char
        for (int i = 0; i < 3; i++) {
          Serial.print(i);
          Serial.print(": ");
          Serial.println(pulseTimes[i]);
        }
      } else {
        printErr();
      }
    }
  }
}

//micros() overflow after 70 min!!!
