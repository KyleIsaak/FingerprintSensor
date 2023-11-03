# FingerprintSensor
## An arduino circuit that replaces a computer's power button with a fingerprint sensor

See included png and pdf files for the diagrams required to build the circuit

This project is built using Adafruit's fingerprint sensor and associated library.
<br>
<br>

To import the library for yourself, follow these instructions:
```
1. Open the Arduino IDE
2. On the top bar, go click on tools -> manage libraries
3. Search for "fingerprint"
4. Find "Adafruit Fingerprint Sensor Library" and click install
```
<br>
<br>

To edit how long you'd like to simulate a button press, edit these delays in Fingerprint_lock.ino:
```
void unlockPC(){
  digitalWrite(pcPin, HIGH);
  Serial.println("PC Turning On");
  delay(200);
  digitalWrite(pcPin, LOW);
}

void lockPC(){
  digitalWrite(pcPin, HIGH);
  Serial.println("PC Shutting Down");
  delay(10000);
  digitalWrite(pcPin, LOW);
}
```
<br>
<br>

To change the code so it only has one type of button press instead of one short press and one long press, edit the following code and remove the if statement:

```
void loop()                 
{
  delay(50);     //don't ned to run this at full speed.
  RGB(255,50,0); //Sets the RGB LED to it's idle yellow colour
  
  fingerStatus = getFingerprintIDez();
  if(fingerStatus != -1 and fingerStatus != -2 ){
    Serial.println("Finger accepted");
    //triggerLock();
    if(pcOn == true){
      lockPC();
      pcOn = false;
    }
    else{
      unlockPC();
      pcOn = true;
    }
  }
```
After editing, it should look like this:
```
void loop()                 
{
  delay(50);     //don't ned to run this at full speed.
  RGB(255,50,0); //Sets the RGB LED to it's idle yellow colour
  
  fingerStatus = getFingerprintIDez();
  if(fingerStatus != -1 and fingerStatus != -2 ){
    Serial.println("Finger accepted");
    pressButton();  
    }
  }
```
Next, edit the corresponding function at the top of Fingerprint_Lock.io to set the delay that you want:
```
void pressButton(){
  digitalWrite(pcPin, HIGH);
  Serial.println("Power Button Pressed");
  delay(200);
  digitalWrite(pcPin, LOW);
}
