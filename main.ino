//Pin numbers
//Bluetooth is connected to standard RX and TX
#define m1 2
#define m2 3
#define led2 13
#define led1 12
#define led0 8
#define usecho 4
#define ustrig 5
#define buzzer 6
#define light0 9
#define light1 10
#define light2 11
char i, a;
float t=9.0f; //Time taken to move 1 floor, calibrate experimentally

void setup()
{
  pinMode(m1, OUTPUT);
  pinMode(m2, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led0, OUTPUT);
  pinMode(ustrig, OUTPUT);
  pinMode(usecho, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(light0, OUTPUT);
  pinMode(light1, OUTPUT);
  pinMode(light2, OUTPUT);
  Serial.begin(9600);
  a='0';
  digitalWrite(led0, HIGH);
  digitalWrite(light0, HIGH);
}

void loop()
{ 
  //Alarm
  if (checkdistance() && (a=='0' || a=='1')) {
    digitalWrite(buzzer, HIGH);
  } else {
    digitalWrite(buzzer, LOW);
  }
  //Lift controller
  if(Serial.available())
  {
      i= Serial.read();
      if (i=='0') {
        if (a=='2') {
          Serial.println("Moving to 0");
          backward(t*2);
        }
        else if (a=='1') {
          Serial.println("Moving to 0");
          backward(t);
        }
        else if (a=='0') {
          Serial.println("Already on 0");
        }
        digitalWrite(led2, LOW);
        digitalWrite(led1, LOW);
        digitalWrite(led0, HIGH);
        a = '0';
      }
      else if (i=='1') {
        if (a=='2') {
          Serial.println("Moving to 1");
          backward(t);
        }
        else if (a=='1') {
          Serial.println("Already on 1");
        }
        else if (a=='0') {
          Serial.println("Moving to 1");
          forward(t);
        }
        digitalWrite(led2, LOW);
        digitalWrite(led1, HIGH);
        digitalWrite(led0, LOW);
        a = '1';
      }
      else if (i=='2') {
        if (a=='2') {
          Serial.println("Already on 2");
        }
        else if (a=='1') {
          Serial.println("Moving to 2");
          forward(t);
        }
        else if (a=='0') {
          Serial.println("Moving to 2");
          forward(2*t);
        }
        digitalWrite(led2, HIGH);
        digitalWrite(led1, LOW);
        digitalWrite(led0, LOW);
        a = '2';
      }
  }
  //Timed LEDs
  if (millis() % 180000 < 90000) {
    digitalWrite(light0, HIGH);
  } else {
    digitalWrite(light0, LOW);
  }
  if (millis() % 55000 < 35000) {
    digitalWrite(light1, HIGH);
  } else {
    digitalWrite(light1, LOW);
  }
  if (millis() % 75000 < 40000) {
    digitalWrite(light2, LOW);
  } else {
    digitalWrite(light2, HIGH);
  }
}

void forward(int t) {
  digitalWrite(m1, HIGH);
  digitalWrite(m2, LOW);
  delay(1000*t);
  stopmotor();
}
void backward(int t) {
  digitalWrite(m1, LOW);
  digitalWrite(m2, HIGH);
  delay(1000*t);
  stopmotor();
}
void stopmotor() {
  //STOP
  digitalWrite(m1, LOW);
  digitalWrite(m2, LOW);
  //Serial.println("stop");
}

bool checkdistance() {
  digitalWrite(ustrig, LOW);
  delayMicroseconds(2);
  digitalWrite(ustrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(ustrig, LOW);
  float duration = pulseIn(usecho, HIGH);
  float dist = duration * 0.034/2;
  if (dist < 50) {
    return true;
  } else {
    return false;
  }
}
