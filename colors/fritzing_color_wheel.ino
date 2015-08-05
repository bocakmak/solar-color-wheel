#include <math.h>

#define RED_GROUND 3
#define MAIN_POWER 4
#define GREEN_GROUND 5
#define BLUE_GROUND 6

#define MAX_COLORS 8
#define MAX_COLOR_LOOP 8

#define MAIN_DELAY 2
#define HOLD_DELAY 12
#define FADE_DELAY 10

#define DEBUG_PIN 12

int R = 255;

int G = 255;

int B = 255;


//DEFINE RGB STRUCT
struct rgbColors {
  int R;
  int G;
  int B;
};

rgbColors WheelColors[MAX_COLOR_LOOP] = {
  {253,3,3},
  {255,128,253},
  {255,223,4},
  {56,255,4},
  {90,214,245},
  {153,75,213},
  {244,168,0},
  {248,229,140}
};

int getColor(int sc, int ec, int incr, int total);

void setRGB(int R,int G,int B);

void colorWheel(void);

void setup() {
  
  Serial.begin(9600);
  pinMode(DEBUG_PIN,INPUT);
  pinMode(RED_GROUND,OUTPUT);
  pinMode(GREEN_GROUND,OUTPUT);
  pinMode(BLUE_GROUND,OUTPUT);
  pinMode(MAIN_POWER,OUTPUT);
  
  digitalWrite(DEBUG_PIN,HIGH);
  digitalWrite(MAIN_POWER,HIGH);
  digitalWrite(RED_GROUND,LOW);
  digitalWrite(GREEN_GROUND,LOW);
  digitalWrite(BLUE_GROUND,LOW);
}


void loop() {
  colorWheel();
  delay(MAIN_DELAY);
}

void colorWheel(void){

  int i;

  int total;
  
  int next_i;
  
  int R,G,B;
  
  int max_range;
  
  int start;
  
  for(int i=0;i<MAX_COLOR_LOOP;i++){
    
    if( i<(MAX_COLOR_LOOP-1) ){
      next_i = (i+1);
    } else {
      next_i = 0;
    }
    
    R = WheelColors[i].R;
    G = WheelColors[i].G;
    B = WheelColors[i].B;
    
    setRGB(R,G,B);
    
    
    max_range = 255;
    
    start = 0;
    
    while(true){
      if( (R == WheelColors[next_i].R) && (G == WheelColors[next_i].G) && (B ==  WheelColors[next_i].B) ){
          delay(HOLD_DELAY);
          break;
        }
        start++;
        
        setRGB(R,G,B);
        
        R = getColor(R, WheelColors[next_i].R, start,max_range);
        
        G = getColor(G,WheelColors[next_i].G, start,max_range);
        
        B = getColor(B,WheelColors[next_i].B, start,max_range);
        
        setRGB(R,G,B);
        
        if( Serial.available() && (bool)(analogRead(DEBUG_PIN) >= 0) ){
          Serial.print("R,G,B");
          Serial.print(R);
          Serial.print(" , ");
          Serial.print(G);
          Serial.print(" , ");
          Serial.print(B);
          Serial.println(" , ");
          Serial.print(start);
          Serial.print(" , ");
        }
        
        delay(FADE_DELAY);
        if((start==max_range)){
          start = -254;
          continue;
        }
    }
  }
}

int getColor(int sc, int ec, int incr, int total){

  int color = (int)sc + (int)(((ec-sc)*incr)/total);

  return color;
}

void setRGB(int R,int G,int B){
  
  analogWrite(RED_GROUND, abs(255-R) );
  
  analogWrite(GREEN_GROUND, abs(255-G) );
  
  analogWrite(BLUE_GROUND, abs(255-B) );
}
