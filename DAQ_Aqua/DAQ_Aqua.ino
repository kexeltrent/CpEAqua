
#include <TimeLib.h>


#include <DHT.h>
 
#include <OneWire.h>
#include <DallasTemperature.h>


#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN A1   
#define DHTTYPE DHT11  
#define ONE_WIRE_BUS 5

#define SENSOR A0            //pH meter Analog output to Arduino Analog Input 0
#define OFFSET 0.00            //deviation compensate
#define LED 13
#define SAMPLING_INTERVAL 20
#define PRINT_INTERVAL 800
#define ARRAY_LENGTH  40    //times of collection

#define TIME_HEADER  "T"   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message 


int PH_ARRAY[ARRAY_LENGTH];   //Store the average value of the sensor feedback
int PH_ARRAY_INDEX=0;  
#define SensorPin A0 //pH meter Analog output to Arduino Analog Input 0
unsigned long int avgValue; //Store the average value of the sensor feedback
float b;
int buf[10],temp;

long duration;
float linearlength;
float dist;


void wt_tmp();
void dht_sen();
void wt_lvl();
void ph_sen();




DHT dht(DHTPIN, DHTTYPE);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

float Celcius=0;
float Fahrenheit=0;

const int trig = 6;
const int echo = 7;




void setup(void)
{
//TIME START
Serial.begin(9600);
  setTime(1, 00, 00, 11, 12, 2019);


//TIME END

//WT START
  
  //Serial.println("Initializing WT...!");
  sensors.begin();
//WT END

//DHT start
  //Serial.println("Initializing DHT...!");
  dht.begin();
//DHT end

//Water Level Start

  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
//Water Level End

//PH START
  pinMode(LED,OUTPUT);   
  pinMode(13,OUTPUT);
  //Serial.println("Initializing PH...!");    //Test the serial monitor
//PH END

//LCD START
lcd.begin(20, 4);
//LCD END
}

void loop(void)
{
//TIME START
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year());  
  Serial.print(","); 
// TIME END

    wt_tmp();
    dht_sen();
    wt_lvl();
    ph_sen();



}


//Functions are beyond here:

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

void ph_sen(){
  for(int i=0;i<10;i++) //Get 10 sample value from the sensor for smoothing the value
{
buf[i]=analogRead(SensorPin);
delay(10);
}
for(int i=0;i<9;i++) //sort the analog from small to large
{
for(int j=i+1;j<10;j++)
{
if(buf[i]>buf[j])
{
temp=buf[i];
buf[i]=buf[j];
buf[j]=temp;
}
}
}
avgValue=0;
for(int i=2;i<8;i++) //take the average value of 6 center sample
avgValue+=buf[i];
float phValue=(float)avgValue*5.0/1024/6; //convert the analog into millivolt
phValue=3.5*phValue+0.2; //convert the millivolt into pH value
  //Serial.print("PH: ");
  Serial.print(phValue,2);
  Serial.println("");


lcd.setCursor(11,1);
lcd.print("PH:");
lcd.setCursor(14,1);
lcd.print(phValue,2);
}


void wt_tmp(){
  sensors.requestTemperatures(); 
  Celcius=sensors.getTempCByIndex(0);
  Serial.print(Celcius);
  Serial.print(",");
  lcd.setCursor(0,0);
  lcd.print("WC:");
  lcd.setCursor(3,0);
  lcd.print(Celcius);



  delay(2000);
}

void dht_sen(){

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  
  if (isnan(h) || isnan(t) ) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  float hif = dht.computeHeatIndex(f, h);

  float hic = dht.computeHeatIndex(t, h, false);

  //Serial.print("EH: ");
  Serial.print(h);
 
  Serial.print(",");
  lcd.setCursor(0,1);
  lcd.print("EH:");
  lcd.setCursor(3,1);
  lcd.print(h);
  lcd.setCursor(8,1);
  lcd.print("%");

  //Serial.print("ET: ");
  Serial.print(t);
  Serial.print(",");
  lcd.setCursor(0,2);
  lcd.print("ET:");
  lcd.setCursor(3,2);
  lcd.print(t);
  
  //Serial.print(" *C ");
   //Serial.print("\n");

  //Serial.print("HI: ");
  Serial.print(hic);
  Serial.print(",");
  lcd.setCursor(0,3);
  lcd.print("HI:");
  lcd.setCursor(3,3);
  lcd.print(hic);



}

void wt_lvl(){
digitalWrite(trig, LOW);
delayMicroseconds(2);
// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trig, HIGH);
delayMicroseconds(10);
digitalWrite(trig, LOW); // Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echo, HIGH); // Calculating the distance
linearlength = duration*0.034/2;
dist= 25.4- linearlength;  //Height of Water in the reservoir in cm
//Serial.print("WL:");
Serial.print(dist);
Serial.print(",");
lcd.setCursor(11,0);
lcd.print("WL:");
lcd.setCursor(14,0);
lcd.print(dist);

}

void time_plot(){
  
}






  
