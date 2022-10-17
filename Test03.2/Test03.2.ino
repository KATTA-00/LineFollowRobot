int ir[5];
int count = 10;
float er[10];
int th = 500;


float i; int d;
float error;
int p_error = 0;

int val[5] = {-12,-8,0,8,12};
float pid_val ;
float t1; 
float t=20;

float sp_left = 125 + 15;
float sp_right = 125;
int shift = 0;
float left = sp_left ;
float right = sp_right ;
float gear1 = 0.8;
float gear2 = 0.6;


int kp = 3.5 + (sp_left+sp_right)/100;
float ki = 0.01;
float kd = 1;

int right_for = 11;
int left_for = 5;
int right_back = 10;
int left_back = 6;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////// MAIN /////////////////
void setup() {
  Serial.begin(9600);
  pinMode(5,OUTPUT);pinMode(6,OUTPUT);pinMode(10,OUTPUT);pinMode(11,OUTPUT);

  for (int i = 0;i<count;i++){
    er[i] = 0.0;
  }
  t1 = millis();
}

void loop() {
  getIR();
  PID();
  driver();
  delay(10);
  //Serial.print(ir[0]);Serial.print(ir[1]);Serial.print(ir[2]);Serial.print(ir[3]);Serial.println(ir[4]);
  //Serial.println(error);Serial.println(i);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////// SENSOR ////////////////////////////////
int check(int val){
  if (val >= th){
    return 0;
  } else{
    return 1;
  }
}
int getIR(){
  for (int i=0;i<5;i++){
    ir[i] = analogRead(i);
  }
  for (int i=0;i<5;i++){
    ir[i] = check(ir[i]);
  }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// PID ///////////////////////////////
float PID(){
  left = sp_left;
  right = sp_right;
  
  error = val[0]*ir[0] + val[1]*ir[1] + val[2]*ir[2] + val[3]*ir[3] + val[4]*ir[4];
  d = p_error - error;

  if ((millis()-t1)> t){
  for (int i = 0;i<count-1;i++){
    er[i+1]  = er[i];
  }
  er[0] = error;
  t1 = millis();
  Serial.println(t1);
  }
  
  for (int i = 0;i<count-1;i++){
    p_error = er[i] + p_error;
  }
  p_error = p_error/count ;

  if ((ir[2] == 1) && (ir[2]*ir[1] == 0 && ir[2]*ir[3] == 0)){
    i = 0;
  }
  if ((left+i*ki)>0 && error<0){
    i = (i  + error);
  }else if ((right-i*ki)>0 && error>0){
    i = (i  + error);
  }

  if (i<-30000){
    i = -30000;
  }
  if (pid_val<-30000){
    pid_val = -30000;
  }
  if (i>30000){
    i = 30000;
  }
  if (pid_val>30000){
    pid_val = 30000;
  }
  
  pid_val = (error * kp + i * ki + d * kd);
  
}
/////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void driver(){
    //gear();
    left = sp_left;
    right = sp_right;
    
   ////////////////////////////////////////////////////////////////////////////
   left = left + pid_val  ;
   right = right - pid_val ;
   ///////////////////////////////////////////////////////////////////
     
     if (left > 255){
      left = 255;
     }else if(left < -255){
      left = -255;
     }
     if (right > 255){
      right = 255;
     }else if(right < -255){
      right = -255;
     }

     if (left>0){
       analogWrite(left_for,left);
       analogWrite(left_back,0);
     } else if(left<0){
       analogWrite(left_back,-left);
       analogWrite(left_for,0);
     }
     if (right>0){
       analogWrite(right_for,right);
       analogWrite(right_back,0);
     } else if(right<0){
       analogWrite(right_back,right);
       analogWrite(right_for,0);
     }
   
 //Serial.print(left);Serial.print(' ');Serial.print(right);Serial.print(' ');Serial.print(i);Serial.print(' ');Serial.print(pid_val);Serial.print(' ');Serial.println(error);
 
 }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void gear(){
  sp_left = 80 + 10;
  sp_right = 80;
  if ((ir[2]*ir[1] == 1 || ir[2]*ir[3] == 1 )){
    sp_left = sp_left * gear1;
    sp_right = sp_right * gear1;
  } else if ((ir[0]*ir[1] == 1 || ir[4]*ir[3] == 1 )){
    sp_left = sp_left * gear2;
    sp_right = sp_right * gear2;
  }
}
