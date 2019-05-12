#include <DHT.h>

#define DHTPIN A1

// Relay Pins
const int relayLight = 3, relayAC = 2;
//Global Variables
String lastCommand = "";
float temp_desired = 24, histeresis = 2, lastTempC = 0, lastHumidity = 0;
long timer = 0, endTimeMillis=0;
bool AC_ON = false, tempControl = false, timerOn =false;


//Initialize pin of Temp and Humidity Read
DHT dht(DHTPIN, DHT11);

void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600);
  dht.begin();

  // Relay's config
  pinMode(relayLight, OUTPUT);
  pinMode(relayAC, OUTPUT);
}

void loop()
{

  // Read temperature as Celsius (the default)
  float tempC = dht.readTemperature();
  float humidity = dht.readHumidity();
  if(lastTempC != tempC && lastHumidity!= humidity)
  {
  lastTempC = tempC;
  lastHumidity = humidity;
  Serial.println("Temperature: " + String(tempC)+ " Humidity: "+String(humidity));
  Serial.println("Temperature Desired: " + String(temp_desired)+" +/- "+String(histeresis)+" Celsius");  
  }
  delay(100);
  // Testa se nesta temperatura deve acionar ou desacionar o ar
  if(AC_ON && tempControl)
  {
    if (tempC < temp_desired + histeresis)
    {
      Serial.println("Auto Turn on TempControl");
      digitalWrite(relayAC, HIGH);
    }

    if (tempC > temp_desired - histeresis)
    {
      Serial.println("Auto Turn off TempControl");
      digitalWrite(relayAC, LOW);
    }
  }
  

  if(timer != 0)
  {
    endTimeMillis = millis() + (timer*100);
    timerOn =true;
    timer = 0;
  }

  if (millis()>endTimeMillis && timerOn)
  {
    Serial.println("AC relay was automaticly turn off because of the timer");
    digitalWrite(relayAC, LOW);
    AC_ON = false;
    timerOn = false;
  }
  if (Serial1.available() > 0)
  {
    String text = Serial1.readString();
    text.trim();
    Serial.println(text);
    if (text != lastCommand)
    {      
      lastCommand = text;
      if (text.equals("lights on"))
      {
        Serial.println("turning light on");
        digitalWrite(relayLight, HIGH);
        return;
      }
      else if (text == "lights off")
      {
        Serial.println("turning light off");
        digitalWrite(relayLight, LOW);
        return;
      }
      else if (text == "AC on")
      {
        Serial.println("turning AC on");
        AC_ON = true;
        digitalWrite(relayAC, HIGH);
        return;
      }
      else if (text == "AC off")
      {
        Serial.println("turning AC off");
        AC_ON = false;
        digitalWrite(relayAC, LOW);
        return;
      }
      else if (text == "temp on")
      {
        Serial.println("turning temp control on");
        tempControl = true;
        return;
      }
      else if (text == "temp off")
      {
        Serial.println("turning temp control off");
        tempControl = false;
        return;
      }
      else
      {
        float num = ExtractNumString(text);
        String command = ExtractTextString(text);
        if (command == "temp")
        {
          temp_desired = num;
          return;
        }
        else if (command == "timer")
        {
          Serial.println("timer set");
          timer = num;
          return;
        }
      }
    }
  }
}

//Extract the number out of the string
// return the number extracted as float
float ExtractNumString(String text)
{
  char charray[50];
  text.toCharArray(charray, 50);
  int i = 0;
  String result = "";
  for (i = 0; i < 50; i++)
  {
    char c = charray[i];
    if (isdigit(c))
    {
      result += c;
    }
  }
  return result.toFloat();
}

//Extract the text out of the string
// return the text extracted as string
String ExtractTextString(String text)
{
  char charray[10];
  text.toCharArray(charray, 10);
  int i = 0;
  String result = "";
  for (i = 0; i < 10; i++)
  {
    char c = charray[i];
    if (isAlpha(c))
    {
      result += c;
    }
  }
  return result;
}
