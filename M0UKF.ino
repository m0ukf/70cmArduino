// include the library code:
#include <LiquidCrystal.h>
#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {29, 28, 27, 26}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {25, 24, 23, 22}; //connect to the column pinouts of the keypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
unsigned long previousMillis = 0;   
const long interval = 200;  
const int ledPin =  3;
int sensorPinX = A0;    // select the input pin for the potentiometer
int sensorPinY = A1;
int sensorValueX;
int sensorValueY;
int lcdhidden = 0;
const int buttonPinA = 2;     // the number of the pushbutton pin
const int buttonPinB = 4;     // the number of the pushbutton pin
const int buttonPinC = 6;     // the number of the pushbutton pin
const int buttonPinD = 5;     // the number of the pushbutton pin
int flashenabled=0;
int buttonStateA = 0;         // variable for reading the pushbutton status
int buttonStateB = 0;        // variable for reading the pushbutton status
int buttonStateC = 0;        // variable for reading the pushbutton status
int buttonStateD = 0;        // variable for reading the pushbutton status
int screenmode;  //Work out if in menu/vfo etc..
String toptext; //Text that appears on top line of LCD
String bottomtext; //Text that appears on bottom line of LCD
String KString;
String prebuild; //String used to convert frequency to 4xx.xxxx even when value under 000
double frequency; //manages frequency
int menuoption;
int pttpressed=0;
int leftin;
int rightin;
int upin;
int downin;
int volume;
int TxCTCSS =1;
double CTCSSOptions[] = {67, 69.3, 71.9, 74.4, 77, 79.7, 82.5, 85.4, 88.5, 91.5, 94.8, 97.4, 100,103.5, 107.2, 110.9, 114.8, 118.8, 123, 127.3, 131.8, 136.5, 141.3, 146.2, 151.4, 156.7, 162.2, 167.9, 173.8, 179.9, 186.2, 192.8, 203.5, 210.7, 218.1, 225.7, 233.6, 241.8, 250.3};
int maxctcssoptions = 39;
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

void setup() {
   Serial.begin(9600);
   pinMode(buttonPinA, INPUT);
   pinMode(buttonPinB, INPUT);
   pinMode(buttonPinC, INPUT);
   pinMode(buttonPinD, INPUT);
   screenmode = 0;
   menuoption = 0;
   leftin=0;
   rightin=0;
   downin=0;
   upin=0;
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
    pinMode(ledPin, OUTPUT);
  // sensorXback=1;
  // Print a message to the LCD.
 frequency = 3500;
 volume = 50;

}


void loop() {
  getbuttonstate();
  if (pttpressed == 0) {statemanager();}
  if (pttpressed == 1) {PTT();}
    delay(125);
}
void statemanager()
{
  if (screenmode==0) Welcome();

  if (screenmode==1) VFOMode();  
  
  if (screenmode==2) Menu();

  if (screenmode==3) KeypadDE();

  if (screenmode==4) CTCSSMode();



  
  char customKey = customKeypad.getKey();
  String OldString = KString;
  if (customKey) {

   KString =  KString + customKey;
  }
  
  if (KString.indexOf('C') > -1) {
    
    if(flashenabled == 0) {flashenabled = 1;KString = OldString;} else {KString = OldString; flashenabled = 0; lcdhidden = 0;}
  } else if (flashenabled == 0){KString = OldString;}
  
  if (KString.indexOf('D') > -1) KString = "";
  if (KString.indexOf('B') > -1) KString = OldString.substring(0,OldString.length()-1);
   
  
}

void PTT()

{
lcd.setCursor(0,0);
lcd.print ("  TRANSMITTING  ");
lcd.setCursor(0,1);
 lcd.print("Freq: 43" + prebuild + String(frequency) + " ");  
}

void CTCSSMode()

{
 
      if (rightin == 1) {TxCTCSS = TxCTCSS + 1;}
      
      if (leftin == 1) {TxCTCSS = TxCTCSS - 1;}
      if (TxCTCSS > maxctcssoptions) {TxCTCSS=0;}
      if (TxCTCSS < 0) {TxCTCSS=maxctcssoptions;}
    


     lcd.setCursor(0,0);
 lcd.print("CTCSS: " + String(CTCSSOptions[TxCTCSS]) + "Hz        ");
 
 
 lcd.setCursor(0,1);
 
  lcd.print("Press > To Exit   ");  

  
}

void KeypadDE()
{

  unsigned long currentMillis = millis();

if (flashenabled ==1 ){
   if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (lcdhidden == 1) {
      lcdhidden = 0;
    } else {
      lcdhidden = 1;
    }

  }
  }
  
    if (lcdhidden==0){

     lcd.setCursor(0,0);
 lcd.print("Keypad: " + KString + "           " );
 
  }

 lcd.setCursor(0,1);
 
  lcd.print("  M0UKF  Radio  ");  
  if (lcdhidden==1){

     lcd.setCursor(0,0);
      lcd.print("                " );
 
  }

}

void Welcome()

{

lcd.setCursor(0,0);
 
  lcd.print("    Welcome!    ");  
  
  lcd.setCursor(0,1);
 
  lcd.print("  M0UKF  Radio  ");  
 
  
}

void Menu()
{

String displayoutput;
    displayoutput = "Menu Option: " + String(menuoption);
    if (rightin == 1) {
    menuoption = menuoption + 1;
    if (menuoption > 5) {menuoption=1;} 

    }
    if (leftin == 1) {
    menuoption = menuoption - 1;
    if (menuoption < 1) {menuoption=5;}  
    }
 
 if (menuoption == 0){displayoutput = "   << Menu >>   "; } 
 if (menuoption == 1){displayoutput = " CTCSS Settings ";}
 if (menuoption == 2){displayoutput = "   TxRx Offset   ";}  
 if (menuoption == 3){displayoutput = "Offset Direction";}
 if (menuoption == 4){displayoutput = "     Squelch    ";}
 if (menuoption == 5){displayoutput = "   Menu Item 5  ";}
  if (buttonStateD == HIGH){
    if (menuoption == 1)  screenmode=4 ;}

 lcd.setCursor(0,0);
 lcd.print(displayoutput);
  
  lcd.setCursor(0,1);
 
  lcd.print("  M0UKF  70cm   ");  
 
}

void getbuttonstate()

{
  buttonStateA = digitalRead(buttonPinA);
  buttonStateB = digitalRead(buttonPinB);
  buttonStateC = digitalRead(buttonPinC);
  buttonStateD = digitalRead(buttonPinD);
  sensorValueX = analogRead(sensorPinX);
  sensorValueY = analogRead(sensorPinY);

  if (buttonStateA == HIGH){
 screenmode = screenmode - 1; 
 if (screenmode < 0) screenmode = 3;
if (screenmode > 3) screenmode = 0;}
  if (buttonStateB == HIGH){
 screenmode = screenmode + 1;
 if (screenmode < 0) screenmode = 3;
if (screenmode > 3) screenmode = 0;}
 if (buttonStateC == HIGH){
    pttpressed = 1;
  } else {pttpressed=0;}


 //start sampling anal stick
 upin=0;
 rightin=0;
 leftin=0;
 downin=0;
 
    if (sensorValueX > 800) 
    {
      leftin=1;
    }
    if (sensorValueX < 200) {
      rightin=1;
    }
   
    if (sensorValueY > 800) 
    {
      upin =1;
    }
    if (sensorValueY < 200) {
      downin=1;
    }
    
    

}

void VFOMode(){
  

  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0,0);
  prebuild = "";
  if (frequency < 1000) prebuild = "0";
  if (frequency < 100) prebuild = "00";
  if (frequency < 10) prebuild = "000";
  
  lcd.print("Freq: 43" + prebuild + String(frequency) + " ");  
  lcd.setCursor(0, 1);

  
   
    if (leftin == 1) 
    {
      frequency = frequency - 12.5;
    }
    if (rightin == 1) {
      frequency = frequency + 12.5;
    }
  
  if (frequency > 9987.5) frequency = 9987.5;
  if (frequency < 000) frequency = 0;
  
   
    if (upin == 1) 
    {
      volume = volume + 1;
    }
    if (downin == 1) {
      volume = volume - 1;
    }
  
  if (volume > 100) volume = 100;
  if (volume < 0) volume = 0;
 
  bottomtext = ("Volume: " + String(volume)+"       ");
  

  
  lcd.setCursor(0,1);
  lcd.print (bottomtext);

  }
