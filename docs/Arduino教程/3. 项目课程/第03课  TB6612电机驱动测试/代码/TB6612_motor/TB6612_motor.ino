//TB6612引脚定义
const int right_R1=8;    
const int right_R2=12;
const int PWM_R=10;
const int left_L1=7;
const int left_L2=6;
const int PWM_L=9;

void setup() 
{
  Serial.begin(9600);          //串口波特率为9600
  
  pinMode(right_R1,OUTPUT);     //TB6612的引脚都设置为输出
  pinMode(right_R2,OUTPUT);
  pinMode(PWM_R,OUTPUT);
  pinMode(left_L1,OUTPUT);
  pinMode(left_L2,OUTPUT);
  pinMode(PWM_L,OUTPUT);
}

void loop() 
{
  //两电机都正转
  digitalWrite(right_R1,HIGH);
  digitalWrite(right_R2,LOW);
  digitalWrite(left_L1,HIGH);
  digitalWrite(left_L2,LOW);
  analogWrite(PWM_R,100);   //写入PWM值0~255（速度）
  analogWrite(PWM_L,100);
}
