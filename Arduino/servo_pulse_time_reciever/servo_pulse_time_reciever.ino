//this is a firmware simply there to be able to control 3 Servos with serial communication from your PC
const unsigned short int PULSE_LENGTH = 20000; //microseconds
const unsigned short int MIN_PULSE = 500;
const unsigned short int MAX_PULSE = 1800;
const unsigned short int REPEATIONS_OF_NEW_PULSE_TIME = 10;
const unsigned short int UPTIME_SHOOTING_POS = 1050;
const unsigned short int UPTIME_SHOOT_LOCK = 1200;
const unsigned short int INITIAL_PULSE_TIMES[2] = {1150, 1500}; 

unsigned short int pulseTimes[2];//in ms
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
    if (i <= 2) {
      pulseTimes[i] = INITIAL_PULSE_TIMES[i];
    }
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
    for (int pin = 0; pin < 2; pin++) {
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
//<char select {0-1}>;<int pulseTime {MIN_PULSE-MAX_PULSE}>; or: a, s

//used in the same way as acknowledgement

void printErr() {
  Serial.print("Malformed expression. Right protocol usage: ");
  Serial.print("<char select {0-1}>;<int pulseTime {");
  Serial.print(MIN_PULSE);
  Serial.print('-');
  Serial.print(MAX_PULSE);
  Serial.print("}> or: s, a");  
}

void pulseServo(int pin, int uptime, int cycles) {
  for (int cycle = 0; cycle < cycles; cycle++) {
    digitalWrite(pin, HIGH);
    delayMicroseconds(uptime);
    digitalWrite(pin, LOW);
    delayMicroseconds(PULSE_LENGTH - uptime);
  }
}

void serialEvent(){
  if (Serial.available()) {
    select = Serial.read();
    if (select == 's') {
      pulseServo(servoPins[2], UPTIME_SHOOTING_POS, 10);
      pulseServo(servoPins[2], UPTIME_SHOOT_LOCK, 200);
      Serial.println("shot");
    } else {
      select -= '0'; //convert char to number
      if (select == 0 || select == 1 && Serial.find(';')) {
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
          for (int i = 0; i < 2; i++) {
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
}

//micros() overflow after 70 min!!!
