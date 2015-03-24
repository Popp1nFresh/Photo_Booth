June 2012
http://eclipse.clinertech.com
Portions of code (IR sequence) from LuckyLarry.co.uk and bigmike.it/ircontrol.


LUCKYLARRY.CO.UK - IR Remote control for Nikon using Arduino
Mimics the infrared signal to trigger the remote for any Nikon camera
which can use the ML-L1 and ML-L3 remotes. Can be used as an intervalometer
for time lapse photography.
The IR sequence I used is originally taken from: http://www.bigmike.it/ircontrol/
You should be able to use my pulse methods to alter to suit other cameras/ hardware.
micros() is an Arduino function that calls the time in Microseconds since your program
first ran. Arduino doesn't reliably work with microseconds so we work our timings by
taking the current reading and then adding our delay on to the end of it rather than rely
on the in built timer.



On "take photo button" push the camera will delay for a predetermined time (timeBeforeFirstPhoto)
before taking a preset number of pictures in a row (numPicToTake). There will be a set delay
(delayBetweenPics) which will allow the users to review the picture on a screen connenected to the
camera (in this case via HDMI) before takeing another picture.


EXAMPLE:
User presses "take photo button".
Countdown light flashes.
First photo is taken.
Delay so picture can be reviewed on screen. (not interactive)
Take second photo.
Delay.
Repeat while numPicsToTake is less than count.
Wait for button press.

NOTES:
--The delay between each of the pictures taken should take into account the amount of time an image
is displayed on the screen for by the camera.
*/

int TAKE_PHOTObutton = 8;         // Pin that should be monitored for button push. --> Indicates that delay before first photo should start.
int IRled = 12;                   // IR LED uses digital pin 12.
int ACTIONled = 13;               // Indicator LED uses digital pin 13 (also onboard LED).
int READYled = 11;                // LED to blink before taking first photo (timeBeforeFirstPhoto).
int numPicturesTaken = 0;         // Current count of number of overall pictures taken.
int pictureDelayInSeconds = 0;    // Picture delay in seconds which is used to calculate total time elapsed in minutes.

/* Variables to Change. */
int pictureDelay = 29000 - 500;   // Delay time between pictures --> How long between shots. -500 due to ACTIONled needing some display (on) time.
int numPicToTake = 800;             // Number of pictures to take each time "take photo button" pressed.
int delayBetweenPics = 6000;      // Delay between each of the pictures taken.
int timeBeforeFirstPhoto = 500;  // Time to wait before first photo is taken.


/************ Prepare the Arduino. ************/
void setup() {
  Serial.begin(9600);                                     // For potential serial print.
  pictureDelayInSeconds = (pictureDelay + 500) / 1000;    // Determine the equivalent seconds of requested picture delay.
  
  /* Set pins as output. */
  pinMode(IRled, OUTPUT);
  pinMode(ACTIONled, OUTPUT);
  pinMode(READYled, OUTPUT);
  pinMode(TAKE_PHOTObutton, INPUT);
} // END setup()


/************ Flash the READYled before taking the first photo. ************/
void readyLightFlashAndDelay(int timeBeforeFirstPhoto) {

  int flashCount = 0;
  unsigned long numberOfFlashes = ((timeBeforeFirstPhoto / 1000) * 2) - 1;
  
  while(flashCount < numberOfFlashes) 
  {
    digitalWrite(READYled, HIGH);               // Turn on the READYled.
    delay(400);
    digitalWrite(READYled, LOW);                // Turn READYled off.
    delay(300); 
    flashCount++;
  }
  
    digitalWrite(READYled, HIGH);               // Turn on the READYled.
    delay(1500);                                // Have the READYled turn solid just before the picture is taken.
    digitalWrite(READYled, LOW);                // Turn READYled off.
}// END readyLightFlashAndDelay()


/************ Set pulse of IR signal. ************/
void pulseON(int pulseTime) {
  unsigned long endPulse = micros() + pulseTime;        // Add the desired pulseTime to the current time (from micros()) to get the time that pulseON should quite.
  while( micros() < endPulse)                           // While the current time (from micro()) is less than the endPulse time keep going.
  {
    digitalWrite(IRled, HIGH);                          // Turn on the IR LED.
    delayMicroseconds(13);                              // Half the clock cycle for 38Khz (26.32×10-6s) - e.g. the 'on' part of our wave.
    digitalWrite(IRled, LOW);                           // Turn IR off.
    delayMicroseconds(13);                              // Delay for the other half of the clock cycle --> generate wave/ oscillation.
  }
}// END pulseON()


/************ Send nothing --> a delay. ************/
void pulseOFF(unsigned long startDelay) {
  unsigned long endDelay = micros() + startDelay;       // Simply create a delay --> current_time + desired_delay_length = stop_time
  while(micros() < endDelay);
}// END pulseOFF()


/************ Send the signal to take a picture. ************/
void takePicture() {
  // Signal twice.
  for (int i=0; i < 2; i++) {
    pulseON(2336); pulseOFF(646); pulseON(1168); pulseOFF(621); pulseON(572); pulseOFF(621);
    pulseON(1168); pulseOFF(621); pulseON(1168); pulseOFF(621); pulseON(572); pulseOFF(621);
    pulseON(1168); pulseOFF(621); pulseON(572); pulseOFF(646); pulseON(572); pulseOFF(621);
    pulseON(1168); pulseOFF(621); pulseON(572); pulseOFF(621); pulseON(1168); pulseOFF(621);
    pulseON(1168); pulseOFF(621); pulseON(1168); pulseOFF(621); pulseON(572); pulseOFF(621);
    pulseON(572);  pulseOFF(646); pulseON(572); pulseOFF(621); pulseON(1168); pulseOFF(621);
    pulseON(1168); pulseOFF(621); pulseON(1168); pulseOFF(621); pulseON(1168); pulseOFF(11008);
    pulseON(2336); pulseOFF(621); pulseON(1168); pulseOFF(621); pulseON(572); pulseOFF(621);
    pulseON(1168); pulseOFF(621); pulseON(1168); pulseOFF(621); pulseON(572); pulseOFF(621);
    pulseON(1168); pulseOFF(621); pulseON(572); pulseOFF(646); pulseON(572); pulseOFF(621);
    pulseON(1168); pulseOFF(621); pulseON(572); pulseOFF(621); pulseON(1168); pulseOFF(621);
    pulseON(1168); pulseOFF(621); pulseON(1168); pulseOFF(621); pulseON(572); pulseOFF(621);
    pulseON(572);  pulseOFF(646); pulseON(572); pulseOFF(621); pulseON(1168); pulseOFF(621);
    pulseON(1168); pulseOFF(621); pulseON(1168); pulseOFF(621); pulseON(1168); pulseOFF(11008);
    pulseON(2336); pulseOFF(621); pulseON(1168); pulseOFF(621); pulseON(572); pulseOFF(621);
    pulseON(1168); pulseOFF(621); pulseON(1168); pulseOFF(621); pulseON(572); pulseOFF(621);
    pulseON(1168); pulseOFF(621); pulseON(572); pulseOFF(646); pulseON(572); pulseOFF(621);
    pulseON(1168); pulseOFF(621); pulseON(572); pulseOFF(621); pulseON(1168); pulseOFF(621);
    pulseON(1168); pulseOFF(621); pulseON(1093); pulseOFF(696); pulseON(572); pulseOFF(621);
    pulseON(572);  pulseOFF(621); pulseON(572); pulseOFF(621); pulseON(572); pulseOFF(1218);
    pulseON(497);  pulseOFF(1292); pulseON(422); pulseOFF(1367); pulseON(373); pulseOFF(11803);
    pulseON(298);  pulseOFF(2659); pulseON(199); pulseOFF(1590); pulseON(174); pulseOFF(1019);
    pulseON(174);  pulseOFF(1615); pulseON(174); pulseOFF(1615); pulseON(149); pulseOFF(1044);
    pulseON(149);  pulseOFF(1640); pulseON(124); pulseOFF(1093); pulseON(149); pulseOFF(1044);
    pulseON(124);  pulseOFF(1665); pulseON(124); pulseOFF(1068); pulseON(124); pulseOFF(1665);
    pulseON(99);   pulseOFF(1690); pulseON(99); pulseOFF(1690); pulseON(99); pulseOFF(1093);
    pulseON(99);   pulseOFF(1118); pulseON(99); pulseOFF(1093); pulseON(99); pulseOFF(1690);
    pulseON(99);   pulseOFF(1690); pulseON(75); pulseOFF(1715); pulseON(75); pulseOFF(12101);
    pulseON(149);  pulseOFF(2833); pulseON(75); pulseOFF(1715); pulseON(75); pulseOFF(1118);
    pulseON(75);   pulseOFF(1715); pulseON(75); pulseOFF(1715); pulseON(75); pulseOFF(1118);
    pulseON(75);   pulseOFF(1715); pulseON(75); pulseOFF(1118); pulseON(99); pulseOFF(1093);
    pulseON(99);   pulseOFF(1690); pulseON(99); pulseOFF(1093); pulseON(99); pulseOFF(1690);
    pulseON(99);   pulseOFF(1690); pulseON(99); pulseOFF(1690); pulseON(99); pulseOFF(1093);
    pulseON(99);   pulseOFF(1118); pulseON(99); pulseOFF(1093); pulseON(99); pulseOFF(1690);
    pulseON(99);   pulseOFF(1690); pulseON(99); pulseOFF(1690); pulseON(99); pulseOFF(646);
  }//END for loop
  numPicturesTaken++;    // Add one to the toal number of pictures taken.
}// END takePicture()


/************ What should the Arduino continue to do? ************/
void loop() {
  int picsTaken = 0;                            // Number of pictures taken since "take photo" was last pressed.
  while(digitalRead(TAKE_PHOTObutton) == LOW);  // WAIT until button is pressed.
  delay(10);                                    // Helps "filter" out random noise that was causing continuous picture taking.
  if(digitalRead(TAKE_PHOTObutton) == HIGH)     // If the button is still pressed then must be true button press.
  {
    //readyLightFlashAndDelay(timeBeforeFirstPhoto); // Flash READYled before taking first photo.
    while(picsTaken < numPicToTake) 
    {
      takePicture();                              // Take the picture.
      if(picsTaken != numPicToTake - 1)           // Turn on READYled for next picture ONLY if there is another picture to be taken.
      {
        digitalWrite(READYled, HIGH);             
        delay(delayBetweenPics);                  // Delay between each picture.
        digitalWrite(READYled, LOW);              
      }
      picsTaken++;                                // Increment the pictures taken counter.
    }// END while
    Serial.print("Picture Count: "); Serial.println(numPicturesTaken);
  }
}// END loop()
