#include <Arduino.h>

void V_switch(uint8_t num);
IntervalTimer Timer;
 uint8_t i=0;
 char var = 0;
bool updown = false;

 volatile bool flag =false;

 void SetFlag(){
     flag = true;
 }

uint8_t array[16][4]={0,	0,	0,	0,
0,	0,	0,	1,
0,	0,	1,	0,
0,	0,	1,	1,
0,	1,	0,	0, 
0,	1,	0,	1,
0,	1,	1,	0,
0,	1,	1,	1,
1,	0,	0,	0,
1,	0,	0,	1,
1,	0,	1,	0,
1,	0,	1,	1,
1,	1,	0,	0,
1,	1,	0,	1,
1,	1,	1,	0,
1,	1,	1, 1
};


uint8_t sine[32]{0,
0,
1,
2,
3,
4,
6,
7,
9,
10,
12,
13,
14,
15,
15,
15,
15,
15,
15,
14,
13,
12,
10,
9,
7,
6,
4,
3,
2,
1,
0,
0
};


void setup() {
    Serial.begin(9600);
    pinMode(9,OUTPUT);
    pinMode(10,OUTPUT);
    pinMode(11,OUTPUT);
    pinMode(12,OUTPUT);
    Timer.begin(SetFlag, 3125);
    // put your setup code here, to run once:
}

void loop() {
    

    //Serial.println(Serial.available());
    //if(Serial.available()>0){
    //   i=Serial.parseInt();
     //  Serial.println(i);   
    //}
    
   
    
    if(flag){
            noInterrupts();
            flag = false;
            interrupts(); 
            Serial.print(i);
            for(int u= 0;u < 4;u++){


                digitalWriteFast(9+u,array[sine[i]][u]);
                
                
                //Serial.print(array[i][u]);
                
            }
           // Serial.println();
           if(i == 31){
                updown = true;
            } 
            if(i ==0){
                updown = false;
            }
           if(!updown){
                i++;
           }
           else{
               i--;
           }
    }
    
    // put your main code here, to run repeatedly:
}

