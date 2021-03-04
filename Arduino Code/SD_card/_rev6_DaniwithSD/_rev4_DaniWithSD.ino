
///////////////Library Includes////////////////////
// include the SD library:
#include <SD.h>
#include <SPI.h>
#include <TimeLib.h>


//==============================================================================
/*
 *                    vvvv Global Variables vvvv 
 */
//==============================================================================
//float dummy[12]={988.50,994.15,886.53,798.55,999.34,543.55,656.70,700.05,854.09,991.5,150.87,547.98};
float dummy[12]={4.5,3.5,4.25,5.01,3.98,4.58,4.16,3.85,4.62,3.22,2.99,4.92};
float dummy2[12]={0.01,0.41 ,0.78,0.52,0.66,0.38,0.23,0.43,0.55,0.78,0.99,0.97};
float * p_dummy = dummy; // Will point to dummy 1 then dummy 2
float current_samples[10]; //
float * p_current_samples = current_samples; 
float Max_Samples=10; // ==Testing only=========//

//Pointer loaded with datapoint information 
float sum;float * p_sum = &sum; 
float average;float * p_avg = &average;
float peak;float * p_peak = &peak;
float datapoints[2];  //Number of columns for the datapoint. (Average, Peak) 
float * p_datapoints = datapoints;

//Time information for datapoints
String Time="";
String * p_Time = &Time;
String Date="";
String * p_Date = &Date;

//==============================================================================
/*
 *                    ^^^^Global Variables^^^^^  
 */
//==============================================================================




//==============================================================================
/*
 *                    vvvvvvv Initializations vvvvvvvvv  
 */
//==============================================================================


                            // set up variables using the SD utility library functions:
                            //Sd2Card card;
                            //SdVolume volume;
                            //SdFile root;
//////////////Confirm Init//////////////

const int chipSelect = BUILTIN_SDCARD;

void setup() {

 // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect.
  }

  Serial.print("Initializing SD card...");
  
    // see if the card is present and can be initialized:
    if (!SD.begin(chipSelect)) {
      Serial.println("Card failed, or not present");
      // don't do anything more:
      return;
    }
    Serial.println("card initialized.");
}//end of void setup

//==============================================================================
/*
 *                    ^^^^Initializations^^^^^  
 */
//==============================================================================




//==============================================================================
/*
 *                    vvvvvv Functions vvvvvvv 
 */
//==============================================================================


///////////////////////////////////////////////////////////
//Transfer #Max_Samples# samples from dummy into current_samples
void transfer(){  
 
  Serial.print(*p_dummy);//check

  
 // makes sure that there isn't more than 300 datapoints
 
 String datastring = "";//create empty string for data
  
    for(int i=0;i<Max_Samples;i++){ 
          *p_current_samples=(*p_dummy) *200 ; // scale up values
   
           //SD Card addition 
            datastring += String(*p_current_samples) +",";
      
    ////////////////Write to File//////////////////////
     Serial.print("Element ");
     Serial.print( i);
     Serial.print(" is ");
     Serial.println(*p_current_samples); 
      delay(300);
          p_current_samples++;
          p_dummy++;
          //pointers go to the next   
        }//end for
          
          // store current samples in a data file
  SD.remove("SAMPLES.csv"); //clear file first before adding anything to the csv file
          
              File dataFile = SD.open("SAMPLES.csv", FILE_WRITE);

                         if (dataFile) {
                            dataFile.println(datastring);
                            delay(1000);
                            
                            dataFile.close();
                            
                            // print to the serial port too:
                               Serial.println(datastring);
                          }  
                          // if the file isn't open, pop up an error:
                          else {
                            Serial.println("error opening SAMPLES.csv");
                          }  
  p_dummy=dummy2;// use pointer to point to array element 1;
  Serial.println(*p_dummy);
  
  //we can call a second time to run dumm2 array
  
  p_current_samples=current_samples; //Reset pointers// will reset later

  return;
  
}// end transfer
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
//------------------------------------------------------///
///////////////////////////////////////////////////////////
/////////Calculate the sum & avg of an array///////////////
// Taking the sum of the elements
void Average(){
  float total=0; // Needs to be a float too
  for(int i=0;i<Max_Samples;i++){
    total+=*p_current_samples;
    p_current_samples++;
  }
 // p_dummy=dummy; //Reset p to the first avriable, otherwise it increases into random memory, 
  p_current_samples=current_samples; //Reset pointers
    *p_sum=total;

  float size=sizeof(current_samples)/sizeof(current_samples[0]);
  float avg= sum/size;
  
   //=======increment p_datapoints =======//
   *p_datapoints = avg;
    p_datapoints++; 
   *p_avg=avg;
 
  return;
} //end Average
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
  }//end Peak
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
void Data2CSV(){

// SD.remove("dataOUT.csv");

  
    String datastring = "" ; //empty
    datastring = String(*p_avg)+ "," + String(*p_peak)+ "," ; // for dummy 1
                              // print to the serial port too:
//                              Serial.println(datastring);
    File dataFile = SD.open("dataOUT.csv", FILE_WRITE);
                     
                        if (dataFile) {
                            dataFile.println(datastring);
                            delay(1000);
                            
                            dataFile.close();
                            
                            // print to the serial port too:
                               Serial.println(datastring); // means it is true
                          }  
                          // if the file isn't open, pop up an error:
                          else {
                            Serial.println("error opening dataOUT.csv");
                          }  
}// end DATA2CSV

///////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////

void formatCSV(){
   String datastring = "" ; //empty
    datastring = String("RMS Average")+ "," + String("Peak Current")+ "," ; // putting the headers 
                              // print to the serial port too:
                             Serial.println(datastring);
    File dataFile = SD.open("dataOUT.csv", FILE_WRITE);
                     
                        if (dataFile) {
                            dataFile.println(datastring);
                            delay(1000);
                            
                            dataFile.close();
                            
                            // print to the serial port too:
                               Serial.println(datastring); // means it is true
                          }  
                          // if the file isn't open, pop up an error:
                          else {
                            Serial.println("error opening dataOUT.csv");
                          }  

  
}//end of formatCSV

//==============================================================================
/*
 *                    ^^^^Functions^^^^^  
 */
//==============================================================================






//==============================================================================
/*
 *                    vvvvvvv Main Function vvvvvvvv  
 */
//==============================================================================

void loop() {
  //Init

//clear out datapoints for new 
  SD.remove("dataOUT.csv");

  formatCSV();
 
//////////////Open(Create) New File///////////////

  Serial.println("Get stored samples from array");
  transfer();  //Get samples for dummy into current_samples
  delay(100);
  
  Serial.println("Passing array element for finding average values");
  delay(100);
  Average();   //Now void function 
  Serial.print("Sum is: ");
  Serial.println(*p_sum); 

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
  Serial.println(*p_avg);
  Serial.print("Peak ");
  Serial.println(*p_peak);

//Call 
 Data2CSV();
 
// call transfer for dummy2 

      transfer(); // will show dummy 2 arrau values
      Average();
      Peak();
 Data2CSV();
//=======================================================
  
  
 while(1);
}//end of loop

//==============================================================================
/*
 *                    ^^^^Main Function^^^^^  
 */
//==============================================================================
