int trigPin = A0;    // 定义超声波发射脚位
int echoPin = A1;    // 定义超声波接收脚位
long duration, cm, inches;
void setup() {
  Serial.begin (9600);  //开始串口打印
  pinMode(trigPin, OUTPUT); //定义超声波发射为输出
  pinMode(echoPin, INPUT); //定义超声波接收为输入
}
void loop() {
 //传感器由10毫秒或更长时间的高电平脉冲触发
 //预先给短的LOW脉冲以确保稳定高电平脉冲
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
   //读取来自传感器的信号：一个高脉冲
  //持续时间是指从发送到发送的时间（以微秒为单位）
  // ping到接收对象的回声。
  duration = pulseIn(echoPin, HIGH);
//将时间转换为距离
  cm = (duration/2) / 29.1;     // 除以29.1或乘以0.0343
  inches = (duration/2) / 74;   // 除以74或乘以0.0135
  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  delay(200);
}
