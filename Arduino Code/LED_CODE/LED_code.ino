void setup() {
  Serial.begin(9600);
  pinMode(13,OUTPUT);
}

///NEED to the 5 required cases


//Global Variables
  //  int LED1USE=0;
  // int LED1ON=0;
  unsigned  long int CurrentTimeLED1;
   unsigned long int PreviousTimeLED1;
   unsigned long int DeltaTLED1;
 //  unsigned long int IntervalLED1=5000; //5s
   
const int MaxTime = 4294967295 ; //ms

    int LED1USE=1;  //This one needs to be used.
    int LED1ON=0; //This one is currently OFF.


// Example 2
//    long int IntervalLED1=500; //0.5s //Since ONtime=OFftime, this can be used for both.
//    int LED1Cycles=2; //ON-OFF Twice
//    int LED1Count=0; //Amount of time LED was cycled.
// Example 3
    long int IntervalLED1ON=200; //0.2s 
    long int IntervalLED1OFF=800;//0.8s
    int LED1Cycles=5; //ON-OFF Twice
    int LED1Count=0; //Amount of time LED was cycled.

    
/////////////////////////////////////////////////////

//function returns DeltaT time
unsigned long getDeltaT(unsigned long CurrentReading,unsigned long PreviousReading){
 
  //unsigned long DeltaT;
  
  if(CurrentReading<PreviousReading){
    // Time Elaspe 
   unsigned long  DeltaT = (MaxTime - PreviousReading)+CurrentReading;
   return(DeltaT);
  }

  else{
 unsigned long   DeltaT =CurrentReading - PreviousReading;
 return(DeltaT);
  }


}//end of getDeltaT



//////////////////////////////////////////////////////////

void loop() {
 
//    // put your main code here, to run repeatedly:
//    //Making LEDs blink when needed without delay.
//    
//    //1)Each LED-Type will require a:
//    //a) DeltaT(NAME) to determine how long its been on.
//    //b) CurrentTime(NAME) & PreviousTime(Name) for DeltaT(NAME)
//    //c) Interval(NAME) to set how long it blinks, if it blink irregularili, it will need several.
//    
//    //Example 1: Blink the LED for 5s when booting up.
//    //This IF statement must be placed where the event occurs that requires LED use.
// 
//    if(LED1USE){    //LED1ON means the LED1 must be turned ON/OFF
//    if(!LED1ON){
//        digitalWrite(13,HIGH); //Turn ON.
//        LED1ON=1; //This one is now ON.
//        PreviousTimeLED1 = millis(); //Represents the time it went ON.
//    }
//    
//     //Is it time to turn off?
//          CurrentTimeLED1 = millis();
//          DeltaTLED1=getDeltaT(CurrentTimeLED1,PreviousTimeLED1);
//     
//            if(DeltaTLED1 > IntervalLED1){
//                  digitalWrite(13,LOW); //Turn OFF.
//                  delay(1000);
//                          PreviousTimeLED1 = millis();
//                          Serial.println("LED go OFF");
//                //  LED1USE=0; //This one doesnt need to be used anymore.
//                  LED1ON=0;  //Mark state as OFF
//            }
//    }
//=================================================================
    //Example 2: Blink the LED with two pulses with 1s period at 50% duty. (ON 0.5s, OFF 0.5s, ON 0.5s, OFF 0.5s)

//  
//    if(LED1USE){    //LED1ON means the LED1 must be turned ON/OFF
//          if(!LED1ON && !LED1Count){ //On the first time, dont need to check time before turning back on.
//              digitalWrite(13,HIGH); //Turn ON.
//              PreviousTimeLED1 = millis(); //Represents the time it went ON.
//              LED1ON=1; //This one is now ON.
//                 Serial.print("LED Go ON 1");
//             }
//          else if(!LED1ON){ //On the second time, need to check time before turning back on.
//                CurrentTimeLED1 = millis();
//                DeltaTLED1=getDeltaT(CurrentTimeLED1,PreviousTimeLED1);//Check how long its been OFF.
//                  if(DeltaTLED1 > IntervalLED1){
//                      digitalWrite(13,HIGH); //Turn ON.
//                      PreviousTimeLED1 = millis(); //Mark when it went ON
//                      //Serial.println("LED go ON");
//                      LED1ON=1; //Mark LED as OFF, but not USE=0
//                      Serial.print("LED Go ON 2");
//                      //LED1Count+=1; //Count how many times the LED was turned ON and then OFF.
//                     }
//          }
//          
//           //Is it time to turn off?
//                CurrentTimeLED1 = millis();
//                DeltaTLED1=getDeltaT(CurrentTimeLED1,PreviousTimeLED1);
//                Serial.print("Checking to go off");
//           
//                  if(DeltaTLED1 > IntervalLED1){
//                      digitalWrite(13,LOW); //Turn OFF.
//                              PreviousTimeLED1 = millis();
//                              Serial.println("LED go OFF");
//                      LED1ON=0; //Mark LED as OFF, but not USE=0
//                      LED1Count+=1; //Count how many times the LED was turned ON and then OFF.
//                  }
//      // cycle is set to 2 
//              if(LED1Count == LED1Cycles){
//                 LED1USE=0; //Stop using this LED
//                 Serial.println("Ending program");
//              }
//          }
  //=============================================================================
  // Example 3
  
    if(LED1USE){    //LED1ON means the LED1 must be turned ON/OFF
          if(!LED1ON && !LED1Count){ //On the first time, dont need to check time before turning back on.
              digitalWrite(13,HIGH); //Turn ON.
              PreviousTimeLED1 = millis(); //Represents the time it went ON.
              LED1ON=1; //This one is now ON.
                 Serial.println("LED Go ON 1");
             }
          else if(!LED1ON){ //On the second time, need to check time before turning back on.
                CurrentTimeLED1 = millis();
                DeltaTLED1=getDeltaT(CurrentTimeLED1,PreviousTimeLED1);//Check how long its been OFF.
                 
                  if(DeltaTLED1 > IntervalLED1OFF){
                      digitalWrite(13,HIGH); //Turn ON.
                      PreviousTimeLED1 = millis(); //Mark when it went ON
                      LED1ON=1; //Mark LED as OFF, but not USE=0
                      Serial.println("LED Go ON 2");
           
                     }
              }
          
           //Is it time to turn off?
           else if(LED1ON){
                  CurrentTimeLED1 = millis();
                  DeltaTLED1=getDeltaT(CurrentTimeLED1,PreviousTimeLED1);
                //  Serial.println("Checking to go off");
             
                    if(DeltaTLED1 > IntervalLED1ON){
                        digitalWrite(13,LOW); //Turn OFF.
                                PreviousTimeLED1 = millis();
                                Serial.println("LED go OFF");
                        LED1ON=0; //Mark LED as OFF, but not USE=0
                        LED1Count+=1; //Count how many times the LED was turned ON and then OFF.
                    Serial.println(LED1Count);
                    }
                  }
      // cycle is set to 2 
              if(LED1Count == LED1Cycles){
                 LED1USE=0; //Stop using this LED
                 Serial.println("Ending program");
              }
          }
      
}
