/************************************************
  * Author: Eloy Salinas
  * Project: Pet Water Warden
  * Company: Make Magazine // RadioShack
  * Date: 8/8/13
  * Version: 1.0
  * http://makezine.com/projects/petwaterwarden/
  *************************************************/
  //Twitter capabilities thanks to http://arduino-tweet.appspot.com/
  
  
  //Ethernet Shield Version
 
  #include <SPI.h>
  #include <Ethernet.h>    // this allows us to use the Ethernet shield easily
  #include <Twitter.h>
 
  //Twitter info
  // Your Token to Tweet (get it from http://arduino-tweet.appspot.com/)
  Twitter twitter("Your-Token-Here");
 
  // Message to post to twitter in case of a fail
  char msg[] = "Pet Water Warden Error: Please check my water!";
 
  //Network information (Only edit if not using default mode).
  //Default mode is Auto DHCP, your router will assign an ip address.
  //Only fill in ip, gateway, and subnet if not using auto DHCP
  byte mac[] = {
    0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // this can be made up
  byte ip[] = {
    0, 0, 0, 0 }; // a free IP address on your network
  byte gateway[] = {
    0, 0, 0, 0 }; // the gateway address of your network
  byte subnet[] = {
    0, 0, 0, 0 };  // the subnet mask of your network
 
  //Set output pin 8 to control switchtail/pump
  const int pump = 8;
 
  //Failsafe off
  long timerA = 0;
 
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
 
  //Set pump output pin
  pinMode(pump, OUTPUT);
 
  //Start with pump off
  digitalWrite(pump, LOW);
 
  
 
  //If not using auto DHCP use Ethernet.begin(mac, ip, gateway, subnet);
  Ethernet.begin(mac);  //begins the Ethernet connection and uses automatic DHCP
  Serial.begin(9600);  // starts serial communications so we can debug easier
  delay(1000);        // a 1 second delay to let everything settle down!
 
 
 
  //Test Twitter
  Serial.println("Pet Water Warden testing twitter connection...");  
 
  if (twitter.post("Pet Water Warden is up and running!!")) {  // Twitter that we are up and running
    int status = twitter.wait(&Serial);     // wait for a response from twitter                    
    if (status == 200) {    // if Twitter responds 200
      Serial.println(", Tweet is OK!");  // print success
      Serial.println();  // print a blank line, used for debugging
    }
    else {
      Serial.print("Tweet failed : code ");
      Serial.println(status);  // print error code
      Serial.println();  // print a blank line, used for debugging
    }
  }
  else {
    Serial.println("Connection to Twitter failed.");
  }
 
}
 
 
void loop(){
 
  //Read the input on A0-A1
  //High and Low Sensors
  int sensorLow = analogRead(A1);
  int sensorHigh = analogRead(A0);
 
  //Convert to a voltage
  float voltageLow = sensorLow * (5.0 / 1023.0);
  float voltageHigh = sensorHigh * (5.0 / 1023.0);  
 
  //Sensor States
  int lowState = 0;
  int highState = 0;
 
  //Are the sensors on or off?
  //Write states, Active Low
  if (voltageLow >= 3.5){lowState = 0;}
  else if (voltageLow < 3.5){lowState = 1;}
 
  if (voltageHigh >= 3.5){highState = 0;}
  else if (voltageHigh < 3.5){highState = 1;}
 
  //Turn on the pump?
  if(highState == 1 && lowState == 1){
    digitalWrite(pump, LOW);
    timerA = 0;  
  }else if(highState == 0 && digitalRead(pump) == LOW){
    //FailSafe Timers
    timerA = 0;
    digitalWrite(pump, HIGH);
    timerA = millis();
    Serial.print("Starting timer: ");    
    Serial.println(timerA);
  }
 
  //My pet bowl fills in about 45 sec, adjust to the size
  // of your pet bowl
  if( (millis() - timerA) >= 45000 && timerA != 0){
    digitalWrite(pump, LOW);  
    Serial.println(timerA);
    Serial.println(millis());
    timerA = 0;
    //Either no water left or the pump didn't turn off, bad sensor?
    tweetFail();
    Serial.println("Either no water left or the pump didn't turn off, bad sensor?");
  }
 
 
  //Debug Prints
  Serial.print("Low Sensor: ");
  Serial.println(lowState);
  Serial.print("High Sensor: ");
  Serial.println(highState);
 
  //Check Sensors every 10 sec
  delay(10000);
 
 
}
 
 
void tweetFail(){
    if (twitter.post(msg)) {  // Twitter that we are up and running
    int status = twitter.wait(&Serial);     // wait for a response from twitter                    
    if (status == 200) {    // if Twitter responds 200
      Serial.println(", Tweet is OK!");  // print success
      Serial.println();  // print a blank line, used for debugging
    }
    else {
      Serial.print("Tweet failed : code ");
      Serial.println(status);  // print error code
      Serial.println();  // print a blank line, used for debugging
      //If Tweet fails try again
      tweetFail();
    }
  }
  else {
    Serial.println("Connection to Twitter failed.");
  }
}
