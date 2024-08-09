#include <MsTimer2.h>

//TB6612引脚定义
const int right_R1 = 8;
const int right_R2 = 12;
const int PWM_R = 10;
const int left_L1 = 7;
const int left_L2 = 6;
const int PWM_L = 9;

const int PinA_left = 5;        //定义检测左电机脉冲的引脚为D5
const int PinA_right = 4;       //定义检测右电机脉冲的引脚为D4

int times = 0, newtime = 0, d_time = 100; //时间，最新的时间，时间间隔
int valA = 0, valB = 0, flagA = 0, flagB = 0; //变量valA和valB用于计算脉冲数

void setup()
{
  Serial.begin(9600);

  pinMode(right_R1, OUTPUT);    //TB6612的引脚都设置为输出
  pinMode(right_R2, OUTPUT);
  pinMode(PWM_R, OUTPUT);
  pinMode(left_L1, OUTPUT);
  pinMode(left_L2, OUTPUT);
  pinMode(PWM_L, OUTPUT);

  pinMode(PinA_left, INPUT);     //设置检测脉冲的引脚为输入状态
  pinMode(PinA_right, INPUT);

  MsTimer2::set(100, inter); // 100ms 触发一次中断
  MsTimer2::start();    //开启中断
}

void loop()
{
  //两电机都正转
  digitalWrite(right_R1, HIGH);
  digitalWrite(right_R2, LOW);
  digitalWrite(left_L1, HIGH);
  digitalWrite(left_L2, LOW);
  analogWrite(PWM_R, 100);   //写入PWM值0~255（速度）
  analogWrite(PWM_L, 200);

  if (digitalRead(PinA_left) == HIGH && flagA == 0) //计算脉冲值
  {
    valA++;
    flagA = 1;
  }
  if (digitalRead(PinA_left) == LOW && flagA == 1)
  {
    valA++;
    flagA = 0;
  }

  if (digitalRead(PinA_right) == HIGH && flagB == 0)
  {
    valB++;
    flagB = 1;
  }
  if (digitalRead(PinA_right) == LOW && flagB == 1)
  {
    valB++;
    flagB = 0;
  }

}

//中断函数
void inter()
{
  sei();    //允许全局中断
  Serial.print("valA = ");     //在串口监视器上打印出脉冲值
  Serial.println(valA);
  Serial.print("valB = ");
  Serial.println(valB);
  valA = valB = 0;
}
