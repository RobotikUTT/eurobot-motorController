/**
 * This code let you test the encoders
 */
 
#define ENCODER_L_INT 0 //Pin 2 on nano
#define ENCODER_L_DIR 4
#define ENCODER_R_INT 1 //Pin 3 on nano
#define ENCODER_R_DIR 5

#define PRINT_SLEEP 50 //ms

long rightPos = 0;
long leftPos = 0;
unsigned long lastPrint = 0;

void leftTick()
{
   if(digitalRead(ENCODER_L_DIR))
     leftPos++;
   else
     leftPos--;
}
   
void rightTick()
{
   if(digitalRead(ENCODER_R_DIR))
     rightPos++;
   else
     rightPos--;
}
   
void setup()
{
  attachInterrupt(ENCODER_L_INT, leftTick, FALLING);
  attachInterrupt(ENCODER_R_INT, rightTick, FALLING);
  Serial.begin(9600);
  lastPrint = millis();
}

void loop()
{
  if(lastPrint + PRINT_SLEEP < millis())
  {
    lastPrint = millis();
    Serial.print(leftPos, DEC);
    Serial.print("|");
    Serial.println(rightPos, DEC);
  }
}
