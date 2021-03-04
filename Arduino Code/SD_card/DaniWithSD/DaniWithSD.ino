/*//////////////Init////////////////////
// include the SD library:
#include <SD.h>
#include <SPI.h>
const int chipSelect = BUILTIN_SDCARD;

// set up variables using the SD utility library functions:
Sd2Card card;
SdVolume volume;
SdFile root;
//////////////Confirm Init//////////////
// see if the card is present and can be initialized:
if (!SD.begin(chipSelect)) {​​
Serial.println("Card failed, or not present");
// don't do anything more:
return;
}​​
Serial.println("card initialized.");
}​​
////////////End of INIT/////////////////*/



// It is simulating taking values from an array to calculate
// the average.

//float dummy[12]={988.50,994.15,886.53,798.55,999.34,543.55,656.70,700.05,854.09,991.5,150.87,547.98};
//dummy low
//float dummy[12]={0.01,0.41 ,0.78,0.52,0.66,0.38,0.23,0.43,0.55,0.78,0.99,0.97};
// sum 10 = 4.75
// Average 10 = 0.47


//dummy high
float dummy[12]={4.5,3.5,4.25,5.01,3.98,4.58,4.16,3.85,4.62,3.22,2.99,4.92};
// sum 10 = 41.67
// Average 10 = 4.17

float sum;float * p_sum = &sum; 

float average;float * p_avg = &average;

float peak;float * p_peak = &peak;

float current_samples[10];
float * p = dummy; 
//using pointer to change the address of different values 
float * p_current_samples = current_samples; 
float Max_Samples=10;

float datapoints[2];  // average,peak. and later time
float * p_datapoints = datapoints;

//==============================================================================
/*
 *                    ^^^^Global Variables^^^^^  
 */
//==============================================================================
void setup() {
Serial.begin(9600);
while(!Serial);
}
///////////////////////////////////////////////////////////
//Transfer #Max_Samples# samples from dummy into current_samples
void transfer(){  

 // String dataString = ""; //Create Empty String for Data
 
  for(int i=0;i<Max_Samples;i++){ 
  *p_current_samples=(*p) *200 ; // amplifer

  //SD Card addition 
  String datastring = String(*p_current_samples); //Want a new row for each sample + ", ";
  ////////////////Write to File//////////////////////
  //If the file is available, write to it:
  if (dataFile) {​​
  dataFile.println(dataString); //Simply print to file. 
  ////////////////////////////////////////////////////

   Serial.print("Element ");
   Serial.print( i);
   Serial.print(" is ");
   Serial.println(*p_current_samples); 
    delay(300);
  p_current_samples++;
  p++;
  //pointers go to the next   

  
 }


 
  p=dummy;// use pointer to point to array element 1
  p_current_samples=current_samples; //Reset pointers// will reset later
  
  return;
}
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
//------------------------------------------------------///
///////////////////////////////////////////////////////////
/////////Calculate the sum & avg of an array///////////////
// Taking the sum of the elements
void summer(){
  float total=0; // Needs to be a float too
  for(int i=0;i<Max_Samples;i++){
    total+=*p_current_samples;
    p_current_samples++;
  }
  p=dummy; //Reset p to the first avriable, otherwise it increases into random memory, 
  p_current_samples=current_samples; //Reset pointers
    *p_sum=total;

  float size=sizeof(current_samples)/sizeof(current_samples[0]);
  float avg= sum/size;
  
   //=======increment p_datapoints =======//
   *p_datapoints = avg;
    p_datapoints++; 
   *p_avg=avg;
 
  return;
}
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
//------------------------------------------------------//
///////////////////////////////////////////////////////////
///////// Find highest value in the sample Array///////////
void Peak(){

int peak_index=0 ; // is the peak current index
//float peak_val; // is the peak current value
 for(int i=0;i<Max_Samples;i++){ 
         if (current_samples[i] > current_samples[peak_index]) {
        peak_index = i; //max will store the maximum values of the current samples
        } // end if
      } //end for 
    
  //  peak_val = current_samples[peak_index];  //made a pointer switch here
  *p_peak=current_samples[peak_index];
    Serial.print("The peak current is ");
    Serial.println(*p_peak);


    
//=======decrement pointer p_datapoints =======//
//  *p_datapoints = peak_val;  //Ken Commented out 
  *p_datapoints = *p_peak;
  p_datapoints--; 
  return;
  }
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

void loop() {
  //Init
  
  //Acquiring Samples
    //Sample # is _. (Looped)
  
  
  //Passing sample array for summing. 
    //Sum is _.
  //Passing sum for averaging 
    //Average is _.
  //Ready to Continue
  //================================= 
  // init

//////////////Open(Create) New File///////////////
mySampleFile = SD.open("samples.csv", FILE_WRITE);  //
String dataString = ""; //Create Empty String for Data
  
  Serial.println("Get stored samples from array");
  transfer();  //Get samples for dummy into current_samples
  delay(100);
  
  Serial.println("Passing array element for summing values");
  delay(100);
  summer();   //Now void function 
  Serial.print("Sum is: ");
  Serial.println(*p_sum); 

  //Added to summing function//
  /*delay(100);
  //Serial.println("Sending sum to find the average value");
  //average(p_sum);*/
    
  Serial.print("Average is: ");
  Serial.println(*p_avg); 

  delay(100);
  Serial.println("Ready for more computations"); 
  
  //===============================
  Serial.println("Find the Peak Value Current Measurement");
  Peak(); // Find peak current value
  delay(100);
//==========================================
/*
 *       Printing Datapoints array
 */
//===========================================

  Serial.println("Datapoints array is ");
  Serial.print("Average ");
  Serial.println(datapoints[0]);
  Serial.print("Peak ");
  Serial.println(datapoints[1]);
  
 while(1);
}//end of loop
