
//Global Variables
String Time="";
String * p_Time = &Time;
String Date="";
String * p_Date = &Date;

int hour = 12;
int minute = 3;
int second = 5;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
 }

void getTime(){
  *p_Time = "";
  
  int hour=3;//hour();
  *p_Time += String(hour)+":";  
  
  int minute = 3;//minute();
    if(minute<10){
      *p_Time += String("0")+String(minute)+":";
    }
    else{
      String minute = String(minute)+":";
    }
  int second = 15;//second();
  if(second<10){
      *p_Time += String("0")+String(second);
    }
    else{
      *p_Time += String(second);
    }
}
  
void getDate(){
  *p_Date="";
  
  int day = 3;//day();
  int month = 3;//month();
  int year = 2021;//year();
  
  *p_Date += String(day)+" "+String(month)+" "+String(year);
}
  



void loop() {

getTime();
Serial.println(*p_Time);
getDate();
Serial.println(*p_Date);

}
