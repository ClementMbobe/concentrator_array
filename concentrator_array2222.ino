#include <Wire.h> // Library for I2C communication
#include <LiquidCrystal_I2C.h>
#include "SoftwareI2C.h"
#include "EmonLib.h" // Include Emon Library
EnergyMonitor emon1; // Create an instance
 

LiquidCrystal_I2C lcd= LiquidCrystal_I2C(0x27, 16, 2);
SoftwareI2C softwarei2c;

const int LpressureInput =A3;
const int HpressureInput =A6; 
const int LpressureZero =90;
const int LpressureMax =780;
const int HpressureZero =90;
const int HpressureMax =793;
const int LpressuretransducerMaxPSI = 30;
const int HpressuretransducerMaxPSI = 100;
const int sensorreadDelay = 250;
const int SSR4comp=5 ,SSR1 =2,SSR2=3,SSR3=4,SSR5=6,SSR6=7,SSR7 =8, sol=12,sol1=11; //Solid State Relay pins
long runTime = 0;
long previousMillis = 0;
long interval = 60000; 
long interval2 = 120000;
float a,b,c,d,e,f;
//PIN D9 and D10

float LpressureValue = 0;
float HpressureValue = 0;
double Irms;


void setup() {// put your setup code here, to run once:
  
  Serial.begin(9600);
  
  //softwarei2c.begin(4, 5);       // sda, scl
  lcd.init();
  lcd.backlight();
  pinMode(SSR1,OUTPUT); //set SSR pins as output pins
  pinMode(SSR2,OUTPUT);
  pinMode(SSR3,OUTPUT);
  pinMode(SSR5,OUTPUT);
  pinMode(SSR6,OUTPUT);
  pinMode(SSR7,OUTPUT);
  pinMode(SSR4comp,OUTPUT);
  pinMode(LpressureInput,INPUT);
  pinMode(HpressureInput,INPUT);
  pinMode(sol,OUTPUT);
  pinMode(sol1,OUTPUT);
  digitalWrite(SSR4comp,LOW);
  digitalWrite(sol,LOW);
  digitalWrite(sol1,LOW);

   lcd.clear();
   lcd.setCursor(0,0);   
   lcd.println("Warming Up");
   Serial.println("Warming Up");
  delay(15000);
  emon1.current(2, 111.1);             // Current: input pin, calibration.
  conc_current_check();
  Serial.print(a);
  Serial.print(", ");
  Serial.print(b);
  Serial.print(", ");
  Serial.print(c);
  Serial.print(", ");
  Serial.print(d);
  Serial.print(", ");
  Serial.print(e);
  Serial.print(", ");
  Serial.println(f); 
}

void loop() {
  unsigned long currentMillis = millis();
  LpressureValue = analogRead(LpressureInput);//read pressure value
  Serial.print("read Lvalue =");
  Serial.print(LpressureValue);
  Serial.print(",");
  
  HpressureValue = analogRead(HpressureInput);//read pressure value
  Serial.print("read Hvalue =");
  Serial.print(HpressureValue);
  Serial.print(",");
  LpressureValue = ((LpressureValue - LpressureZero)*LpressuretransducerMaxPSI)/(LpressureMax - LpressureZero);//convert pressure value to PSi
  HpressureValue = ((HpressureValue - HpressureZero)*HpressuretransducerMaxPSI)/(HpressureMax - HpressureZero);//convert pressure value to PSI
  if(LpressureValue < 0)          //set all negative pressures to zero
    LpressureValue = 0;
  if(HpressureValue < 0)       //set all negative pressures to zero
    HpressureValue = 0;
//  if (LpressureValue > 8 && HpressureValue < 50)  
//    compressorSol_on();         // turn on compressor and valve
//  if(LpressureValue <4)
//   compressorSol_off();         // turn off compressor and valve and turns on all concentrators and relief solenoid valve
//  if (LpressureValue > 10)
//   all_conc_off();
  if (LpressureValue > 8 && HpressureValue < 50)  
    compressorSol_on();         // turn on compressor and valve
  if(LpressureValue <4)
   compressorSol_off();         // turn off compressor and valve and turns on all concentrators and relief solenoid valve
  if (LpressureValue > 10)
   all_conc_off();
  if ((HpressureValue < 40) && (LpressureValue < 8) || (HpressureValue < 50)){
   all_on();}
   

  runTime=currentMillis - previousMillis;//time taken by left and right side concentrators when HpressureValue is greater than 50 but < 70
 //if high pressure is greater tahn 50 but less than 70 and lowpressure is less than 8 do the following
  if((HpressureValue > 50) && (HpressureValue < 71) && (LpressureValue < 8)){ 
   if(runTime < interval) { 
      if(a>0.8 && b>0.8 && c>0.8){// if all concentrators on the left side are connected
       leftHS_concentrators_on();
      }
      else{
        for(int i=0;i<10;i++){// if some concentrators on the left side are not connected, select the connected from both sides
          if(a>0.8 && d>0.8 && e>0.8){
             half_on1 ();
             break;
          }
          if(a>0.8 && d>0.8 && f>0.8){
             half_on2 ();
             break;
           }
          if(a>0.8 && e>0.8 && f>0.8){
              half_on3 ();
              break;
           }
          if(b>0.8 && d>0.8 && e>0.8){
              half_on4 ();
              break;
             }
          if(b>0.8 && d>0.8 && f>0.8){
              half_on5 ();
              break;
             }
          if(b>0.8 && e>0.8 && f>0.8){
              half_on6 ();
              break;
             }
          if(c>0.8 && d>0.8 && e>0.8){
              half_on7 ();
              break;
             }
          if(c>0.8 && d>0.8 && f>0.8){
             half_on8 ();
             break;
           }
          if(c>0.8 && e>0.8 && f>0.8){
             half_on9 ();
             break;
           }
          if(d>0.8 && a>0.8 && b>0.8){
              half_on10 ();
              break;
             }

          if(d>0.8 && a>0.8 && c>0.8){
              half_on11 ();
              break;
             } 

          if(d>0.8 && b>0.8 && c>0.8){
              half_on12 ();
              break;
             }
          if(e>0.8 && a>0.8 && b>0.8){
             half_on13 ();
             break;
              }

          if(e>0.8 && a>0.8 && c>0.8){
             half_on14 ();
             break;
             }
          if(e>0.8 && b>0.8 && c>0.8){
             half_on15 ();
             break;
            }

          if(f>0.8 && a>0.8 && b>0.8){
              half_on16 ();
              break;
             }
          if(f>0.8 && a>0.8 && c>0.8){
              half_on17 ();
              break;
              }
           if(f>0.8 && b>0.8 && c>0.8){
              half_on18 ();
              break;
             }
           if((a+b+c)>0.8){
              leftHS_concentrators_on();
              break;
             }
           if((d+e+f)>0.8){
              rightHS_concentrators_on();
              break;
             }
             
        }}}
           

//          switch (i){
//            Serial.println(i);
//            case 1:
//             if(a>0.45 && d>0.45 && e>0.45){
//              half_on1 ();
//             }
//             break;
//             break;
//
//             
//            case 2:
//             if(a>0.45 && d>0.45 && f>0.45){
//              half_on2 ();
//             }
//             break;
//              break;
//            case 3:
//             if(a>0.45 && e>0.45 && f>0.45){
//              half_on3 ();
//             }
//             break;
//            case 4:
//             if(b>0.45 && d>0.45 && e>0.45){
//              half_on4 ();
//             }
//             break;
//             
//            case 5:
//             if(b>0.45 && d>0.45 && f>0.45){
//              half_on5 ();
//             }
//             break;
//            case 6:
//             if(b>0.45 && e>0.45 && f>0.45){
//              half_on6 ();
//             }
//             break; 
//             
//            case 7:
//             if(c>0.45 && d>0.45 && e>0.45){
//              half_on7 ();
//             }
//             break;
//             
//            case 8:
//             if(c>0.45 && d>0.45 && f>0.45){
//              half_on8 ();
//             }
//             break;
//            case 9:
//             if(c>0.45 && e>0.45 && f>0.45){
//              half_on9 ();
//             }
//             break;
//             
//            default: // if all options are false, turn on those on the right 
//             leftHS_concentrators_on();
//             break; 
//             }
//            }
//        }
//       
//       }
 
     else if((runTime > interval) && (runTime < interval2)) {
      if(d>0.8 && e>0.8 && f>0.8){ //if all concentrators on the right side are connected
       rightHS_concentrators_on();
      }
      else{
        for(int i=0;i<10;i++){// if some concentrators on the right side are not connected, select the connected from both sides
          if(d>0.8 && a>0.8 && b>0.8){
              half_on10 ();
              break;
             }

          if(d>0.8 && a>0.8 && c>0.8){
              half_on11 ();
              break;
             } 

          if(d>0.8 && b>0.8 && c>0.8){
              half_on12 ();
              break;
             }
          if(e>0.8 && a>0.8 && b>0.8){
             half_on13 ();
             break;
              }

          if(e>0.8 && a>0.8 && c>0.8){
             half_on14 ();
             break;
             }
          if(e>0.8 && b>0.8 && c>0.8){
             half_on15 ();
             break;
            }

          if(f>0.8 && a>0.8 && b>0.8){
              half_on16 ();
              break;
             }
          if(f>0.8 && a>0.8 && c>0.8){
              half_on17 ();
              break;
              }
          if(f>0.8 && b>0.8 && c>0.8){
              half_on18 ();
              break;
             }
          if(a>0.8 && d>0.8 && e>0.8){
             half_on1 ();
             break;
          }
          if(a>0.8 && d>0.8 && f>0.8){
             half_on2 ();
             break;
           }
          if(a>0.8 && e>0.8 && f>0.8){
              half_on3 ();
              break;
           }
          if(b>0.8 && d>0.8 && e>0.8){
              half_on4 ();
              break;
             }
          if(b>0.8 && d>0.8 && f>0.8){
              half_on5 ();
              break;
             }
          if(b>0.8 && e>0.8 && f>0.8){
              half_on6 ();
              break;
             }
          if(c>0.8 && d>0.8 && e>0.8){
              half_on7 ();
              break;
             }
          if(c>0.8 && d>0.8 && f>0.8){
             half_on8 ();
             break;
           }
          if(c>0.8 && e>0.8 && f>0.8){
             half_on9 ();
             break;
           }
         if((d+e+f)>0.8){
              rightHS_concentrators_on();
              break;
             }
         if((a+b+c)>0.8){
              leftHS_concentrators_on();
              break;
             }
           
        }
      }
     }

      else{
       previousMillis = currentMillis; 
     }
     
 }
  
 if(HpressureValue > 50 && HpressureValue < 71 && LpressureValue > 8){
      all_conc_off();
      compressorSol_on();  
  }



 if(HpressureValue > 70){
   all_off();                   // turm off all concentrators, compressor and valve
  }
   
 lcd.clear();
 lcd.setCursor(0,0);   
 lcd.print("P1: ");
 lcd.print(LpressureValue,1);  
 lcd.println("psi");
 lcd.setCursor(0, 1);
 lcd.print("P2: ");
 lcd.print(HpressureValue,1);
 //lcd.print(runTime,1);
 lcd.println("psi");  
 
 Serial.print("Low output pressure =");
 Serial.print (LpressureValue,1);  
 Serial.print(",");
 Serial.print(" High output pressure1 =");
 Serial.println(HpressureValue,1);
 Serial.println(runTime,1);  
 delay(sensorreadDelay);
}
void compressorSol_off(){ //this function switches on all concentrators and switches off the outlet compressor
  
 digitalWrite(SSR4comp,LOW);
 digitalWrite(sol,LOW);
 digitalWrite(SSR1,HIGH);
 digitalWrite(SSR2,HIGH);
 digitalWrite(SSR3,HIGH);
 digitalWrite(SSR5,HIGH);
 digitalWrite(SSR6,HIGH);
 digitalWrite(SSR7,HIGH);
 //delay(2000);
 //digitalWrite(sol1,HIGH);
 //delay(5000);
 //digitalWrite(sol1,LOW);

 

}
void compressorSol_on(){//this function switches off all concentrators and switches on the outlet compressor

 digitalWrite(sol1,HIGH);
 delay(1000);
 digitalWrite(sol1,LOW);
 digitalWrite(SSR4comp,HIGH);
 digitalWrite(sol,HIGH);

 
 
}
void all_on(){
  digitalWrite(SSR1,HIGH);
  digitalWrite(SSR2,HIGH);
  digitalWrite(SSR3,HIGH);
  digitalWrite(SSR5,HIGH);
  digitalWrite(SSR6,HIGH);
  digitalWrite(SSR7,HIGH);
  }
void leftHS_concentrators_on(){
  digitalWrite(SSR1,HIGH);
  digitalWrite(SSR2,HIGH);
  digitalWrite(SSR3,HIGH);
  digitalWrite(SSR5,LOW);
  digitalWrite(SSR6,LOW);
  digitalWrite(SSR7,LOW);
}

 void rightHS_concentrators_on(){
  digitalWrite(SSR1,LOW);
  digitalWrite(SSR2,LOW);
  digitalWrite(SSR3,LOW);
  digitalWrite(SSR5,HIGH);
  digitalWrite(SSR6,HIGH);
  digitalWrite(SSR7,HIGH);
  }

 void half_on1(){
  digitalWrite(SSR1,HIGH);
  digitalWrite(SSR2,LOW);
  digitalWrite(SSR3,LOW);
  digitalWrite(SSR5,HIGH);
  digitalWrite(SSR6,HIGH);
  digitalWrite(SSR7,LOW);
  }

 void half_on2(){
  digitalWrite(SSR1,HIGH);
  digitalWrite(SSR2,LOW);
  digitalWrite(SSR3,LOW);
  digitalWrite(SSR5,HIGH);
  digitalWrite(SSR6,LOW);
  digitalWrite(SSR7,HIGH);
  }

 void half_on3(){
  digitalWrite(SSR1,HIGH);
  digitalWrite(SSR2,LOW);
  digitalWrite(SSR3,LOW);
  digitalWrite(SSR5,LOW);
  digitalWrite(SSR6,HIGH);
  digitalWrite(SSR7,HIGH);
  }

 void half_on4(){
  digitalWrite(SSR1,LOW);
  digitalWrite(SSR2,HIGH);
  digitalWrite(SSR3,LOW);
  digitalWrite(SSR5,HIGH);
  digitalWrite(SSR6,HIGH);
  digitalWrite(SSR7,LOW);
  }

 void half_on5(){
  digitalWrite(SSR1,LOW);
  digitalWrite(SSR2,HIGH);
  digitalWrite(SSR3,LOW);
  digitalWrite(SSR5,HIGH);
  digitalWrite(SSR6,LOW);
  digitalWrite(SSR7,HIGH);
  }

 void half_on6(){
  digitalWrite(SSR1,LOW);
  digitalWrite(SSR2,HIGH);
  digitalWrite(SSR3,LOW);
  digitalWrite(SSR5,LOW);
  digitalWrite(SSR6,HIGH);
  digitalWrite(SSR7,HIGH);
  }
 void half_on7(){
  digitalWrite(SSR1,LOW);
  digitalWrite(SSR2,LOW);
  digitalWrite(SSR3,HIGH);
  digitalWrite(SSR5,HIGH);
  digitalWrite(SSR6,HIGH);
  digitalWrite(SSR7,LOW);
  }

 void half_on8(){
  digitalWrite(SSR1,LOW);
  digitalWrite(SSR2,LOW);
  digitalWrite(SSR3,HIGH);
  digitalWrite(SSR5,HIGH);
  digitalWrite(SSR6,LOW);
  digitalWrite(SSR7,HIGH);
  }

 void half_on9(){
  digitalWrite(SSR1,LOW);
  digitalWrite(SSR2,LOW);
  digitalWrite(SSR3,HIGH);
  digitalWrite(SSR5,LOW);
  digitalWrite(SSR6,HIGH);
  digitalWrite(SSR7,HIGH);
  }

 void half_on10(){
  digitalWrite(SSR1,HIGH);
  digitalWrite(SSR2,HIGH);
  digitalWrite(SSR3,LOW);
  digitalWrite(SSR5,HIGH);
  digitalWrite(SSR6,LOW);
  digitalWrite(SSR7,LOW);
  }

 void half_on11(){
  digitalWrite(SSR1,HIGH);
  digitalWrite(SSR2,LOW);
  digitalWrite(SSR3,HIGH);
  digitalWrite(SSR5,HIGH);
  digitalWrite(SSR6,LOW);
  digitalWrite(SSR7,LOW);
  }

 void half_on12(){
  digitalWrite(SSR1,LOW);
  digitalWrite(SSR2,HIGH);
  digitalWrite(SSR3,HIGH);
  digitalWrite(SSR5,HIGH);
  digitalWrite(SSR6,LOW);
  digitalWrite(SSR7,LOW);
  }

 void half_on13(){
  digitalWrite(SSR1,HIGH);
  digitalWrite(SSR2,HIGH);
  digitalWrite(SSR3,LOW);
  digitalWrite(SSR5,LOW);
  digitalWrite(SSR6,HIGH);
  digitalWrite(SSR7,LOW);
  }

 void half_on14(){
  digitalWrite(SSR1,HIGH);
  digitalWrite(SSR2,LOW);
  digitalWrite(SSR3,HIGH);
  digitalWrite(SSR5,LOW);
  digitalWrite(SSR6,HIGH);
  digitalWrite(SSR7,LOW);
  }

 void half_on15(){
  digitalWrite(SSR1,LOW);
  digitalWrite(SSR2,HIGH);
  digitalWrite(SSR3,HIGH);
  digitalWrite(SSR5,LOW);
  digitalWrite(SSR6,HIGH);
  digitalWrite(SSR7,LOW);
  }

 void half_on16(){
  digitalWrite(SSR1,HIGH);
  digitalWrite(SSR2,HIGH);
  digitalWrite(SSR3,LOW);
  digitalWrite(SSR5,LOW);
  digitalWrite(SSR6,LOW);
  digitalWrite(SSR7,HIGH);
  }

 void half_on17(){
  digitalWrite(SSR1,HIGH);
  digitalWrite(SSR2,LOW);
  digitalWrite(SSR3,HIGH);
  digitalWrite(SSR5,LOW);
  digitalWrite(SSR6,LOW);
  digitalWrite(SSR7,HIGH);
  }

 void half_on18(){
  digitalWrite(SSR1,LOW);
  digitalWrite(SSR2,HIGH);
  digitalWrite(SSR3,HIGH);
  digitalWrite(SSR5,LOW);
  digitalWrite(SSR6,LOW);
  digitalWrite(SSR7,HIGH);
  }

  
  
 void all_off(){// all concentrators,solenoid and compressor off
  digitalWrite(SSR1,LOW);
  digitalWrite(SSR2,LOW);
  digitalWrite(SSR3,LOW);
  digitalWrite(SSR5,LOW);
  digitalWrite(SSR6,LOW);
  digitalWrite(SSR7,LOW);
  digitalWrite(SSR4comp,LOW);
  digitalWrite(sol,LOW);
  //delay(2000);
  //digitalWrite(sol1,HIGH);
  //delay(5000);
  //digitalWrite(sol1,LOW);
 
  }
 void all_conc_off(){
  digitalWrite(SSR1,LOW);
  digitalWrite(SSR2,LOW);
  digitalWrite(SSR3,LOW);
  digitalWrite(SSR5,LOW);
  digitalWrite(SSR6,LOW);
  digitalWrite(SSR7,LOW); 
 }
 
void conc_current_check(){// measres current drawn by a concentrator and stores the value in a variable
  //int a,b,c,d,e,f;
  //if (Irms<0.3){
    //Irms=0;} 
  all_conc_off();
  digitalWrite(SSR1,HIGH);
  Irms = emon1.calcIrms(1480);  // Calculate Irms only
  delay(1000);
   if (Irms>1.1){
    a=Irms; 
    digitalWrite(SSR1,LOW);
    }
   else{
     a=0;}
   
  digitalWrite(SSR2,HIGH);
  Irms = emon1.calcIrms(1480);  // Calculate Irms only
  delay(1000);
   if (Irms>0.8){
    b=Irms; 
    digitalWrite(SSR2,LOW);
   }
   else{
    b=0;}

  digitalWrite(SSR3,HIGH);
  Irms = emon1.calcIrms(1480);  // Calculate Irms only
  delay(1000);
   if (Irms>0.8){
    c=Irms; 
    digitalWrite(SSR3,LOW);
   }
   else{
    c=0;}

  digitalWrite(SSR5,HIGH);
  Irms = emon1.calcIrms(1480);  // Calculate Irms only
  delay(1000);
   if (Irms>0.8){
    d=Irms; 
    digitalWrite(SSR5,LOW);
   }
   else{
    d=0;}

  digitalWrite(SSR6,HIGH);
  Irms = emon1.calcIrms(1480);  // Calculate Irms only
  delay(1000);
   if (Irms>0.8){
    e=Irms; 
    digitalWrite(SSR6,LOW);
   }
   else{
    e=0;}

  digitalWrite(SSR7,HIGH);
  delay(1000);
  Irms = emon1.calcIrms(1480);  // Calculate Irms only
   if (Irms>0.8){
    f=Irms; 
    digitalWrite(SSR7,LOW);
   }
   else{
    f=0;} 


    
 }




 
  




 
