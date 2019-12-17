#include <LiquidCrystal.h>

const int rs = 8, en = 9, d4 = 4, d5 = 5, d6 = 6, d7 = 7; //Match LCD pins to Arduino pins
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
float weightVoltage;
int initialT = millis();

//Pre-state our functions
float calibrate(float *high, float *low);
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max);
float sumt, oldValue, flowRateprev;


void setup() {
    Serial.begin(9600); //begin serial port
    lcd.display(); //turn display on
    lcd.begin(16,2); //Initialize number of rows and columns
//    lcd.setCursor(0,0);
//    lcd.print("Awaiting weight");
//    lcd.setCursor(0,2);
//    lcd.print(" ");
    pinMode(weightVoltage, INPUT);
}

void loop() {
    int change,counterZero;
    float currentT, flowRate, endT, difT, counter,sum = 0, highRead, lowRead, highRead2, lowRead2;
    float values[20];
    int i;
    int button = analogRead(0); //button read
    float weightVoltage, weightVoltage2;
    if(button == 639) //each button has a set analog read value so we look for that when checking if the button was pressed
    {
      calibrate(&highRead, &lowRead); //call calibrate function
    }

    if(button == 409)
    {
      setZero(&lowRead); //call setZero function
    }
    
    for(i=0; i<20; i++)s
    {
      weightVoltage = analogRead(A1);//for 20 readings we take the input analog reading
      //Serial.print(weightVoltage );
      weightVoltage = mapfloat(weightVoltage, lowRead, highRead, 0, 100); //map the analog value between the calibrated values
      values[i] = weightVoltage; //input all the values into our array
      delay(50);
    }
    for(i=0; i<20; i++)
    {
      sum = sum + values[i]; //sum the values of the array
    }
    sumt = sum/20;
    flowRate = ((sumt-oldValue)/1.010); //flow rate calculation

    if(sumt>-5&&sumt<110)
    {
      if(sumt>=0) //if the weight is between our desired range and not negative then it is printed ~(so a negative weight is not displayed)
      {
        lcd.setCursor(0,0);
        lcd.print("Weight:         ");
        lcd.setCursor(9,0);
        lcd.print(sumt);
        lcd.setCursor(0,1);
        lcd.print("Flow rate:");
      } else if(sumt<0) {
        lcd.setCursor(0,0);
        lcd.print("Weight: ");
        lcd.print("0       ");
        lcd.setCursor(0,1);
        lcd.print("Flow rate:");
      }
    } else{
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Please calibrate"); //if the weight is a non-zero value then it needs calibrating
    }
    if(flowRate>10&&flowRate<50){
      
      lcd.setCursor(12,1);
      flowRateprev = flowRate;
      lcd.print(flowRateprev);
    }
    oldValue = sumt; //. set our prevous weight to the current weight
    delay(10);
}

float calibrate(float *high, float *low) //input pointers to point our the output values back to the void loop function
{
  float weightVoltage, weightVoltage2, sum = 0, sumt = 0, sum2 = 0, sumt2 = 0;
  float values[20];
  float values2[20];
  int i;
  lcd.setCursor(0,0);
  lcd.clear();
  lcd.print("Take off any");
  lcd.setCursor(0,1);
  lcd.print("weight"); 
  delay(3000);
  lcd.clear();
  for(i=3; i>=0; i--)
  {
    lcd.setCursor(0,0);
    lcd.print(i);
    delay(800); k- ijn
  }
  for(i=0; i<20; i++) //take avg of analog read values at no weight 
  {
    weightVoltage2 = analogRead(A1);
    values2[i] = weightVoltage2;
    delay(50);
  }
  for(i=0; i<20; i++)
  {
    sum2 = sum2 + values2[i];
  }
  sumt2 = sum2/20;
  lcd.setCursor(0,0);
  lcd.print("Place 100g");
  delay(3000);
  lcd.clear();
  for(i=3; i>=0; i--)
  {
    lcd.setCursor(0,0);
    lcd.print(i);
    delay(800);
  }
    for(i=0; i<20; i++) //take avg at 100g
    {
      weightVoltage = analogRead(A1);
      values[i] = weightVoltage;
      delay(5);
    }
    for(i=0; i<20; i++)
    {
      sum = sum + values[i];
    }
    sumt = sum/20;
//    Serial.print(sumt);
//    Serial.print(weightVoltage);
      lcd.clear();
      *high = sumt;
      *low = sumt2; //point our values back to our input adresses
}

void setZero(float *low)
{
  float weightVoltage, weightVoltage2, sum = 0, sumt = 0, sum2 = 0, sumt2 = 0;
  float values[20];
  float values2[20];
  int i;
  for(i=0; i<20; i++)
  {
    weightVoltage2 = analogRead(A1);
    values2[i] = weightVoltage2;
    delay(50);
  }
  for(i=0; i<20; i++)
  {
    sum2 = sum2 + values2[i];
  }
  sumt2 = sum2/20;
  *low = sumt2;
}

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
 return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
