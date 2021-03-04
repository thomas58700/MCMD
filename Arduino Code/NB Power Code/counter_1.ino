void setup() {
  Serial.begin(9600);
}

//==========Global Variable==============//

// Tracks the time since last event fired
const int MaxTime = 4294967295 ; //ms
int SampleInterval_1= 300000 ;// 5 min
unsigned long DeltaT1;
unsigned long CurrentReading1=0;
unsigned long PreviousReading1 =0;

int SampleInterval_2= 10000;
unsigned long DeltaT2;
unsigned long CurrentReading2=0;
unsigned long PreviousReading2 =0;

// unsigned long * p_DeltaT1 = &DeltaT1;
// unsigned long * p_CurrentReading1= &CurrentReading1;
// unsigned long * p_PreviousReading1 = &PreviousReading1;
//
// unsigned long * p_DeltaT2 = &DeltaT2;
// unsigned long * p_CurrentReading2 =&CurrentReading2; 
// unsigned long * p_PreviousReading2 =&PreviousReading2;

//==========================================================
//function returns DeltaTd time
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


}


void loop() {
  DeltaT1=getDeltaT(CurrentReading1,PreviousReading1);
//  Serial.println(DeltaT1);
 DeltaT2=getDeltaT(CurrentReading2,PreviousReading2);
  
  //===================================================

// wait until 5 sec DeltaT
  if(DeltaT1 > SampleInterval_1){
    //do something
    Serial.println(" 5 sec is up");
    Serial.println(DeltaT1);
    PreviousReading1 = millis();
  }
// wait until 10 sec DeltaT
//    if(DeltaT2 > SampleInterval_2){
//    //do something
//    Serial.println("10 sec is up");
//    Serial.println(DeltaT2);
//    PreviousReading2 = millis();
//  }

// Get snapshot of time
CurrentReading1 = millis();
//Serial.print("Current Reading1: ");
//Serial.println(CurrentReading1);
delay(10);
CurrentReading2 = millis();
//Serial.print("Current Reading2: ");
//Serial.println(CurrentReading2);
delay(10);


} //end loop
