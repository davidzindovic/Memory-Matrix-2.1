//to do:quality of life(vmesni casi), pwmjadnje 7seg(probi se cele cifre ali pa več segmentov hkrati);drgač dela
//notes: pwm_zaslon rabi večje cifre kot delay, spucaj komentarje in spiši primerne :)
//za probat: vsak waiting loop prizgi en segment ugasni drugega
//preizkus gretja: waiting loop - OK

#include <SPI.h>
#include <ShiftOutX.h>
#include <ShiftPinNo.h>

#define zamik 1 //refresh rate zaslona
#define zamik_luck 10
#define START_GUMB 2
#define START_LED 3

shiftOutX regOne(A1, A2, A0, MSBFIRST, 4); 

//7 SEG:
uint64_t enice[8]={0,shPin1,shPin2,shPin3,shPin4,shPin5,shPin6,shPin7};
uint64_t desetice[8]={0,shPin9,shPin10,shPin11,shPin12,shPin13,shPin14,shPin15};
bool zaslon=0;//za PWM-janje
uint32_t ponovitve=0;
//lucke:
uint64_t vrstice[6]={0,shPin25,shPin26,shPin27,shPin28,shPin29};
uint64_t stolpci[6]={0,shPin17,shPin18,shPin19,shPin20,shPin21};
//tipkovnica:
byte h=0,v=0;              // variables used in for loops
const int period=50;       // A little delay to avoid errors.
int kdelay=0;              // Non Blocking Delay
const byte rows=5;         // Rows in Keypad
const byte columns=5;      // Columns in Keypad
const byte Output[rows]={9,10,11,12,13};   //Row connceted to Arduino Pins
const byte Input[columns]={8,7,6,5,4}; //Columns connected to Arduino Pins
//za igro:
int cilj[100];//zaporedje tipk, ki bi jih moral igralec pritisniti
int izbor[100];//zaporedje tipk, ki jih je igralec pritisnil
uint8_t stopnja;
int izguba=0;
//start gumb/LED:
uint32_t prej=0; bool stanje=0; int zacasno=0;

void setup() {
  for(byte i=0;i<rows;i++)      //Loop to declare output pins.
  {
  pinMode(Output[i],OUTPUT);
  }
  for(byte s=0;s<columns;s++)  // Loop to decalre Input pins, Initial Pulledup.
  {
    pinMode(Input[s],INPUT_PULLUP);
  }  
  pinMode(START_GUMB, INPUT_PULLUP);
  pinMode(START_LED,OUTPUT);
  attachInterrupt(digitalPinToInterrupt(START_GUMB), sprememba, FALLING);
  
  Serial.begin(9600);
    for(int i=1;i<6;i++) //PREMAKNENO V SETUP
  {
    regOne.pinOff(vrstice[i]);//delay(1);
    regOne.pinOn(stolpci[i]);//delay(1);
  }
}

void sprememba(){zacasno++;}

byte branje_tipkovnice()                   
{bool zasloncek=0;
//    if(millis()-kdelay>period) //used to make non-blocking delay
//  {
//    kdelay=millis();  //capture time from millis function
 while(1){
 static bool no_press_flag=0;    
  for(byte x=0;x<columns;x++)     
  {
     if (digitalRead(Input[x])==HIGH);   
     else
     break;
     if(x==(columns-1))        
     {
      no_press_flag=1;
      h=0;
      v=0;
     }
     zasloncek=!zasloncek;
     pwm_zaslon(0,zasloncek);
  }
  if(no_press_flag==1)  
  {
    for(byte r=0;r<rows;r++)  
    digitalWrite(Output[r],LOW);
    for(h=0;h<columns;h++)  
    {
      if(digitalRead(Input[h])==HIGH) 
      continue;
      else   
      {
          for (v=0;v<rows;v++)    
          {
          digitalWrite(Output[v],HIGH);   
          if(digitalRead(Input[h])==HIGH)  
          {
            no_press_flag=0;               
            for(byte w=0;w<rows;w++)  
            digitalWrite(Output[w],LOW);
          zasloncek=!zasloncek;
          pwm_zaslon(0,zasloncek);
            return v*5+h+1;
          }
          zasloncek=!zasloncek;
          pwm_zaslon(0,zasloncek);
          }
      }
      zasloncek=!zasloncek;
      pwm_zaslon(0,zasloncek);
    }
  }
 //return 50;
}}//}

void lucke(int LED,uint16_t pocakaj, bool pwm)
{
  /*
  for(int i=1;i<6;i++) //PREMAKNENO V SETUP
  {
    regOne.pinOff(vrstice[i]);//delay(1);
    regOne.pinOn(stolpci[i]);//delay(1);
  }  
  */
  //delay(pocakaj);
    //for(int c=0;c<pocakaj;c++){
    regOne.pinOff(stolpci[(LED-1)%5+1]);//delay(20);
    regOne.pinOn(vrstice[((LED-1)/5)+1]);
    //delay(pocakaj/3);
    //pwm_zaslon(pocakaj,0);//delay(pocakaj);
//    pwm_zaslon(0,1);
  // if(!pwm) pwm_zaslon(pocakaj,0);//delay(2);
  //  else delay(pocakaj);
  pwm_zaslon(pocakaj,0);
    //delay(pocakaj/3);
    regOne.pinOn(stolpci[(LED-1)%5+1]);//delay(20);
    regOne.pinOff(vrstice[((LED-1)/5)+1]);//delay(0.5);//}
    //Serial.print((LED-1)%5+1);Serial.print("<- Stolpci || Vrstice ->");Serial.println(((LED-1)/5)+1);
  /*
   for(int i=1;i<6;i++)
  {
    regOne.pinOff(vrstice[i]);//delay(1);
    regOne.pinOff(stolpci[i]);//delay(1);
  } 
  //delay(pocakaj);
  */
}

/*void rezultat(uint8_t cifra,bool stanje,bool ekstrem)
{
  if(ekstrem && !stanje)regOne.allOff();
else{
  if(!stanje)
{//regOne.allOff();
for(int w=1;w<15;w++)
  {
    if(w<8)regOne.pinOff(enice[w]);
    else regOne.pinOff(desetice[w-7]);
  }
}

//if(!stanje)regOne.allOff();
else{
switch(cifra%10){
  
  case 0:
    regOne.pinOn(enice[1]);
    regOne.pinOn(enice[2]);
    regOne.pinOn(enice[3]);
    regOne.pinOn(enice[4]);
    regOne.pinOn(enice[5]);
    regOne.pinOn(enice[6]);
  break;

  case 1:
    regOne.pinOn(enice[2]);
    regOne.pinOn(enice[3]);
  break;  

  case 2:
    regOne.pinOn(enice[1]);
    regOne.pinOn(enice[2]);
    regOne.pinOn(enice[4]);
    regOne.pinOn(enice[5]);
    regOne.pinOn(enice[7]);   
  break;

  case 3:
    regOne.pinOn(enice[1]);
    regOne.pinOn(enice[2]);
    regOne.pinOn(enice[3]);
    regOne.pinOn(enice[4]);
    regOne.pinOn(enice[7]);
  break;  

  case 4:
    regOne.pinOn(enice[2]);
    regOne.pinOn(enice[3]);
    regOne.pinOn(enice[6]);
    regOne.pinOn(enice[7]);
  break;

  case 5:
    regOne.pinOn(enice[1]);
    regOne.pinOn(enice[3]);
    regOne.pinOn(enice[4]);
    regOne.pinOn(enice[6]);
    regOne.pinOn(enice[7]);
  break;  

  case 6:
    regOne.pinOn(enice[1]);
    regOne.pinOn(enice[3]);
    regOne.pinOn(enice[4]);
    regOne.pinOn(enice[5]);
    regOne.pinOn(enice[6]);
    regOne.pinOn(enice[7]);
  break;  

  case 7:
    regOne.pinOn(enice[1]);
    regOne.pinOn(enice[2]);
    regOne.pinOn(enice[3]);
  break; 

  case 8:
    regOne.pinOn(enice[1]);
    regOne.pinOn(enice[2]);
    regOne.pinOn(enice[3]);
    regOne.pinOn(enice[4]);
    regOne.pinOn(enice[5]);
    regOne.pinOn(enice[6]);
    regOne.pinOn(enice[7]);
  break;   

  case 9:
    regOne.pinOn(enice[1]);
    regOne.pinOn(enice[2]);
    regOne.pinOn(enice[3]);
    regOne.pinOn(enice[4]);
    regOne.pinOn(enice[6]);
    regOne.pinOn(enice[7]);
  break;  
} 
switch(cifra/10){
  
  case 0:
    regOne.pinOn(desetice[1]);
    regOne.pinOn(desetice[2]);
    regOne.pinOn(desetice[3]);
    regOne.pinOn(desetice[4]);
    regOne.pinOn(desetice[5]);
    regOne.pinOn(desetice[6]);
  break;

  case 1:
    regOne.pinOn(desetice[2]);
    regOne.pinOn(desetice[3]);
  break;  

  case 2:
    regOne.pinOn(desetice[1]);
    regOne.pinOn(desetice[2]);
    regOne.pinOn(desetice[4]);
    regOne.pinOn(desetice[5]);
    regOne.pinOn(desetice[7]);   
  break;

  case 3:
    regOne.pinOn(desetice[1]);
    regOne.pinOn(desetice[2]);
    regOne.pinOn(desetice[3]);
    regOne.pinOn(desetice[4]);
    regOne.pinOn(desetice[7]);
  break;  

  case 4:
    regOne.pinOn(desetice[2]);
    regOne.pinOn(desetice[3]);
    regOne.pinOn(desetice[6]);
    regOne.pinOn(desetice[7]);
  break;

  case 5:
    regOne.pinOn(desetice[1]);
    regOne.pinOn(desetice[3]);
    regOne.pinOn(desetice[4]);
    regOne.pinOn(desetice[6]);
    regOne.pinOn(desetice[7]);
  break;  

  case 6:
    regOne.pinOn(desetice[1]);
    regOne.pinOn(desetice[3]);
    regOne.pinOn(desetice[4]);
    regOne.pinOn(desetice[5]);
    regOne.pinOn(desetice[6]);
    regOne.pinOn(desetice[7]);
  break;  

  case 7:
    regOne.pinOn(desetice[1]);
    regOne.pinOn(desetice[2]);
    regOne.pinOn(desetice[3]);
  break; 

  case 8:
    regOne.pinOn(desetice[1]);
    regOne.pinOn(desetice[2]);
    regOne.pinOn(desetice[3]);
    regOne.pinOn(desetice[4]);
    regOne.pinOn(desetice[5]);
    regOne.pinOn(desetice[6]);
    regOne.pinOn(desetice[7]);
  break;   

  case 9:
    regOne.pinOn(desetice[1]);
    regOne.pinOn(desetice[2]);
    regOne.pinOn(desetice[3]);
    regOne.pinOn(desetice[4]);
    regOne.pinOn(desetice[6]);
    regOne.pinOn(desetice[7]);
  break; } 
}//Serial.print("Stopnja  ");Serial.println(cifra);  
}}*/

void rezultat(uint8_t cifra,uint32_t trajanje,bool ekstrem)
{//Serial.print("cifra v loopu:  ");Serial.println(cifra);
if(trajanje>2)
{
    regOne.pinOff(enice[1]);
    regOne.pinOff(enice[2]);
    regOne.pinOff(enice[3]);
    regOne.pinOff(enice[4]);
    regOne.pinOff(enice[5]);
    regOne.pinOff(enice[6]);
    regOne.pinOff(enice[7]);
    regOne.pinOff(desetice[1]);
    regOne.pinOff(desetice[2]);
    regOne.pinOff(desetice[3]);
    regOne.pinOff(desetice[4]);
    regOne.pinOff(desetice[5]);
    regOne.pinOff(desetice[6]);
    regOne.pinOff(desetice[7]);
}
  
  if(ekstrem && trajanje<2)regOne.allOff();
else{
switch(cifra%10){
  
  case 0:
    regOne.pinOn(enice[1]);
    regOne.pinOn(enice[2]);
    regOne.pinOn(enice[3]);
    regOne.pinOn(enice[4]);
    regOne.pinOn(enice[5]);
    regOne.pinOn(enice[6]);
  break;

  case 1:
    regOne.pinOn(enice[2]);
    regOne.pinOn(enice[3]);
  break;  

  case 2:
    regOne.pinOn(enice[1]);
    regOne.pinOn(enice[2]);
    regOne.pinOn(enice[4]);
    regOne.pinOn(enice[5]);
    regOne.pinOn(enice[7]);   
  break;

  case 3:
    regOne.pinOn(enice[1]);
    regOne.pinOn(enice[2]);
    regOne.pinOn(enice[3]);
    regOne.pinOn(enice[4]);
    regOne.pinOn(enice[7]);
  break;  

  case 4:
    regOne.pinOn(enice[2]);
    regOne.pinOn(enice[3]);
    regOne.pinOn(enice[6]);
    regOne.pinOn(enice[7]);
  break;

  case 5:
    regOne.pinOn(enice[1]);
    regOne.pinOn(enice[3]);
    regOne.pinOn(enice[4]);
    regOne.pinOn(enice[6]);
    regOne.pinOn(enice[7]);
  break;  

  case 6:
    regOne.pinOn(enice[1]);
    regOne.pinOn(enice[3]);
    regOne.pinOn(enice[4]);
    regOne.pinOn(enice[5]);
    regOne.pinOn(enice[6]);
    regOne.pinOn(enice[7]);
  break;  

  case 7:
    regOne.pinOn(enice[1]);
    regOne.pinOn(enice[2]);
    regOne.pinOn(enice[3]);
  break; 

  case 8:
    regOne.pinOn(enice[1]);
    regOne.pinOn(enice[2]);
    regOne.pinOn(enice[3]);
    regOne.pinOn(enice[4]);
    regOne.pinOn(enice[5]);
    regOne.pinOn(enice[6]);
    regOne.pinOn(enice[7]);
  break;   

  case 9:
    regOne.pinOn(enice[1]);
    regOne.pinOn(enice[2]);
    regOne.pinOn(enice[3]);
    regOne.pinOn(enice[4]);
    regOne.pinOn(enice[6]);
    regOne.pinOn(enice[7]);
  break;  
} 
switch(cifra/10){
  
  case 0:
    regOne.pinOn(desetice[1]);
    regOne.pinOn(desetice[2]);
    regOne.pinOn(desetice[3]);
    regOne.pinOn(desetice[4]);
    regOne.pinOn(desetice[5]);
    regOne.pinOn(desetice[6]);
  break;

  case 1:
    regOne.pinOn(desetice[2]);
    regOne.pinOn(desetice[3]);
  break;  

  case 2:
    regOne.pinOn(desetice[1]);
    regOne.pinOn(desetice[2]);
    regOne.pinOn(desetice[4]);
    regOne.pinOn(desetice[5]);
    regOne.pinOn(desetice[7]);   
  break;

  case 3:
    regOne.pinOn(desetice[1]);
    regOne.pinOn(desetice[2]);
    regOne.pinOn(desetice[3]);
    regOne.pinOn(desetice[4]);
    regOne.pinOn(desetice[7]);
  break;  

  case 4:
    regOne.pinOn(desetice[2]);
    regOne.pinOn(desetice[3]);
    regOne.pinOn(desetice[6]);
    regOne.pinOn(desetice[7]);
  break;

  case 5:
    regOne.pinOn(desetice[1]);
    regOne.pinOn(desetice[3]);
    regOne.pinOn(desetice[4]);
    regOne.pinOn(desetice[6]);
    regOne.pinOn(desetice[7]);
  break;  

  case 6:
    regOne.pinOn(desetice[1]);
    regOne.pinOn(desetice[3]);
    regOne.pinOn(desetice[4]);
    regOne.pinOn(desetice[5]);
    regOne.pinOn(desetice[6]);
    regOne.pinOn(desetice[7]);
  break;  

  case 7:
    regOne.pinOn(desetice[1]);
    regOne.pinOn(desetice[2]);
    regOne.pinOn(desetice[3]);
  break; 

  case 8:
    regOne.pinOn(desetice[1]);
    regOne.pinOn(desetice[2]);
    regOne.pinOn(desetice[3]);
    regOne.pinOn(desetice[4]);
    regOne.pinOn(desetice[5]);
    regOne.pinOn(desetice[6]);
    regOne.pinOn(desetice[7]);
  break;   

  case 9:
    regOne.pinOn(desetice[1]);
    regOne.pinOn(desetice[2]);
    regOne.pinOn(desetice[3]);
    regOne.pinOn(desetice[4]);
    regOne.pinOn(desetice[6]);
    regOne.pinOn(desetice[7]);
  break; }
  if(trajanje>2){
for(int q=0;q<trajanje;q++){ponovitve+=q;if(ponovitve>7)ponovitve-=7;
 //prvo vklop, pol pwm 
  switch(cifra%10){
  case 0:
if(ponovitve==1){regOne.pinOff(enice[1]);delay(1);regOne.pinOn(enice[1]);}
else if(ponovitve==2){regOne.pinOff(enice[2]);delay(1);regOne.pinOn(enice[2]);}
else if(ponovitve==3){regOne.pinOff(enice[3]);delay(1);regOne.pinOn(enice[3]);}
else if(ponovitve==4){regOne.pinOff(enice[4]);delay(1);regOne.pinOn(enice[4]);}
else if(ponovitve==5){regOne.pinOff(enice[5]);delay(1);regOne.pinOn(enice[5]);}
else if(ponovitve==6){regOne.pinOff(enice[6]);delay(1);regOne.pinOn(enice[6]);}
  break;

  case 1:
if(ponovitve==1){regOne.pinOff(enice[2]);delay(2);regOne.pinOn(enice[2]);}
else if(ponovitve==2){regOne.pinOff(enice[3]);delay(2);regOne.pinOn(enice[3]);}
  break;  

  case 2:
if(ponovitve==1){regOne.pinOff(enice[1]);delay(2);regOne.pinOn(enice[1]);}
else if(ponovitve==2){regOne.pinOff(enice[2]);delay(2);regOne.pinOn(enice[2]);}
else if(ponovitve==3){regOne.pinOff(enice[4]);delay(2);regOne.pinOn(enice[4]);}
else if(ponovitve==4){regOne.pinOff(enice[5]);delay(2);regOne.pinOn(enice[5]);}
else if(ponovitve==5){regOne.pinOff(enice[7]);delay(2);regOne.pinOn(enice[7]);}
  break;

  case 3:
if(ponovitve==1){regOne.pinOff(enice[1]);delay(2);regOne.pinOn(enice[1]);}
else if(ponovitve==2){regOne.pinOff(enice[2]);delay(2);regOne.pinOn(enice[2]);}
else if(ponovitve==3){regOne.pinOff(enice[3]);delay(2);regOne.pinOn(enice[3]);}
else if(ponovitve==4){regOne.pinOff(enice[4]);delay(2);regOne.pinOn(enice[4]);}
else if(ponovitve==5){regOne.pinOff(enice[7]);delay(2);regOne.pinOn(enice[7]);}
  break;  

  case 4:
if(ponovitve==1){regOne.pinOff(enice[2]);delay(2);regOne.pinOn(enice[2]);}
else if(ponovitve==2){regOne.pinOff(enice[3]);delay(2);regOne.pinOn(enice[3]);}
else if(ponovitve==3){regOne.pinOff(enice[6]);delay(2);regOne.pinOn(enice[6]);}
else if(ponovitve==4){regOne.pinOff(enice[7]);delay(2);regOne.pinOn(enice[7]);}
  break;

  case 5:
if(ponovitve==1){regOne.pinOff(enice[1]);delay(2);regOne.pinOn(enice[1]);}
else if(ponovitve==2){regOne.pinOff(enice[3]);delay(2);regOne.pinOn(enice[3]);}
else if(ponovitve==3){regOne.pinOff(enice[4]);delay(2);regOne.pinOn(enice[4]);}
else if(ponovitve==4){regOne.pinOff(enice[6]);delay(2);regOne.pinOn(enice[6]);}
else if(ponovitve==5){regOne.pinOff(enice[7]);delay(2);regOne.pinOn(enice[7]);}
  break;  

  case 6:
if(ponovitve==1){regOne.pinOff(enice[1]);delay(2);regOne.pinOn(enice[1]);}
else if(ponovitve==2){regOne.pinOff(enice[3]);delay(2);regOne.pinOn(enice[3]);}
else if(ponovitve==3){regOne.pinOff(enice[4]);delay(2);regOne.pinOn(enice[4]);}
else if(ponovitve==4){regOne.pinOff(enice[5]);delay(2);regOne.pinOn(enice[5]);}
else if(ponovitve==5){regOne.pinOff(enice[6]);delay(2);regOne.pinOn(enice[6]);}
else if(ponovitve==6){regOne.pinOff(enice[7]);delay(2);regOne.pinOn(enice[7]);}
  break;  

  case 7:
if(ponovitve==1){regOne.pinOff(enice[1]);delay(2);regOne.pinOn(enice[1]);}
else if(ponovitve==2){regOne.pinOff(enice[2]);delay(2);regOne.pinOn(enice[2]);}
else if(ponovitve==3){regOne.pinOff(enice[3]);delay(2);regOne.pinOn(enice[3]);}
  break; 

  case 8:
if(ponovitve==1){regOne.pinOff(enice[1]);delay(2);regOne.pinOn(enice[1]);}
else if(ponovitve==2){regOne.pinOff(enice[2]);delay(2);regOne.pinOn(enice[2]);}
else if(ponovitve==3){regOne.pinOff(enice[3]);delay(2);regOne.pinOn(enice[3]);}
else if(ponovitve==4){regOne.pinOff(enice[4]);delay(2);regOne.pinOn(enice[4]);}
else if(ponovitve==5){regOne.pinOff(enice[5]);delay(2);regOne.pinOn(enice[5]);}
else if(ponovitve==6){regOne.pinOff(enice[6]);delay(2);regOne.pinOn(enice[6]);}
else if(ponovitve==7){regOne.pinOff(enice[7]);delay(2);regOne.pinOn(enice[7]);}
  break;   

  case 9:
if(ponovitve==1){regOne.pinOff(enice[1]);delay(2);regOne.pinOn(enice[1]);}
else if(ponovitve==2){regOne.pinOff(enice[2]);delay(2);regOne.pinOn(enice[2]);}
else if(ponovitve==3){regOne.pinOff(enice[3]);delay(2);regOne.pinOn(enice[3]);}
else if(ponovitve==4){regOne.pinOff(enice[4]);delay(2);regOne.pinOn(enice[4]);}
else if(ponovitve==5){regOne.pinOff(enice[6]);delay(2);regOne.pinOn(enice[6]);}
else if(ponovitve==6){regOne.pinOff(enice[7]);delay(2);regOne.pinOn(enice[7]);}
  break; } 
 
switch(cifra/10){
  
  case 0:
if(ponovitve==1){regOne.pinOff(desetice[1]);delay(1);regOne.pinOn(desetice[1]);}
else if(ponovitve==2){regOne.pinOff(desetice[2]);delay(1);regOne.pinOn(desetice[2]);}
else if(ponovitve==3){regOne.pinOff(desetice[3]);delay(1);regOne.pinOn(desetice[3]);}
else if(ponovitve==4){regOne.pinOff(desetice[4]);delay(1);regOne.pinOn(desetice[4]);}
else if(ponovitve==5){regOne.pinOff(desetice[5]);delay(1);regOne.pinOn(desetice[5]);}
else if(ponovitve==6){regOne.pinOff(desetice[6]);delay(1);regOne.pinOn(desetice[6]);}
  break;

  case 1:
if(ponovitve==1){regOne.pinOff(desetice[2]);delay(2);regOne.pinOn(desetice[2]);}
else if(ponovitve==2){regOne.pinOff(desetice[3]);delay(2);regOne.pinOn(desetice[3]);}
  break;  

  case 2:
if(ponovitve==1){regOne.pinOff(desetice[1]);delay(2);regOne.pinOn(desetice[1]);}
else if(ponovitve==2){regOne.pinOff(desetice[2]);delay(2);regOne.pinOn(desetice[2]);}
else if(ponovitve==3){regOne.pinOff(desetice[4]);delay(2);regOne.pinOn(desetice[4]);}
else if(ponovitve==4){regOne.pinOff(desetice[5]);delay(2);regOne.pinOn(desetice[5]);}
else if(ponovitve==5){regOne.pinOff(desetice[7]);delay(2);regOne.pinOn(desetice[7]);}
  break;

  case 3:
if(ponovitve==1){regOne.pinOff(desetice[1]);delay(2);regOne.pinOn(desetice[1]);}
else if(ponovitve==2){regOne.pinOff(desetice[2]);delay(2);regOne.pinOn(desetice[2]);}
else if(ponovitve==3){regOne.pinOff(desetice[3]);delay(2);regOne.pinOn(desetice[3]);}
else if(ponovitve==4){regOne.pinOff(desetice[4]);delay(2);regOne.pinOn(desetice[4]);}
else if(ponovitve==5){regOne.pinOff(desetice[7]);delay(2);regOne.pinOn(desetice[7]);}
  break;  

  case 4:
if(ponovitve==1){regOne.pinOff(desetice[2]);delay(2);regOne.pinOn(desetice[2]);}
else if(ponovitve==2){regOne.pinOff(desetice[3]);delay(2);regOne.pinOn(desetice[3]);}
else if(ponovitve==3){regOne.pinOff(desetice[6]);delay(2);regOne.pinOn(desetice[6]);}
else if(ponovitve==4){regOne.pinOff(desetice[7]);delay(2);regOne.pinOn(desetice[7]);}
  break;

  case 5:
if(ponovitve==1){regOne.pinOff(desetice[1]);delay(2);regOne.pinOn(desetice[1]);}
else if(ponovitve==2){regOne.pinOff(desetice[3]);delay(2);regOne.pinOn(desetice[3]);}
else if(ponovitve==3){regOne.pinOff(desetice[4]);delay(2);regOne.pinOn(desetice[4]);}
else if(ponovitve==4){regOne.pinOff(desetice[6]);delay(2);regOne.pinOn(desetice[6]);}
else if(ponovitve==5){regOne.pinOff(desetice[7]);delay(2);regOne.pinOn(desetice[7]);}
  break;  

  case 6:
if(ponovitve==1){regOne.pinOff(desetice[1]);delay(2);regOne.pinOn(desetice[1]);}
else if(ponovitve==2){regOne.pinOff(desetice[3]);delay(2);regOne.pinOn(desetice[3]);}
else if(ponovitve==3){regOne.pinOff(desetice[4]);delay(2);regOne.pinOn(desetice[4]);}
else if(ponovitve==4){regOne.pinOff(desetice[5]);delay(2);regOne.pinOn(desetice[5]);}
else if(ponovitve==5){regOne.pinOff(desetice[6]);delay(2);regOne.pinOn(desetice[6]);}
else if(ponovitve==6){regOne.pinOff(desetice[7]);delay(2);regOne.pinOn(desetice[7]);}
  break;  

  case 7:
if(ponovitve==1){regOne.pinOff(desetice[1]);delay(2);regOne.pinOn(desetice[1]);}
else if(ponovitve==2){regOne.pinOff(desetice[2]);delay(2);regOne.pinOn(desetice[2]);}
else if(ponovitve==3){regOne.pinOff(desetice[3]);delay(2);regOne.pinOn(desetice[3]);}
  break; 

  case 8:
if(ponovitve==1){regOne.pinOff(desetice[1]);delay(2);regOne.pinOn(desetice[1]);}
else if(ponovitve==2){regOne.pinOff(desetice[2]);delay(2);regOne.pinOn(desetice[2]);}
else if(ponovitve==3){regOne.pinOff(desetice[3]);delay(2);regOne.pinOn(desetice[3]);}
else if(ponovitve==4){regOne.pinOff(desetice[4]);delay(2);regOne.pinOn(desetice[4]);}
else if(ponovitve==5){regOne.pinOff(desetice[5]);delay(2);regOne.pinOn(desetice[5]);}
else if(ponovitve==6){regOne.pinOff(desetice[6]);delay(2);regOne.pinOn(desetice[6]);}
else if(ponovitve==7){regOne.pinOff(desetice[7]);delay(2);regOne.pinOn(desetice[7]);}
  break;   

  case 9:
if(ponovitve==1){regOne.pinOff(desetice[1]);delay(2);regOne.pinOn(desetice[1]);}
else if(ponovitve==2){regOne.pinOff(desetice[2]);delay(2);regOne.pinOn(desetice[2]);}
else if(ponovitve==3){regOne.pinOff(desetice[3]);delay(2);regOne.pinOn(desetice[3]);}
else if(ponovitve==4){regOne.pinOff(desetice[4]);delay(2);regOne.pinOn(desetice[4]);}
else if(ponovitve==5){regOne.pinOff(desetice[6]);delay(2);regOne.pinOn(desetice[6]);}
else if(ponovitve==6){regOne.pinOff(desetice[7]);delay(2);regOne.pinOn(desetice[7]);}
  break; } 
  }
}}//Serial.print("Stopnja  ");Serial.println(cifra);  
}

void izziv(int vrednost,int mesto)
{
cilj[mesto]=vrednost;
//stopnja++;
  for(int i=1;i<6;i++)
  {
    regOne.pinOff(vrstice[i]);//delay(1);
    regOne.pinOn(stolpci[i]);//delay(1);
  }
for(int t=0;t<(stopnja+1);t++)//animacija za Simon Says
  {
  //for(int b=0;b<80;b++)lucke(cilj[t],0.5); //cifra v b-for loopu diktira koliko dolgo je LED prižgana
  lucke(cilj[t],150000,0);
  pwm_zaslon(100000,0);//cajt med prizigom luck
  }
}

int igra()
{int tipka=0;//stopnja=0;
while(!izguba){
randomSeed(analogRead(A3));//Serial.print("stopnja:");Serial.println(stopnja);
//rezultat(stopnja,0);delay(2);rezultat(stopnja,1);
//pwm_zaslon(300,0);
izziv(random(1,26),stopnja);
//Serial.println("New Level");
for(int runda=0;(runda<(stopnja+1))&&(izguba!=1);runda++)
  {
  delay(10);tipka=26;
  while(tipka>25){
    //rezultat(stopnja,0);
  izbor[runda]=branje_tipkovnice(); //if(zaslon!=1)pwm_zaslon(0);
  pwm_zaslon(0,0);
  tipka=izbor[runda];
  //Serial.println("cakam");
   /* rezultat(stopnja,1)*/;}
 // Serial.print("Izbrana tipka:");Serial.println(tipka);
    for(int i=1;i<6;i++) //ker omogocimo all.Off pr tipkovnici
  {
    regOne.pinOff(vrstice[i]);//delay(1);
    regOne.pinOn(stolpci[i]);//delay(1);
  }
  lucke(tipka,60000,0);
 if(izbor[runda]!=cilj[runda]){
  delay(2);Serial.println("Game Over");animacija_tipkovnice(1);izguba=1;zacasno=0;stanje=0;prej=0;}
  else{izguba=0;/*Serial.println("Bravo");*/pwm_zaslon(500,0);/*delay(2);*/}/*rezultat(stopnja,1)*/
  }if(izguba!=1)stopnja=stopnja+1;pwm_zaslon(32000,0);
  }izguba=0;
}

void animacija_tipkovnice(int mode){
  float pavza=0.5*4;
  switch(mode){
    case 1:
      for(int q=0;q<40;q++){//šopanje PWMja
        lucke(1,pavza,1);
        lucke(5,pavza,1);
        lucke(7,pavza,1);
        lucke(9,pavza,1);
        lucke(13,pavza,1);
        lucke(17,pavza,1);
        lucke(19,pavza,1);
        lucke(21,pavza,1);
        lucke(25,pavza,1);
      }
    break;

    case 2:
    break;
    }
  }

void pwm_zaslon(uint32_t duration, bool izjema)
{
   // for(int p=0;p<(duration+1);p++)
   // { 
   //   zaslon=!zaslon;
      rezultat(stopnja,duration,izjema);  
   // }
    if (duration>2)rezultat(stopnja,0,izjema);
}

void loop() 
{ //Serial.println(branje_tipkovnice());
  //if (zacasno) igra();
 /* if(millis()-kdelay>period) //used to make non-blocking delay
  {
    kdelay=millis();  //capture time from millis function
  Serial.println(branje_tipkovnice());}*/

  //rezultat(stopnja,1,0);
  if (zacasno) {stopnja=0;digitalWrite(START_LED,0);/*delay(1500)*/pwm_zaslon(200000,0);igra();}
  //if((millis()-prej)>=400){prej=millis();stanje=!stanje;} //400 je hitrost utripanja LED [50%PWM]
  for(int ww=0;ww<40;ww++){pwm_zaslon(1,0);}
  stanje=!stanje;pwm_zaslon(1,0);
  digitalWrite(START_LED,stanje);//Serial.print("prej:");Serial.print(prej);Serial.print("  millis:");Serial.println(millis());
  //delay(0.5);rezultat(stopnja,1,1);
 // delay(1);

//for(int c=1;c<26;c++)lucke(c,200);

  
//animacija_tipkovnice(1);delay(2000);
 
//if(branje_tipkovnice()==13) {while(1){animacija_tipkovnice();}}
//for(int i=0;i<100;i++){
//rezultat(i);delay(200);}

/*randomSeed(analogRead(A3));
rezultat(stopnja);
izziv(random(1,26),stopnja);
igra();*/
}
