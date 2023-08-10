 //OLED Library
 #include <Wire.h>
 #include <Adafruit_GFX.h>
 #include <Adafruit_SSD1306.h>

 //OLED Screen Resolution
 #define SCREEN_WIDTH 128
 #define SCREEN_HEIGHT 32

 // declare an SSD1306 display object connected to I2C
 Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

 //DHT Library 
 #include<dht.h>

 //Servo motor Library
 #include<Servo.h>
 // create servo object to control a servo
 Servo myservo;
 int pos = 0;

 //DHT pin
 #define dht_apin A1
 //object of DHT
 dht DHT;

 //sr04 pins
 #define echoPin 8
 #define trigPin 9
 // defines variables
 long duration; // variable for the duration of sound wave travel
 int distance; // variable for the distance measurement

 // RTC Library
 #include <RTClib.h>
 //object for time module 
 RTC_DS3231 rtc;
 char t[32];

 //Soil Sensor pins

 #define sensorPin A0

 #define soilWet 500 // Define max value we consider soil 'wet'
 #define soilDry 750 // Define min value we consider soil 'dry'

 #define buzzerPin 13

 void setup() {
 //BAUD Rate
 Serial.begin(9600);

 //time
 rtc.begin();
 rtc.adjust(DateTime(F(__DATE__),F(__TIME__)));

 // initialize OLED display with address 0x3C for 128x32
 if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
 Serial.println(F("SSD1306 allocation failed"));
 while (true);
 }
 delay(2000); // wait for initializing
 oled.clearDisplay(); // clear display
 oled.setTextSize(1); // text size
 oled.setTextColor(WHITE); // text color
 oled.setCursor(0, 10); // position to display
 oled.println("WELCOME!!"); // text to display
 oled.display();

 // attaches the servo on pin 7 to the servo object
 myservo.attach(7); 

 // Sets the trigPin as an OUTPUT
 pinMode(trigPin, OUTPUT); 
 // Sets the echoPin as an INPUT
 pinMode(echoPin, INPUT);

 //set Buzzer Pin as an OUTPUT
 pinMode(buzzerPin, OUTPUT);
 digitalWrite(buzzerPin, LOW);

 }

 void loop() {

 int distance = getDistance();
 char data;
 if(distance<10){

 for(int i=0; i<5; i++){

 digitalWrite(buzzerPin, HIGH);
 delay(500);
 digitalWrite(buzzerPin, LOW);
 delay(500);
 }
 Serial.println("Intruder Detected!!!!");
 }

 String time = getTime();

 int temp = getTemp();



 int soilMoisture = getMoisture();
 // Determine status of our soil
 if (soilMoisture < soilWet) {
 Serial.println("Status: Soil is too wet");

 } else if (soilMoisture >= soilWet && soilMoisture < soilDry) {
 Serial.println("Status: Soil moisture is perfect");

 } else {
 Serial.println("Status: Soil is too dry - time to water!");
 rotateServo();
 }

 displayString("Time: ",t);
 delay(3000);
 displayValue("Distance: ",distance);
 delay(3000);
 displayValue("Temperature: ",temp);
 delay(3000);
 displayValue("Soil Moisture: ",soilMoisture);

 }

 int getTemp(){

 DHT.read11(dht_apin);
 int temp = DHT.temperature;

 Serial.print("temperature = ");
 Serial.print(temp); 
 Serial.println("C ");
 delay(2000);

 return temp;
 }

 void displayValue(String text ,int value){
 oled.clearDisplay();
 oled.setCursor(0, 10);
 oled.print(text);
 oled.print(value);
 oled.display();
 }

 void displayString(String text, String value){
 oled.clearDisplay();
 oled.setCursor(0, 10);
 oled.print(text);
 oled.print(value);
 oled.display();
 }

 String getTime(){
 DateTime now = rtc.now();
 sprintf(t, "%02d:%02d:%02d", now.hour(), now.minute(), now.second()); 
 //Serial.print(F("Time: "));
 //Serial.println(t);
 delay(1000);
 return t;
 }

 int getDistance(){

 // Clears the trigPin condition
 digitalWrite(trigPin, LOW);
 delayMicroseconds(2);
 // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
 digitalWrite(trigPin, HIGH);
 delayMicroseconds(10);
 digitalWrite(trigPin, LOW);
 // Reads the echoPin, returns the sound wave travel time in microseconds
 duration = pulseIn(echoPin, HIGH);
 // Calculating the distance
 distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go 
and back)
 // Displays the distance on the Serial Monitor
 Serial.print("Distance: ");
 Serial.print(distance);
 Serial.println(" cm");
 delay(1000);

 return distance;
 }

 // function returns the analog soil moisture measurement
 int getMoisture() {
 int soilMoisture = analogRead(sensorPin); // Read the analog value form 
sensor
 Serial.print("Soil Moisture: ");
 Serial.println(soilMoisture);
 return soilMoisture; // Return analog moisture value
 }

 void rotateServo(){
 Serial.println("TAP OPENING!!!");
 displayString("TAP OPENING!!!","");
 for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 
degrees
 // in steps of 1 degree
 myservo.write(pos); // tell servo to go to position in 
variable 'pos'
 //delay(500); // waits 15ms for the servo to 
reach the position
 }

 delay(3000);
 Serial.println("TAP CLOSING!!!");
 displayString("TAP CLOSING!!!","");
 for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 
degrees
 myservo.write(pos); // tell servo to go to position in 
variable 'pos'
 //delay(500); // waits 15ms for the servo to 
reach the position
 }

 }
