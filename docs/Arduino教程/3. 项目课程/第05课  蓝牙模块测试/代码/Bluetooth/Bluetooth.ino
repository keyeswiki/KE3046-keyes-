//TB6612引脚定义
const int right_R1=8;    
const int right_R2=12;
const int PWM_R=10;
const int left_L1=7;
const int left_L2=6;
const int PWM_L=9;
char val;   //蓝牙变量

void setup() 
{
  Serial.begin(9600);
  
  pinMode(right_R1,OUTPUT);   //TB6612的引脚都设置为输出
  pinMode(right_R2,OUTPUT);
  pinMode(PWM_R,OUTPUT);
  pinMode(left_L1,OUTPUT);
  pinMode(left_L2,OUTPUT);
  pinMode(PWM_L,OUTPUT);
}

void loop() 
{
  if(Serial.available())    //如果串口缓存区有数值
  {
    val = Serial.read();      //将串口上读取到的值赋给变量val
    Serial.println(val);
    switch(val)             //switch语句
    {
      case 'F': front(); break;     //电机正转
      case 'B': back(); break;       //反转
      case 'S': Stop();break;    //停止
    }
  }
}

//电机正转
void front()
{
  digitalWrite(right_R1,HIGH);
  digitalWrite(right_R2,LOW);
  digitalWrite(left_L1,HIGH);
  digitalWrite(left_L2,LOW);
  analogWrite(PWM_R,100);
  analogWrite(PWM_L,100);
}
//反转
void back()
{
  digitalWrite(right_R1,LOW);
  digitalWrite(right_R2,HIGH);
  digitalWrite(left_L1,LOW);
  digitalWrite(left_L2,HIGH);
  analogWrite(PWM_R,100);
  analogWrite(PWM_L,100);
}
//停止
void Stop()
{
  digitalWrite(right_R1,LOW);
  digitalWrite(right_R2,HIGH);
  digitalWrite(left_L1,LOW);
  digitalWrite(left_L2,HIGH);
  analogWrite(PWM_R,0);
  analogWrite(PWM_L,0);
}
