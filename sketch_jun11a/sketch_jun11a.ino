int ir[5];
int th = 500;

int kp = 3;
float ki = 0.0;
float kd = 0.0;
int i; int d;
int error;
int p_error = 0;
int val[5] = {-2500,-1000,0,1000,2500};
float pid_val ;

int sp = 100;
int c = 0;
int left = sp;
int right = sp;


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
  //analogWrite(11,100);
  //digitalWrite(5,HIGH);
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
  error = val[0]*ir[0] + val[1]*ir[1] + val[2]*ir[2] + val[3]*ir[3] + val[4]*ir[4];
  i = i + error;
  if (ir[2] == 1){
    i = 0;
  }
  d = p_error - error;
  p_error = error;

  pid_val = error * kp + i * ki + d * kd;

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
  int left = sp +10;
  int right = sp;
 left = left + pid_val/100 + c ;
 if (left>0){
   analogWrite(5,left);
   analogWrite(6,0);
 } else if(left<0){
   analogWrite(6,-left);
   analogWrite(5,0);
 }

 right = right - pid_val/100 + c;
 if (right>0){
   analogWrite(11,right);
   analogWrite(10,0);
 } else if(right<0){
   analogWrite(10,right);
   analogWrite(11,0);
 }
 Serial.print(left);Serial.print(' ');Serial.print(right);Serial.print(' ');Serial.print(i);Serial.print(' ');Serial.print(pid_val);Serial.print(' ');Serial.println(error);
 }
