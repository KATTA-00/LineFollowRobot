int ir[5];
int th = 500;


float i; int d;
float error;
int p_error = 0;

int val[5] = {-20,-9,0,9,20};
float pid_val ;

int sp_left = 125 + 10;
int sp_right = 125;
int shift = 0;
int left ;
int right ;

int kp = 3 + (sp_left+sp_right)/80;
float ki = 0.03;
float kd = 0.0;

int right_for = 11;
int left_for = 5;
int right_back = 10;
int left_back = 6;


void setup() {
  Serial.begin(9600);
  pinMode(5,OUTPUT);pinMode(6,OUTPUT);pinMode(10,OUTPUT);pinMode(11,OUTPUT);
}

void loop() {
  getIR();
  PID();
  driver();
  delay(10);
  //Serial.print(ir[0]);Serial.print(ir[1]);Serial.print(ir[2]);Serial.print(ir[3]);Serial.println(ir[4]);
  //Serial.println(error);Serial.println(i);
}


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


float PID(){
  if (ir[0] == 1 || ir[4] == 1){
    shift = -20;
  } else{
    shift = 0;
  }
  
  error = val[0]*ir[0] + val[1]*ir[1] + val[2]*ir[2] + val[3]*ir[3] + val[4]*ir[4];
  i = (i  + error);
  d = p_error - error;
  p_error = error;

  pid_val = (error * kp + i * ki + d * kd);
  
  if (ir[2] == 1){
    i = 0;
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
}

void driver(){
  left = sp_left;
  right = sp_right;
  
 left = left + pid_val + shift ;
 if (left > 255){
  left = 255;
 }else if(left < -255){
  left = -255;
 }
 if (left>0){
   analogWrite(left_for,left);
   analogWrite(left_back,0);
 } else if(left<0){
   analogWrite(left_back,-left);
   analogWrite(left_for,0);
 }

 right = right - pid_val + shift;
 if (right > 255){
  right = 255;
 }else if(right < -255){
  right = -255;
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
