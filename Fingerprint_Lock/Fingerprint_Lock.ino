/*************************************************** 
  This is an example sketch for our optical Fingerprint sensor

  Designed specifically to work with the Adafruit BMP085 Breakout 
  ----> http://www.adafruit.com/products/751

  These displays use TTL Serial to communicate, 2 pins are required to 
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/


#include <Adafruit_Fingerprint.h>

// On Leonardo/Micro or others with hardware serial, use those! #0 is green wire, #1 is white
// uncomment this line:
// #define mySerial Serial1

// For UNO and others without hardware serial, we must use software serial...
// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
// comment these two lines if using hardware serial
SoftwareSerial mySerial(2, 3);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

int redPin = 9;
int greenPin = 11;
int bluePin = 10;

int lockPin = 13;
int pcPin = 13;

int fingerStatus = -1;

bool locked = true;
bool pcOn = false;


void RGB(int redValue, int greenValue, int blueValue){
  analogWrite(redPin, 255-redValue);
  analogWrite(greenPin, 255-greenValue);
  analogWrite(bluePin, 255-blueValue);
}

void triggerLock(){
  digitalWrite(lockPin, HIGH);
  Serial.println("UNLOCKED");
  delay(5000);
  digitalWrite(lockPin, LOW);
  Serial.println("LOCKED");
  
}

// Function for short button press
void unlockPC(){
  digitalWrite(pcPin, HIGH);
  Serial.println("PC Turning On");
  delay(200);
  digitalWrite(pcPin, LOW);
}

// Function for long button press
void lockPC(){
  digitalWrite(pcPin, HIGH);
  Serial.println("PC Shutting Down");
  delay(10000);
  digitalWrite(pcPin, LOW);
}

// Function that is used for single button mode
// instead of one long press and one short press
void pressButton(){
  digitalWrite(pcPin, HIGH);
  Serial.println("Power Button Pressed");
  delay(200);
  digitalWrite(pcPin, LOW);
}

void setup()  
{
  //*********************************LIBRRARY CODE***************************************************
  Serial.begin(9600);
  while (!Serial);  // For Yun/Leo/Micro/Zero/...
  delay(100);
  //Serial.println("\n\nAdafruit finger detect test");

  // set the data rate for the sensor serial port
  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }

  finger.getTemplateCount();
  Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  Serial.println("Waiting for valid finger...");

//*********************************MY CODE***************************************************

pinMode(redPin, OUTPUT);
pinMode(greenPin, OUTPUT);
pinMode(bluePin, OUTPUT);
pinMode(lockPin, OUTPUT);

pcOn = false;
}





void loop()                 
{
  delay(50);     //don't ned to run this at full speed.
  RGB(255,50,0); //Sets the RGB LED to it's idle yellow colour
  
  // Check fingerprint and trigger power button if the fingerprint is accepted.
  // Switches between short press and long press each time it is run.
  fingerStatus = getFingerprintIDez();
  if(fingerStatus != -1 and fingerStatus != -2 ){
    Serial.println("Finger accepted");
    if(pcOn == true){
      lockPC();
      pcOn = false;
    }
    else{
      unlockPC();
      pcOn = true;
    }
  }
  
  else{
    if(fingerStatus == -2){
    Serial.println("INVALID PRINT");
    RGB(255,0,0); //Sets the RGB LED to red
    delay(2000);
    }
  }
  
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p!=2){
    Serial.println(p);
  }
  if (p != FINGERPRINT_OK)  return -1;
  
  p = finger.image2Tz();
  if (p!=2){
    Serial.println(p);
  }
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -2;
  
  // The code from here to the end of the function only runs if a match was found.
  // (Previous return statements prevent it from running if an invalid print is scanned)
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence);

  // Allows different results for different register prints based on fingerID.
  if(finger.fingerID == 1){
    Serial.println("Finger: Thumb");
    RGB(0,255,0); //Sets the RGB LED to green
    delay(1000);
  }
  else if(finger.fingerID == 2){
    Serial.println("Finger: Index");
    RGB(0,0,255); //Sets the RGB LED to blue
    delay(1000);
  }
  else if(finger.fingerID == 3){
    Serial.println("Finger: Pinky");
    RGB(255,0,200); //Sets the RGB LED to purple
    delay(1000);
  }
  
  return finger.fingerID; 
}
