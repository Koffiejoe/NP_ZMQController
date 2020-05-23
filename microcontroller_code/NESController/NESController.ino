#define CLOCK 4
#define LATCH 3
#define DATA  2                                                         
#define CONTROLLERID "NES"

#define SAYHELLO    1
#define SENDDATA    2
#define GETTYPE     3
#define NOTHING     0
#define INVALIDCOM  -1

//functions
//A B START SELECT UP DOWN LEFT RIGHT
byte getNESData()
{
  byte controllerTemp = 0;
  digitalWrite(LATCH, HIGH);
  digitalWrite(LATCH, LOW);
  for(int i = 0; i < 8; i++)
  {
    controllerTemp |= !digitalRead(DATA) << i ;
    digitalWrite(CLOCK, HIGH);
    digitalWrite(CLOCK, LOW);
  }
  return controllerTemp;
}

void setup() 
{
  pinMode(LATCH,  OUTPUT);
  pinMode(CLOCK,  OUTPUT);
  pinMode(DATA,   INPUT);
  Serial.begin(9600);
}

void loop()
{
  String test = Serial.readStringUntil('\n');
  if (test == "init")
  {
    Serial.println("ACK");
  }
  else if (test == "gData")
  {
    Serial.println(getNESData());
  }
  else if (test == "gType")
  {
    Serial.println(CONTROLLERID);
  }
  else if (test == "")
  {
    //do nothing
  }
  else
  {
    Serial.print("INV COM");
    Serial.print(test);
  }
}
