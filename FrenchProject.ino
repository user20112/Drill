/*
Mariem Hassan
Final Project Code
Finish Date: 4/23/18
Edited 5/9/18 MH
Edited WLF 5/10/18 spelling and reorg of pre-setup and added comments.
*/
//Function Declarations and Constants Declared
byte XGRAY; //byte to hold 8-bit gray code for X motor (on Port A)
byte YGRAY; //byte to hold 8-bit gray code for Y motor (on Port C)

//added for WLF's manual code
int  xmin = 20;  // X <0-49 mV - close to hard left; go fastest left
int  xdbl = 450; // X deadband low 2.4 V
int  xdbh = 550; // X deadband Hi 2.6 V
int  xmax = 1003; // x >4.95 v - close to hard right; go fastest right
int  ymin = 20;  // Y <0-49 mV
int  ydbl = 450; // Y deadband low 2.4 V
int  ydbh = 550;  // Y deadband Hi 2.6 V
int  ymax = 1003; // Y >4.95 v
int  minXusec = 2000;   // shortest X delay for manual mode
int  minYusec = 2200;   // shortest y delay for manual mode
// End WLF Manual mode variables

int xval;   //Joystick analog X (A3)
int yval;   //Joystick analog Y (A4)

int MinMotorDelay= 3000; // for all automated procedures. 

// int motorSpeedX; //unused determines motor speed for positive x direction
// int motorSpeedX1; //unused determines motor speed for neg x direction
// int motorSpeedY; //unused determines motor speed for positive y direction
// int motorSpeedY1; //unused determines motor speed for neg y direction
int read_LCD_buttons(); //reads buttons
int getKey();           //debounce problem

int holeDelay=3000;   //mSec between each drill
int value;            //get value for free drill
int getNumber();      //function for get value for free drill
int nholes;           //number of holes for free drill
int nrows;            //number of rows for free drill

void barebonesIC();   //Barebones 328P IC 
void freeDrill();     //free drill function
void horizontalIC();  //function for horizontal IC 16 pin
void megaShield();    // function for mega shield
void noMotor();       // null motor - does nothing!

void StepXCCW();      //move x i step ccw
void StepXCW();       //move x i step cw
void StepYCW();       //move y i step cw
void StepYCCW();      //move y i step ccw

void unoShield();     // function for uno shield
void verticalIC();    // function for vertical IC

void x100mils();      //x 100 mils cw
void x100mils1();     //x 100 mils ccw
void x160mils1();     //x 160 mil ccw
void x300mils();      //x 300 mils cw

void y100mils();      //y 100 mils cw
void y100mils1();     //y 100 mils ccw
void y300mils();      //y 300 mils cw
void y300mils1();     //y 300 mils ccw

int lcd_key     = 0;  // holds output from read_lcd_buttons
int adc_key_in  = 0;  // holds analog value read from keypad

#include <LiquidCrystal.h>

// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

////////////////////////////////////////////////////////////////////////////////////////////
void setup() {  
  Serial.begin(9600);
  lcd.begin(16, 2);              // 16 Col 2 Row LCD
  lcd.setCursor(0,0);
  lcd.print("Select is AUTO"); // print beginning message
  lcd.setCursor(0,1);
  lcd.print("Left is MANUAL");
  
  DDRA = B11111111; //data direction reg. Port A= output
  DDRC = B11111111; // data direction reg. Port C = output
  XGRAY = 0X33; //initial gray code byte for X motor = 00110011
  YGRAY = 0X33; // Initial gray code byte fot Y motor = 00110011 
}
/////////////////////////////////////////////////////////////////////////////////////////////
void loop() {

int x = analogRead(0);  //Read analog buttons


if (x<450)          //if pressed wrong button
 {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("NOT A CASE");
  lcd.setCursor(0,1);
void x300mils1(); //x neg 300 mils
  lcd.print("Press reset");
  delay(500000);
  }
  
  else if(x<650)                      // Manual Mode
  {                                   // Only way out is via the "RST"
  lcd.clear();
  lcd.print("MANUAL MODE");
  while(1){                           // Locked into this mode
    xval = analogRead(A3);            // Read Joystick X value 0-1023
    yval = analogRead(A4);            // Read joystick Y value 0-1023
    
    if (xval < xmin) {                // If Xstick Hard left
      StepXCCW();                     // move one step X CCW
      delayMicroseconds(minXusec);    // Minimum delay
    }
    
    if ((xval >= xmin) && (xval< xdbl)) { 
                                      // If Xstick normal left
      StepXCCW();                     // move one step X CCW
      delay(xval);                    // and delay according to joystick
    }
    
    if ((xval >= xdbl) && (xval < xdbh)){ 
    }                                 // if Xstick in deadband
      noMotor();                      //  Don't you move a bit
  }                                    // THIS SEEMS TO PAIR WITH WHILE 1!
  
    if ((xval >= xdbh) && (xval < xmax)) {
      StepXCW();                      // If Xstick ir normal right
                                      // move 1 step CW
      delay( 1026-xval);
    }
    
    if (xval >= xmax){                // If Xstick hard right
      StepXCW();                      // move 1 step CW
      delayMicroseconds(minXusec);    // and minimum delay.
    }
    
   // Done X per joystick
    
    // Do y per joystick              
    if (yval < ymin) {                // If Ystick hard down
      StepYCCW();                     // 1 step CCW
      delayMicroseconds(minYusec);    // and minimum Y delay
    }
    
    if ((yval >= ymin) && (yval< ydbl)) {
      StepYCCW();                     // If Ystick normal down
       delay(yval);;                  // Move ccw and delay per joystick
    }
    
    if ((yval >= ydbl) && (yval < ydbh)){
      noMotor();                      //If Ystick in deadband do noting
    }
    
    if ((yval >= ydbh) && (yval < ymax)) 
    {
      StepYCW();                      // if Ystick normal up
      delay(1026-yval);               // Step once and calculated delay
    }
    
    if (yval >= ymax){                // If Ystick hard-up
      StepYCW();                      // step once CW
      delayMicroseconds(minYusec);    // with minimal delay
    }
  }                                   // END OF MANUAL MODE
  

//automated modes
else if ( x<850)
  {
    while(1){                       //always stays in this loop
     lcd.setCursor(0,0);
     lcd.print("Up 4 templates");   // print a message
     lcd.setCursor(0,1);
     lcd.print("Down 4 free dril");
   getKey();                        //debounce
   int c = analogRead(0);
   if (c<250){                      //pressed templates button
     while(1){
     lcd.clear();
     lcd.setCursor(0,0);
     lcd.print("Which case would"); // print a message
     lcd.setCursor(0,1);
     lcd.print("u like to drill?");
      
     getKey();
     lcd_key = read_LCD_buttons();  // read the buttons

     switch (lcd_key){
       case btnRIGHT:              //vertical ic function
         { 
         lcd.clear();
         lcd.print("vertical IC");
         lcd.setCursor(0,1);
         lcd.print("16 PIN");
         verticalIC();              // Do vertical 16 pin IC
         break;
         }
       case btnLEFT:                //horizontal IC function
         { 
         lcd.clear();
         lcd.print("Horizontal IC");
         lcd.setCursor(0,1);
         lcd.print("16 PIN");
         horizontalIC();            // Do Horizonal 16 Pin IC
         break;
         }
       case btnUP:                  //uno shield function
         { 
         lcd.clear();
         lcd.print("UNO SHIELD ");
         unoShield();               // DO UNO shield
         break;
         }
       case btnDOWN:                //mega shield function
         { 
         lcd.clear();
         lcd.print("MEGA SHIELD ");
         megaShield();              // DO Mega shield
         break;
         }
       case btnSELECT:              // Bare Bones 28 pin IC
         { 
         lcd.clear();
         lcd.print("Barebones IC");
         barebonesIC();
         break;
         }
         case btnNONE:
         {
         while (lcd_key==btnNONE){
          lcd_key = read_LCD_buttons(); //always read buttons
         }
         break;
         }
      }
    }     // While 1          WHY NO {
   }      //If C<250                //if pressed for free drill
   
    else if(c<450){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("# of holes?");       //gathers number of holes
    getNumber();                    //calls get number
    nholes = value;                 //stores value
    value = 0;                      //set to zero when done
    lcd.clear(); 
    lcd.print("# of rows?");        //gathers number of rows
    getNumber();                    //gets number
    nrows = value;                  //store value
    freeDrill();                    //goes to free drill
     }  
    }             // Another while 1
    }
   }              // Void Loop


///////////////////////////////////////////////////////////////// Here are the procedures:
// read the buttons
int read_LCD_buttons()
{
 adc_key_in = analogRead(0);      // read the value from the sensor 
 // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
 // we add approx 50 to those values and check to see if we are close
 if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 250)  return btnUP; 
 if (adc_key_in < 450)  return btnDOWN; 
 if (adc_key_in < 650)  return btnLEFT; 
 if (adc_key_in < 850)  return btnSELECT;  
 return btnNONE;  // when all others fail, return this...
}

// Move X clockwise direction
void StepXCW(){
  XGRAY = XGRAY >> 1 | XGRAY <<7;
  PORTA =XGRAY;
  //delayMicroseconds (MinMotorDelay);
}

//Move X counter clockwise direction
void StepXCCW(){
  XGRAY = XGRAY << 1 | XGRAY >>7;
  PORTA = XGRAY;
  //delayMicroseconds (minXusec);
}

//Move Y clockwise direction
void StepYCW(){
  YGRAY = YGRAY >>1 | YGRAY <<7;
  PORTC = YGRAY;
  //delayMicroseconds (minYusec);
}

//Move Y counter clockwise direction
void StepYCCW(){
  YGRAY = YGRAY <<1 | YGRAY >>7;
  PORTC = YGRAY;
  //delayMicroseconds (minYusec);
}

//motor is not moving
void noMotor()
{
  //motor is not moving
}

void x100mils(){    //makes motor go clockwise 100 mils x direction
  for(int i=0; i<400; i++)    // 400 steps = 100 mils
  {
    StepXCW();
    delayMicroseconds(MinMotorDelay);
  }
}

void x100mils1(){    //makes motor go ccw 100 mils x direction
  for(int o=0; o<400; o++)   //400 steps
  {
    StepXCCW();     
    delayMicroseconds(MinMotorDelay);
  }
}

void y100mils(){  //makes motor go clockwise 100 mils y direction
  for(int i=0; i<400; i++)    // 400 steps
  {
    StepYCW();
    delayMicroseconds(MinMotorDelay);
  }
}

void y100mils1(){  //makes motor go c-clockwise 100 mils y direction
  for(int o=0; o<400; o++)   //400 steps
  {
    StepYCCW();
    delayMicroseconds(MinMotorDelay);
  }
}

void x300mils(){  //makes motor go clockwise 300 mils x direction
  for(int i=0; i<1200; i++)    // 1200 steps
  {
    StepXCW();
    delayMicroseconds(MinMotorDelay);
  }
}

void x300mils1(){  //makes motor go c-clockwise 300 mils x direction
  for(int o=0; o<1200; o++)   //1200 steps
  {
    StepXCCW();
    delayMicroseconds(MinMotorDelay);
  }
}

void y300mils(){   //makes motor go clockwise 300 mils y direction
  for(int i=0; i<1200; i++)    // 1200 steps
  {
    StepYCW();
    delayMicroseconds(MinMotorDelay);
  }
}

void y300mils1(){  //makes motor go c-clockwise 300 mils y direction
  for(int o=0; o<1200; o++)   //1200 steps
  {
    StepYCCW();
    delayMicroseconds(MinMotorDelay);
  }
}

void x160mils1(){  //makes motor go c-clockwise 160 mils x direction
  for(int i=0; i<640; i++) //640 steps
  {
    StepXCCW();
    delayMicroseconds(MinMotorDelay);
  }
}

void verticalIC(){ //vertical IC shield
  delay(holeDelay);
  for(int i=1; i<8; i++) //makes 8 holes 
  {
    y100mils(); //y direction
    delay(holeDelay); //delay
  }
  x300mils(); //move 300 mils x direction
  delay(holeDelay); //delay
  for(int i=1; i<8; i++)  //make 8 holes 
  {
    y100mils1();  //other y direction
    delay(holeDelay);  //delay
  }
}

void horizontalIC(){   //horizontal IC shield
  delay(holeDelay);
  for(int i=1; i<8; i++)
  {
    x100mils1(); //start in top left corner, goes to the right
    delay(holeDelay);
  }
  y300mils(); //goes down 300 mils
  delay(holeDelay);
  for(int i=1; i<8; i++)
  {
    x100mils(); // goes to the left
    delay(holeDelay);
  }
}

void unoShield(){  //uno shield function
  delay(holeDelay);
  for(int i=1; i<10; i++)
  {
    x100mils1();
    delay(holeDelay);
  }
  x160mils1();
  delay(holeDelay);
  for(int o=1; o<8; o++)
  {
    x100mils1();
    delay(holeDelay);
  }
  
  for(int h=0; h<19; h++)
  {
    y100mils();
  }
  delay(holeDelay);
  for(int y=1; y<6; y++)
  {
    x100mils();
    delay(holeDelay);
  }
  for(int w=1; w<2; w++)
  {
  x100mils();
  }
  for(int r=0; r<8; r++)
  {
    x100mils();
    delay(holeDelay);
  }
}                   // End UNO shield

void megaShield(){  //mega shield function
  delay(holeDelay);
  for(int i=1; i<8; i++)    // 7 more holes in x  100 mil
  {
    x100mils1();           
    delay(holeDelay);
  }
  x160mils1();              // leave 160 mil gap        
  delay(holeDelay);
  for(int o=1; o<8; o++)    // do 8 holes 100 mil
  {
    x100mils1();
    delay(holeDelay);
  }
  for(int z=0; z<2; z++)    // Do 3 holes 
  {
    x100mils1();
  }
  delay(holeDelay);
  for(int y=1; y<8; y++)
  {
    x100mils1();
    delay(holeDelay);
  }
  x300mils1();
  delay(holeDelay);
  for(int e=1; e<18; e++)
  {
    y100mils();
    delay(holeDelay);
  }
  x100mils1();
  delay(holeDelay);
  for(int d=1; d<18; d++)
  {
    y100mils1();
    delay(holeDelay);
  }
  for(int s=1; s<20; s++)
  {
    y100mils();
  }
  for(int q=0; q<2; q++)
  {
    x100mils();
  }
  delay(holeDelay);
  for(int w=0; w<3; w++)
  {
    for(int c=1; c<8; c++)
    {
      x100mils();
      delay(holeDelay);
    }
    for(int x=0; x<2; x++)
    {
      x100mils();
    }
    delay(holeDelay);
  }
}

int getKey(){  //button debounce
  int c;
  int b;
  do{
  c = read_LCD_buttons();
  delay(100);
  b = read_LCD_buttons();
  if(c=b){
    return c;
} 
}
while (c!=b);
}

int getNumber()
{  //get value from user function
  lcd.setCursor(0,1);
  lcd.print("0");
  bool d = false;
  while(d == false)
   {
    getKey();
    int x = analogRead(0);
    if (x<250)
    {
      value++; //increment 
      lcd.setCursor(0,1);
      lcd.print(value);
    }
    else if (x<450)
    {
      value--; //decrement
      if(value<0)
      {
        value=0;
      }
      lcd.setCursor(0,1);
      lcd.print(value);
    }
    else if (x<850)
    {
     d = true;
    }
   }   
}

void freeDrill()  //free drill function
{
  lcd.clear();
  while(1){
    lcd.print("SPACING:300=down");
    lcd.setCursor(0,1);
    lcd.print("600=left, 100=up");
    getKey();
    int c = analogRead(0);
    if (c<250)  //for 100 between row drill
    {
      lcd.clear();
      lcd.print("100 mil drill");
      lcd.setCursor(0,1);
      lcd.print("between rows");
      delay(holeDelay);
      for(int r=1; r<nrows; r++)
      { 
        for(int t=1; t<nholes; t++)
        {
          x100mils();
          delay(holeDelay);
        }
        for(int y=1; y<nholes; y++)
         {
           x100mils1();
         }
        y100mils();
        delay(holeDelay);
      }
      lcd.clear();
    }
    else if(c<450)   //for 300 between row drill
    {
      lcd.clear();
      lcd.print("300 mil drill");
      lcd.setCursor(0,1);
      lcd.print("between rows");
      delay(holeDelay);
      for(int r=1; r<nrows; r++)
      { 
        for(int t=1; t<nholes; t++)
        {
          x100mils();
          delay(holeDelay);
        }
        for(int y=1; y<nholes; y++)
         {
           x100mils1();
         }
        y300mils();
        delay(holeDelay);
      }
      lcd.clear();
    }
    else if(c<650)   //for 600 between row drill
    {
      lcd.clear();
      lcd.print("600 mil drill");
      lcd.setCursor(0,1);
      lcd.print("between rows");
      delay(holeDelay);
      for(int r=1; r<nrows; r++)
      { 
        for(int t=1; t<nholes; t++)
        {
          x100mils1();
          delay(holeDelay);
        }
        for(int y=1; y<nholes; y++)
         {
           x100mils();
         }
         for(int t=0; t<2; t++)
         {
        y300mils();
         }
        delay(holeDelay);
      }
      lcd.clear();
    }
  }
}

//Function to drill a Barebones Atmel 328P IC 
// 28 pins 300 mil rows
void barebonesIC()
{
  delay(holeDelay);
  for(int i=1; i<14; i++)
  {
    x100mils();
    delay(holeDelay);
  }
  y300mils();
  delay(holeDelay);
  for(int r=1; r<14; r++)
  {
    x100mils1();
    delay(holeDelay);
  }
}
