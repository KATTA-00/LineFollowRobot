int ir[7];
int th = 500;


float i; int d;
float error;
int p_error = 0;

int val[7] = {-40,-15,-10,0,10,15,40};
float pid_val ;

int sp_left = 150 + 10;
int sp_right = 150;
int shift = 0;
int left = sp_left ;
int right = sp_right ;

int kp = 3 + (sp_left+sp_right)/80;
float ki = 0.01;
float kd = 0.08;

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
  pinMode(8,OUTPUT);
  digitalWrite(8,HIGH);
}

void loop() {
  getIR();
  PID();
  driver();
  delay(10);
  Serial.print(ir[0]);Serial.print(ir[1]);Serial.print(ir[2]);Serial.print(ir[3]);Serial.print(ir[4]);Serial.print(ir[5]);Serial.println(ir[6]);
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
  for (int i=1;i<6;i++){
    ir[i] = analogRead(i);
  }
  for (int i=1;i<6;i++){
    ir[i] = check(ir[i]);
  }
  if (check(analogRead(0)) == 1){
    ir[0] = 0;
    }else{
    ir[0] = 1;
   }
  if (check(analogRead(6)) == 1){
    ir[6] = 0;
    }else{
    ir[6] = 1;
   }
  
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// PID ///////////////////////////////
float PID(){
  left = sp_left;
  right = sp_right;
  
  error = val[0]*ir[0] + val[1]*ir[1] + val[2]*ir[2] + val[3]*ir[3] + val[4]*ir[4] + val[5]*ir[5] + val[6]*ir[6];
  d = p_error - error;
  p_error = error;

  if (ir[3] == 1){
    i = 0;
  } else if ((left+i*ki)>0 && error<0){
    i = (i  + error);
  }else if ((right-i*ki)>0 && error>0){
    i = (i  + error);
  }
  
  pid_val = (error * kp + i * ki + d * kd);
  
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
}
/////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void driver(){
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
   
 Serial.print(left);Serial.print(' ');Serial.print(right);Serial.print(' ');Serial.print(i);Serial.print(' ');Serial.print(pid_val);Serial.print(' ');Serial.println(error);
 }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
