#define readVacuum A1 // analog vacuum sensor input
#define gripSignal 4 // digital high/low to X67 CAN IO, MOSFET needed 24VDC
#define gripSwitch 2 // user grip input from handle 
#define redLED 6 // output for red LED, MOSFET needed 12VDC   
#define greenLED 8 // output for green LED, MOSFET needed 12VDC
#define grip_state 10 // input used to determine grip state from X67 CAN IO

int vacuum=0;
int loopDelay=100; // miliseconds
int inHG=0; // converted to inHG from raw input
int gripPulse=100; // time in miliseconds a high is sent to X67
int gripThreshold=7; // min vacuum required to switch to trigger float mode
// int gripTimmer=0; // rolling value of above vacuum holding time

void setup() {
  pinMode(readVacuum,INPUT);
  pinMode(gripSignal,OUTPUT);
  pinMode(gripSwitch,INPUT_PULLUP); 
  pinMode(redLED,OUTPUT);
  pinMode(greenLED,OUTPUT);
  pinMode(grip_state,INPUT);
  Serial.begin(9600);
}

void loop() {
  
  vacuum=analogRead(readVacuum);
  inHG=-0.0269*vacuum+25.841;
  Serial.println();
  Serial.print("Vacuum = ");
  Serial.print(vacuum);
  Serial.println();
  Serial.print("Vacuum = ");
  Serial.print(inHG);
  Serial.print(" inHG");
  Serial.println();

  // grip switch is pressed and vacuum is above safe threshold to grip
  if(digitalRead(gripSwitch)==LOW && inHG>=gripThreshold) {
    digitalWrite(gripSignal,HIGH);
    delay(gripPulse);
    digitalWrite(gripSignal,LOW);
    Serial.println("Switch LOW");
  }

  // grip switch is pressed and load is still active, not set down
  // Gorbel software won't release load till it senses weight set down
  if(grip_state==HIGH && digitalRead(gripSwitch)==LOW)  {
    digitalWrite(gripSignal,HIGH);
    delay(gripPulse);
    digitalWrite(gripSignal,LOW);
    Serial.println("Switch HIGH");
  }
  
  delay(loopDelay); // clock cycle control
  
}
