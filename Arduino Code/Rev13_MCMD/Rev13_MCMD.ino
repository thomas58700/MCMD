
///////////////Library Includes////////////////////
// include the SD library:
#include <SD.h>
#include <SPI.h>
#include <TimeLib.h>

                      // set up variables using the SD utility library functions:
                      //Sd2Card card;
                      //SdVolume volume;
                      //SdFile root;

//==============================================================================
/*
 *                    vvvv Global Variables/Declarations vvvv 
 */
//==============================================================================

float current_samples[10]; //
float * p_current_samples = current_samples; 
float Max_Samples=10; // ==Testing only=========//

//Time information for datapoints
String Time="";
String * p_Time = &Time;
String Date="";
String * p_Date = &Date;
String Device_Name = "MCMD 01";
String * p_Device_Name= &Device_Name;
//========================

//Pointer loaded with datapoint information 
float sum;float * p_sum = &sum; 
float average;float * p_avg = &average;
float peak;float * p_peak = &peak;
float datapoints[2];  //Number of columns for the datapoint. (Average, Peak) 
float * p_datapoints = datapoints;

 String datastring = ""; // updated empty string ; will added samples 
 String * p_datastring = &datastring;
 
//========================================
//===========CURRENT SENSOR===============

float RefVal = 3.3;

//use pin A13 on Teensy
//#define Pin A13;

//These are the variable for the current sensor

float sensitivity = 1000.0 / 264.0; //1000mA per 264mV

float Vref = 327.49;   //You need test it !!!

    // The on-board ADC is 10-bits
        // Different power supply will lead to different reference sources
        // example: 2^10 = 1024 -> 5V / 1024 ~= 4.88mV
        //          unitValue= 5.0 / 1024.0*1000 ;
        float unitValue = (3.3 / 1024.0) * 1000 ;  
        // 2^10 accuracy step size , multiply 1000 = mV
        
      //  float voltage = unitValue * sensorValue;
           
          float voltage;
          float * p_voltage = &voltage;

          long int sensorValue = 0;
          long int * p_sensorValue = &sensorValue;

        // sensor value is in bit value
    
        
        float current ;
        float * p_current = &current;
        

//====================Global RTC Declarations==============================

time_t getTeensy3Time()
  {
    return Teensy3Clock.get();
  }


    /*  code to process time sync messages from the serial port   */
  #define TIME_HEADER  "T"   // Header tag for serial time sync message
  
  unsigned long processSyncMessage() {
    unsigned long pctime = 0L;
    const unsigned long DEFAULT_TIME = 1612681200; // Feb 7 2021 
  
    if(Serial.find(TIME_HEADER)) {
       pctime = Serial.parseInt();
       return pctime;
       if( pctime < DEFAULT_TIME) { // check the value is a valid time (greater than Jan 1 2013)
         pctime = 0L; // return 0 to indicate that the time is not valid
       }
    }
    return pctime;
  }

//==============================================================================
/*
 *                    ^^^^Global Variables/Declarations^^^^^  
 */
//==============================================================================




//==============================================================================
/*
 *                    vvvvvvv Initializations vvvvvvvvv  
 */
//==============================================================================

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

    //====================================================
    // set the Time library to use Teensy 3.0's RTC to keep time
    setSyncProvider(getTeensy3Time);
    
    // put your setup code here, to run once:
    Serial.begin(115200);
  
    while (!Serial);  // Wait for Arduino Serial Monitor to open
    delay(100);
     if (timeStatus()!= timeSet) {
      Serial.println("Unable to sync with the RTC");
    } else {
      Serial.println("RTC has set the system time");
    }
    //===================================================

    
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
// CURRENT SENSOR DATA
void ACS_init(){  
 const int numValue = 500;
 for(int i =0;i<numValue;i++){
    *p_sensorValue += analogRead(A13); // take 1 sample at time for resolution
    delay(2);
 }
    *p_sensorValue = *p_sensorValue / numValue;
     *p_voltage = unitValue * (*p_sensorValue);
     Vref = *p_voltage;
}
// Collecting current samples

void Samples(){  
 const int numValue = 500;
 for(int i =0;i<numValue;i++){
    *p_sensorValue += analogRead(A13); // take 1 sample at time for resolution
    delay(2);
 }
    *p_sensorValue = *p_sensorValue / numValue;
     *p_voltage = unitValue * (*p_sensorValue);
 Serial.print ("->Voltage: "); 
 Serial.print(*p_voltage);
 Serial.println( " mV");
    // Calculate the corresponding current
      *p_current = (*p_voltage - Vref) * sensitivity;
  
      *p_datastring += String(*p_current)+ ",";

      // very time sample() is called updated sample is added to an array current samples
      *p_current_samples = *p_current;
      *p_current_samples++;
  
   // makes sure that there isn't more than 300 datapoints
                    // String datastring = "";//create empty string for data
                    //    for(int i=0;i<Max_Samples;i++){ 
                    //          *p_current_samples=(*p_dummy) *200 ; // scale up values
                    //   
                    //           //SD Card addition 
                    //            datastring += String(*p_current_samples) +",";
                    //      
                    //    ////////////////Write to File//////////////////////
                    //     Serial.print("Element ");
                    //     Serial.print( i);
                    //     Serial.print(" is ");
                    //     Serial.println(*p_current_samples); 
                    //      delay(300);
                    //          p_current_samples++;
                    //          p_dummy++;
                    //          //pointers go to the next   
                    //        }//end for
} // end Samples
//================================================================================
 //Name: Samples2CSV
//Purpose: With use of a pointer to the sample datastring, this function
//creates a CSV file of all current samples to be used in the datapoint. 
//Note: In a later revision, we may want this function to include the pointers, 
//in the same way Data2CSV works. 
//Transfer #Max_Samples# samples from dummy into current_samples 
//=================================================================================
//NAME: TEMPERATURE SESNOR CODE
 int tempPin = A9;
 int tempReading;
 float temperatureC;
 void temperature(){
  tempReading = analogRead(tempPin);
  float voltage_temp = tempReading * 3.3;
  voltage_temp /= 1024.0;
  //float temperatureC =(voltage_temp -0.5) * 100;
   temperatureC =(voltage_temp -0.5) * 100;
  Serial.print("->Temperature: ");
  Serial.print(temperatureC);
  Serial.println("°C");
 }

//=================================================================================
// NAME: PUT THE CURRENT VALUE INTO A STRING
  void Samples2CSV(){  
    
       SD.remove("SAMPLES.csv"); //clear file first before adding anything to the csv file
              
                  File dataFile = SD.open("SAMPLES.csv", FILE_WRITE);
    
                             if (dataFile) {
                                dataFile.println(*p_datastring);
                                delay(1000);
                                
                                dataFile.close();
                                
                                // print to the serial port too:
                                   //Serial.println(*p_datastring);
                              }  
                              // if the file isn't open, pop up an error:
                              else {
                                Serial.println("error opening SAMPLES.csv");
                              }  
 
  return;
  }// end transfer
//==================================================================================
///////////////////////////////////////////////////////////

//------------------------------------------------------///



//------------------------------------------------------///

///////////////////////////////////////////////////////////
//Name: Average
//Purpose: With use of pointers, this function calculates 
///both the sum and the average of an array with samples and
//stores it within the pointer p_avg. 
//
// Taking the sum of the elements
void Average(){

  p_current_samples=current_samples; //Reset pointer to first elemnent

  // Summing
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

//------------------------------------------------------//

///////////////////////////////////////////////////////////
//Name: Peak
//Purpose: With use of pointers, this function finds 
///and assigns the highest value within the samples array to p_peak. 
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
    Serial.print(*p_peak);
    Serial.println(" mA");


    
//=======decrement pointer p_datapoints =======//
//  *p_datapoints = peak_val;  //Ken Commented out 
  *p_datapoints = *p_peak;
  p_datapoints--; 
  return;
  }//end Peak
///////////////////////////////////////////////////////////

//------------------------------------------------------//

///////////////////////////////////////////////////////////
//Name: Data2CSV
//Purpose: With use of pointers, this function places all the  
///appropriate pointers within a string to be appended to dataOUT.csv. 
void Data2CSV(){

  // call Time and Date
      getTime();
      getDate();
      
  
// SD.remove("dataOUT.csv");

  
    datastring = "" ; //empty
    datastring = String(*p_Device_Name)+ "," + String(*p_Date)+ "," + String(*p_Time)+ "," + String(*p_avg)+ "," + String(*p_peak); // for dummy 1
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
              
      datastring = "" ; //empty
}// end DATA2CSV

///////////////////////////////////////////////////////////

//------------------------------------------------------//

///////////////////////////////////////////////////////////
//Name: format CSV
//Purpose: With use of pointers, this function places all the  
///appropriate headers within a string to be the first column of dataOUT.csv. 

void formatCSV(){
   String datastring = "" ; //empty
    datastring = String("Device Name")+ "," + String("Date (dd/mm/yyyy)")+ "," + String("Time (24hrs)")+ "," + String("Average RMS Current (A)")+ "," + String("Peak RMS Current (A)"); // putting the headers 
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

///////////////////////////////////////////////////////////

//------------------------------------------------------//

///////////////////////////////////////////////////////////
//Name: getTime
//Purpose: With use of pointers, this function places the 
//current time in a string pointed to by p_Time. 
 void getTime(){
      *p_Time = "";
      
      int hour_val=hour();
      *p_Time += String(hour_val)+":";  
      
      int minute_val = minute();
        if(minute_val<10){
          *p_Time += String("0")+String(minute_val)+":";
        }
        else{
          *p_Time += String(minute_val)+":";
        }
      int second_val = second();
      if(second_val<10){
          *p_Time += String("0")+String(second_val);
        }
        else{
          *p_Time += String(second_val);
        }
  }// end getTime
  
///////////////////////////////////////////////////////////

//------------------------------------------------------//

///////////////////////////////////////////////////////////
//Name: getDate
//Purpose: With use of pointers, this function places the 
//current date in a string pointed to by p_Date. 
  void getDate(){
      *p_Date="";
      
      int day_val = day();
            if(day_val<10){
              *p_Date += String("0")+String(day_val)+ "/";
            }
            else{
              *p_Date += String(day_val);
            }
   
      
          int month_val = month();
                if(month_val<10){
                  *p_Date += String("0")+String(month_val)+ "/";
                }
                else{
                  *p_Date += String(month_val);
                }
      
         int year_val = year();
        
       *p_Date += String(year_val);
        
  }// end getDate
  
///////////////////////////////////////////////////////////

//------------------------------------------------------//

///////////////////////////////////////////////////////////
//Name: initRTC
//Purpose: This function initializes the Teensy's RTC.  
 void initRTC(){
  
    if (Serial.available()) {
      time_t t = processSyncMessage();
      if (t != 0) {
        Teensy3Clock.set(t); // set the RTC
        setTime(t);
      }
    }

    
 }// end initRTC
 
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
 initRTC();
 ACS_init();
//clear out datapoints for new 
  SD.remove("dataOUT.csv");

  formatCSV();
 
//////////////Open(Create) New File///////////////

  for(int i=0; i< Max_Samples; i++){
    Serial.println("GET THE CURRENT VALUE");
    Samples();  //Get current_samples
    delay(1000);
    Serial.print("-->Current I = ");
    Serial.print(*p_current);
    Serial.println(" mA");
    temperature();
    //Serial.println(temperatureC);
  }// end for

  Samples2CSV();
  // all 10 values will be added datastring

  // must create an array
  
  Serial.println("Passing array element for finding average values");
  delay(100);
  Average();   //Now void function 
  Serial.print("Sum is: ");
  Serial.print(*p_sum);
  Serial.println(" mA"); 

  Serial.print("Average is: ");
  Serial.print(*p_avg);
  Serial.println(" mA"); 

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

  Serial.print("Datapoints array is: ");
  Serial.println(*p_datastring);
  Serial.print("Average ");
  Serial.print(*p_avg);
  Serial.println(" mA");
  Serial.print("Peak ");
  Serial.print(*p_peak);
  Serial.println(" mA");
//Call 
 Data2CSV();
  
 // RUN ONCE//
 while(1);

 
}//end of loop
// ========================END OF MAIN LOOP==============================//

//==============================================================================
/*
 *                    ^^^^Main Function^^^^^  
 */
//==============================================================================
