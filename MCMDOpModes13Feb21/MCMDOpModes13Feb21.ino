/////////////////Simulation Cases//////////////////////////
float current = 1; //1000 Amps
float temp=100; //simulated to be higher than 85
int FunctionalDelay=1440; // minutes
int ReducedDelay=720;
int TR3=0;    //Simulates operation mode 

/////////////////////////////////////////////////////////////
//Initialization of Global Variables 
float MaxTemp=85;           //Trigger Safety Mode at this temperature. 
float LowCThresh=2;
float HighCThresh=1500;
int lowcount=0;           //Amount of sample where the current<2A.
int Reported = 0;           //Flag to indicate when low current event is reported.
int * pLowcount=&lowcount;    
int * pReport=&Reported;
int LEDState=0;           //LED currently initialized to Functional Mode.
int * pLED= &LEDState; //Pointer to indicate which LED state is used. May need to be merged with Thomas's way. 
float TxDelay=720; // Can be changed for testing, but should be used to initalize the tx delay time. 
float * p2= &TxDelay;    //Pointer to the Delay between transmissions. 


//Initialization of Serial Printing
void setup(){
 Serial.begin(9600);
}
/////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////
//Temperature Control Function 
//Purpose: Prevents MCMD from operating while Temp>MaxTemp with inf-loop. 
//Input: None
//Output: None

void TempControl(){
  while(temp>MaxTemp){
  //Temp=analogread(temp);   //Add Thanh's Current Grab Here
                             //Add any Interrupts if necessary
  //Slow Red LED blink to indicate temp problem?

 //////////////////
 //Test Case Only//
 // Serial.println("Checking Temp"); 
 //   delay(3000); //In place of reading and waiting. 
 //   temp=60; //simulating temp drop
 //////////////////
    }
return;
}
////////////////////////////////////////////////////////////////
// MCMD Operational Modes
// Purpose: Set all required pointers to change operation modes. 
// Input: None
// Output: None

void FunctionalMode(){
  *pLED=1;
  *p2=FunctionalDelay;
  return;
}

void ReducedMode(){
  *pLED=2;
  *p2=ReducedDelay;
  return;
}

void ShutdownMode(){
  *pLED=3;
    while(TR3 == 0){   //while below 25% //Will have to be updated based on our new SMC. 
    
    //do nothing, except charge.

      ///////////////////Test Case Only//////////////////
      // delay(2000); //delay to simulate waiting      //
      // TR3=1;                                        //
      // Serial.print("Battery has charged above 25%");//
      ///////////////////////////////////////////////////
    }
  ReducedMode(); // Return to Reduced Mode once charged above 25%
  return;
}

void SafetyMode(){
  //switch(6);     //Hard coded the flash so it returns to its last state after. 
  //SendSafetyMessage();
    Serial.println("Entering Safety Mode"); 
  delay(6000);  //60s delay (Stepped down to 6s for TinkerCAD
  TempControl();  //Call another function to wait for temp to drop. 
}

/////////////////////////////////////////////////////////////
//Main Loop 
void loop(){

  
  //While loop for reading current, interrupts will be needed to break the loop and do something else. 
  //If all of this can be done under 1s, then we can add a delay to wait for 1s before taking next sample. 
while(1) {
//analogread(x);   //Thanh's magic ACS read. 
//storedata(); //SD Card Functionality 

if(current >= HighCThresh){
  SafetyMode();
  *pLowcount=0;  //Reset, no low current. 

  ///////////TEst Case Only ////////////
    Serial.println("High Current"); //
    delay(2000);                    //
  //////////////////////////////////////
}

else if(current < LowCThresh && !Reported){
    Serial.println("Low Current");
    *pLowcount +=1; //Count how many times it is low
    Serial.println(*pLowcount); //Track how many consecutive samples were too low
  
      if(*pLowcount >= 60){  // && !report //If low for 60 samples (=60 seconds)
    //SendLowCurrentNotice
    *pReport = 1;
    //InitateRMScalc and prep data for tx.
    //prepfortx(); 
        Serial.println("One Minute of Low Current");
        delay(2000);
    } 
}
  
else if(current < LowCThresh && Reported){
  while(current<LowCThresh){ //Wait until current rises above the low threshold.
  //Read current
    Serial.println("Incident Reported");
    delay(5000); //
  }
  }
  
else{  //current is healthy
  *pLowcount=0;
     Serial.println("Current is healthy");
      delay(2000);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
}
