/* Nikon Timelapse IR Remote Trigger - Bill Turnbull
 *  
 * The purpose is to fire the shutter trigger signal via IR LED at one of 6 set times 1.5, 4, 6, 11, 21 or 31 seconds (for each common exposure long time plus 1 second)
 * Once powered on, the Set button needs to be held down to set each exposure mode, which cascades up through each mode. If the button isn't held down, it defaults to 1.5 seconds
 * An LED for each mode (aside from default) turns on in turn while the button is down and there is a delay of 1 second between each mode test to allow time to release the button
 * Once the mode setup is complete, a 2 second delay is set before the first firing of the trigger signal. The signal simplay repeats forever until power off. 
 * 
 * Tip: Go into settings in the camera and extend the timer that turns off the IR remote - so you don't wonder why it's not working if you weren't ready to start after turning remote on */


int set_button = 2; // Set Delay button Pulled High
int delay_led1 = 3; // Delay mode display LEDs - Mode pins need to be PWM capable for dimmer LEDs
int delay_led2 = 5;
int delay_led3 = 6;
int delay_led4 = 9;
int delay_led5 = 10;
int set_test = 0; // Counter for testing for how long to set delay mode. Setup runs the test 5 times to count which mode to set and turns on each delay LED as it runs
int exp_delay_time = 1500; // Actual delay time between Exposure Triggers

int exp_delay = 0; // Delay between Exposure Triggers Mode number
int set_wait = 0; // How long to wait to see if the Set Button is HIGH for each cycle



void setup() {
pinMode(set_button,INPUT);
digitalWrite(set_button, HIGH); // Turn on Set button Pull Up resistor
pinMode(delay_led1,OUTPUT);
pinMode(delay_led2,OUTPUT);
pinMode(delay_led3,OUTPUT);
pinMode(delay_led4,OUTPUT);
pinMode(delay_led5,OUTPUT);
Serial.begin(9600);


while (set_test < 5) {
  exp_delay_set_test(); // Call the test for whether the set button has been pressed
  set_test = set_test +1;
  if (exp_delay == 1) {
    analogWrite(delay_led1, 15); // Turn on the corresponding LED to the exposure delay setting - If the set button isn't pressed, the default mode is used (1.5 seconds)
    exp_delay_time = 4000;
      }
  if (exp_delay == 2) {
    analogWrite(delay_led2, 15); // etc etc
    exp_delay_time = 6000;
      }
  if (exp_delay == 3) {
    analogWrite(delay_led3, 15); // etc etc
    exp_delay_time = 11000;
      }
  if (exp_delay == 4) {
    analogWrite(delay_led4, 15); // etc etc
    exp_delay_time = 21000;
      }
  if (exp_delay == 5) {
    analogWrite(delay_led5, 15); // etc etc
    exp_delay_time = 31000;
      }
  
  Serial.print(" Exposure Delay - "); // Debug to display which mode and what the corresponding time is in seconds as the setup runs
  Serial.print(exp_delay);
  Serial.print("  -  ");
  Serial.print(exp_delay_time / 1000);
  Serial.println(" Seconds");
  
  delay(1000); // Setup waits 1 sec so the button can be released between mode counts
}

delay(2000); // Just a delay between setup and first firing

}


void loop() {                                     // Sending the trigger signal via pin 13
    Serial.println(" Firing ");
    tone(13,38000);
    delay(2);
    noTone(13);
    delay(28);//original delay 28ms
    tone(13,38000);
    delayMicroseconds(200);
    noTone(13);
    delayMicroseconds(1500);
    tone(13,38000);
    delayMicroseconds(200);
    noTone(13);
    delayMicroseconds(3300);
    tone(13,38000);
    delayMicroseconds(200);
    noTone(13);

    delay(exp_delay_time);                         // Waits the set exposure delay time and repeats forever...

}



void exp_delay_set_test() {
  set_wait = 0;
  while (set_wait < 100) {
    if (digitalRead(set_button) == HIGH) {
      set_wait = set_wait + 10;
      delay(10);
    }
    if (digitalRead(set_button) == LOW) {
      exp_delay = exp_delay + 1;
      set_wait = 100;
    }
  }
}
