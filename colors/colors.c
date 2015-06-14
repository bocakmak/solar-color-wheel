#include <math.h>
#define RED_GROUND 3
#define MAIN_POWER 4
#define GREEN_GROUND 5
#define BLUE_GROUND 6
#define MAX_COLORS 8
#define MAX_COLOR_LOOP 8

//Use positive ints to maintain correct RGB display
int R = 255;

int G = 255;

int B = 255;

//Create RGB Data Struct
struct rgbColors {
  int R;
  int G;
  int B;
};

//Main Wheel Data Object
rgbColors WheelColors[MAX_COLOR_LOOP] = {
  {253,3,3},
  {255,128,4},
  {255,223,4},
  {56,255,4},
  {90,214,245},
  {153,75,213},
  {244,168,247},
  {248,229,140}
};

int findDiff(int r,int g,int b, int num_one,int num_two,int num_three);

int getColor(int sc, int ec, int incr, int total);

void setRGB(int R,int G,int B);

void colorWheel(void);

void checkLight(void);

void setup() {
  
  Serial.begin(9600);
  
  pinMode(RED_GROUND,OUTPUT);
  pinMode(GREEN_GROUND,OUTPUT);
  pinMode(BLUE_GROUND,OUTPUT);
  pinMode(MAIN_POWER,OUTPUT);

  digitalWrite(MAIN_POWER,HIGH);
  digitalWrite(RED_GROUND,LOW);
  digitalWrite(GREEN_GROUND,LOW);
  digitalWrite(BLUE_GROUND,LOW);
}


void loop() {
  
  colorWheel();

  delay(4);

}

void colorWheel(void){

  int i;

  int total;
  
  int next_i;
  
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
    
    int range = findDiff(R,G,B,WheelColors[next_i].R,WheelColors[next_i].G,WheelColors[next_i].B);
    
    int max_range = 255;
    
    int start = 0;
    
    while(true){
      if( (R == WheelColors[next_i].R) && (G == WheelColors[next_i].G) && (B ==  WheelColors[next_i].B) ){
          delay(2500);
          break;
        }
        start++;
        
        setRGB(R,G,B);
        
        R = getColor(R, WheelColors[next_i].R, start,max_range);
        
        G = getColor(G,WheelColors[next_i].G, start,max_range);
        
        B = getColor(B,WheelColors[next_i].B, start,max_range);
        
        setRGB(R,G,B);

        //UnComment for Debugging

        /**
        if( Serial.available() ){
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
        **/

        delay(25);
        if((start==max_range)){
          start = -254;
          continue;
        }
    }
    checkLight();
  }
}

int findDiff(int r,int g,int b, int num_one,int num_two,int num_three){
  
  int r_diff = abs(abs( r )-abs( num_one ) );
  int b_diff = abs(abs( b )-abs( num_two ) );
  int g_diff = abs(abs( g )-abs( num_three ) );
  
  if( (r_diff > b_diff) && (r_diff > g_diff) ){
    return r_diff;
  } else if( (b_diff > r_diff) && (b_diff > g_diff) ){
    return b_diff;
  } else if((g_diff > r_diff) && (g_diff > b_diff)){
    return g_diff;
  } else {
    return r_diff;
  }
  return r_diff;
}

int getColor(int sc, int ec, int incr, int total){

  int color = sc + (((ec-sc)*incr)/total);

  return color;
}

void setRGB(int R,int G,int B){
  
  analogWrite(RED_GROUND, abs(255-R) );
  
  analogWrite(GREEN_GROUND, abs(255-G) );
  
  analogWrite(BLUE_GROUND, abs(255-B) );
}

void checkLight(void){
  
  double l;
  int d;
  l = analogRead(LIGHT_PIN);
  
  Serial.print("Light Reading: ");
  Serial.println(l);
    
    if(l<=300.00){
      d = digitalRead(MAIN_POWER);
      if(d==0){
        digitalWrite(MAIN_POWER,HIGH);
      }
    } else if(l>=300.00) {
      d = digitalRead(MAIN_POWER);
      if(d==1){
        digitalWrite(MAIN_POWER,LOW);
      }
    }
}