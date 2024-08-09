const int buz = 11;    //定义蜂鸣器的引脚
const int btn = 13;    //定义按钮的引脚
int button;    //按钮的变量

void setup() 
{
  pinMode(btn,INPUT);       //设置为输入状态
  pinMode(buz,OUTPUT);       //设置为输出状态
}

void loop() 
{
  button = digitalRead(btn);       //将读取到的按钮值赋给变量button
  if(button == 0)    //如果按钮按下去了
  {
    delay(10);    //延时消抖
    if(button == 0)     //再次判断，如果按钮是按下了
    {
      buzzer();  //执行蜂鸣器的子函数
    }
  }
  else        //按钮没按下
  {
    digitalWrite(buz,LOW);    //蜂鸣器不响
  }
}

//蜂鸣器发出“嘀嘀”声响
void buzzer()      
{
    for(int i=0;i<50;i++)
    {
    digitalWrite(buz,HIGH);
    delay(1);
    digitalWrite(buz,LOW);
    delay(1);
    }
    delay(50);
    for(int i=0;i<50;i++)
    {
    digitalWrite(buz,HIGH);
    delay(1);
    digitalWrite(buz,LOW);
    delay(1);
    }
}
