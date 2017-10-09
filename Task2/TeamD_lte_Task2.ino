// Microprocessor assignment

// Definition of pins
//----------------------------Input Buttons------------------------------------
const int button0 = 2;                          //Bottom button switches states
const int button1 = 3;                          //Top button 
// LED RGB Adjusts
const int red = 9;                              //PWM top pin that changes Red Value of LED
const int green = 10;                           //PWM middle pin that changes Green Value of LED
const int blue = 11;                            //PWM bottom pin that changes Red Value of LED
// Input Potentiometer Adjusts
const int potentiometer = A0;                   // Is connected to 10kOhm rotary potentiometer

// Important Definitions
//------------------Set Default Values for Inputs ----------------------------
int button0State = 0;
int button1State = 0;
int potentState = 0;
// Variables that count button press occurences (Initial values equal to 0)
int zeroCount = 0;
int oneCount = 0;
      
//----------------------------Initial State-----------------------------------
volatile int state = 0;

// This is the definition of RGB quantities from the Serial Event (set them to 100 each by default  - common anode so all 255,255,255 is off, whereas 0,0,0 = on)
int redValue = 100;
int greenValue = 100;
int blueValue = 100;

//-----------------------------Debouncing-------------------------------------
volatile int affirm_trigger;
volatile int affirm_trigger1;

//Define variable a which will be incremented with each time the button is pressed
// Light will be turned off or turned on depending on the divisibility wrt to 2 (even)
int a = 0;


void setup() {
  //----Input Pin Set-------
  pinMode(potentiometer, INPUT);
  
  pinMode(button0, INPUT);
  digitalWrite(button0, HIGH);
  
  pinMode(button1, INPUT);
  digitalWrite(button1, HIGH);

  attachInterrupt(digitalPinToInterrupt(button0), affirm, CHANGE);
  attachInterrupt(digitalPinToInterrupt(button1), affirm1, CHANGE);
  
  //----Output Pin Set------
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  Serial.begin(9600);   //9600 baud rate is standard
}


void affirm(){      //Used for Button0
  int button0State = digitalRead(button0);
  if (button0State == HIGH){   //Has been pressed
    // Increment the state by 1
    if (state != 2){
      state = state + 1;}
    else{
      state = 0;} // If current state is 2, next button should reset to state 0
    Serial.println(state);
    affirm_trigger = 1;
    }
  }


void affirm1(){       //Used for Button1
    // Digital read from pin connected to button1
    int button1State = digitalRead(button1);
    if (button1State == HIGH){   //Has been pressed
      a = a + 1;
      affirm_trigger1 = 1;
    }
  }
  
void loop() {
  if (affirm_trigger == 1){
    detachInterrupt(digitalPinToInterrupt(button0));
    delay (65);
    attachInterrupt(digitalPinToInterrupt(button0), affirm, CHANGE);
    
  }

  //------Proceed based on state--------
  
  if (state == 0){    //System is in state 0
    blinkLed();}
  
  if (state == 1){   //System is in state 1
    brightnessControl();}
  
  if (state == 2){   //System is in state 2
    serialCommands();}

  }



// -------- State Void Functions ------

void blinkLed(){

  if (affirm_trigger1 == 1){
    detachInterrupt(digitalPinToInterrupt(button1));
    delay (60);
    attachInterrupt(digitalPinToInterrupt(button1), affirm1, CHANGE);  
  }

  if (a % 2 != 0){  //If it is an odd number, then it must be switched on
    digitalWrite(red, HIGH);
    digitalWrite(green, HIGH);
    digitalWrite(blue, HIGH);}
    
  if (a % 2 == 0){ // If it is an even number, then it must be switched off
    digitalWrite(red, LOW);
    digitalWrite(green, LOW);
    digitalWrite(blue, LOW);}    
  }
  

  
void brightnessControl(){
  // Read the analog input from the rotary potentiometer
  potentState = analogRead(potentiometer);
  potentState = map(potentState, 0, 1023, 0, 255);
  analogWrite(red, potentState);
  analogWrite(green, potentState);
  analogWrite(blue, potentState);
  }




void serialCommands (){

    
    static char commandLine [5];  //Expect maximum of 5 characters per line(eg. b123(0)), ((0) = 0 null byte)
                                                //Null Byte terminates the line in the serial command
    for (int k = 0; k < 4; k++){commandLine[k] = 0;}
 
    while (Serial.available () > 0){

      static unsigned int currentPosition = 0;
      byte current = Serial.read();
      switch (current){
        //Normally
        default:
          // keep adding if not full ... allow for terminating null byte
          if (currentPosition < (4)){
            commandLine [currentPosition++] = current;
            if (strlen(commandLine[currentPosition]) != strlen(commandLine[currentPosition+1])){
              commandLine[currentPosition + 1] == 0;}
            }
          break;
          
        // When endline reached
        case '\n':   
          commandLine [currentPosition] = 0;  // fill current Position element with a null byte that terminates operation
          
          //Proceed to classifying instructions as either B, G or R
          String number;    //Holds the number that follows the r,g,b letter

          if (commandLine[0] == 'b'){
            blueValue = 0 ;

            bool spurious = false;
            for (int i = 1; i < 4; i++ ){
              
              //Spurious Input Detector
              if (isAlpha(commandLine[i]) || isWhitespace(commandLine[i]) || isPunct(commandLine[i])){
                spurious = true;}
              
              //RGB Number finder 
              if (isDigit(commandLine[i])){
                number += commandLine[i];}
              }//End of for loop

            //Only make use of non spurious input
            if (spurious == false){   

              //0,0,0 should be turned off, 255,255,255 should be turned on to max (white light)
              // Therefore the number should not be greater than 255 before abs(x-255) to it (x)
              if (number.toInt() <= 255){

                blueValue = number.toInt();
                analogWrite(blue, abs(blueValue-255));}
              }
            }//end of if 'b' first element

            
          if (commandLine[0] == 'g'){
            greenValue = 0;
            //Same thing as if command[0] == 'b' code so comments are not included
            bool spurious = false;
            for (int i = 1; i < 4; i++ ){
              if (isAlpha(commandLine[i]) || isWhitespace(commandLine[i]) || isPunct(commandLine[i])){
                spurious = true;}
              if (isDigit(commandLine[i])){
                number += commandLine[i];}
              }
            if (spurious == false){   
              if (number.toInt() <= 255){
                greenValue = number.toInt();
                analogWrite(green, abs(greenValue-255));}
            }
          }

          if (commandLine[0] == 'r'){
            redValue = 0;
            //Same thing as if command[0] == 'b' code so comments are not included
            bool spurious = false;
            for (int i = 1; i < 5; i++ ){
              if (isAlpha(commandLine[i]) || isWhitespace(commandLine[i]) || isPunct(commandLine[i])){
                spurious = true;}
              if (isDigit(commandLine[i])){
                number += commandLine[i];}
              }
            if (spurious == false){   
              if (number.toInt() <= 255){
                redValue = number.toInt();
                
                analogWrite(red, abs(redValue-255));}
            }
          }

          //Serial.println(redValue);
          //Serial.println(blueValue);
          //Serial.println(greenValue);
          currentPosition = 0;    //Reset current position along the byte where we are
          break; 
        }
    }
  }

