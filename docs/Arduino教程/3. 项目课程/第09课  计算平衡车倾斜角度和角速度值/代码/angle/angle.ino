#include <MPU6050.h>      //MPU6050的库文件
#include <Wire.h>        //IIC通讯库文件

MPU6050 mpu6050;     //实例化一个MPU6050对象，对象名称为mpu6050
int16_t ax, ay, az, gx, gy, gz;     //定义三轴加速度，三轴陀螺仪的变量

float Angle;   //角度变量
int16_t Gyro_x;   //角速度变量

void setup() 
{
  // 加入I2C总线
  Wire.begin();                            //加入 I2C 总线序列
  Serial.begin(9600);                       //开启串口，设置波特率为9600
  delay(1500);
  mpu6050.initialize();                       //初始化MPU6050
  delay(2);
}

void loop() 
{
  mpu6050.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);     //IIC获取MPU6050六轴数据 ax ay az gx gy gz
  Angle = -atan2(ay , az) * (180/ PI);           //弧度转角度计算公式,负号为方向处理
  Gyro_x = -gx / 131;              //陀螺仪计算的X轴角速度，负号为方向处理
  Serial.print("Angle = ");
  Serial.print(Angle);
  Serial.print("   Gyro_x = ");
  Serial.println(Gyro_x);
}
