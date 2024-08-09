#include <MsTimer2.h>        //内部定时器2
#include <PinChangeInt.h>    //这个库文件可以使arduino UNO板上的所有引脚作为外部中断
#include <MPU6050.h>      //MPU6050的库文件
#include <Wire.h>        //IIC通讯库文件

MPU6050 mpu6050;     //实例化一个MPU6050对象，对象名称为mpu6050
int16_t ax, ay, az, gx, gy, gz;     //定义三轴加速度，三轴陀螺仪的变量

//TB6612引脚定义
const int right_R1=8;    
const int right_R2=12;
const int PWM_R=10;
const int left_L1=7;
const int left_L2=6;
const int PWM_L=9;

///////////////////////角度参数//////////////////////////////
float Angle;
float angle_X; //由加速度计算关于X轴的倾斜角度变量
float angle_Y; //由加速度计算关于Y轴的倾斜角度变量
float angle0 = 1; //实际测量出的角度（理想时是0度）
float Gyro_x,Gyro_y,Gyro_z;  //用于陀螺仪算出的各轴角速度
///////////////////////角度参数//////////////////////////////

///////////////////////Kalman_Filter////////////////////////////
float Q_angle = 0.001;  //陀螺仪噪声的协方差
float Q_gyro = 0.003;    //陀螺仪漂移噪声的协方差
float R_angle = 0.5;    //加速度计的协方差
char C_0 = 1;
float dt = 0.005; //dt的取值为滤波器采样时间
float K1 = 0.05; // 含有卡尔曼增益的函数，用于计算最优估计值的偏差
float K_0,K_1,t_0,t_1;
float angle_err;
float q_bias;    //陀螺仪漂移

float accelz = 0;
float angle;
float angleY_one;
float angle_speed;

float Pdot[4] = { 0, 0, 0, 0};
float P[2][2] = {{ 1, 0 }, { 0, 1 }};
float  PCt_0, PCt_1, E;
//////////////////////Kalman_Filter/////////////////////////

//////////////////////PD参数///////////////////////////////
double kp = 34, ki = 0, kd = 0.62;                   //角度环参数
double setp0 = 0; //角度平衡点
int PD_pwm;  //角度输出
float pwm1=0,pwm2=0;

//void anglePWM();

void setup() 
{
  //设置控制电机的引脚为输出状态
  pinMode(right_R1,OUTPUT);       
  pinMode(right_R2,OUTPUT);
  pinMode(left_L1,OUTPUT);
  pinMode(left_L2,OUTPUT);
  pinMode(PWM_R,OUTPUT);
  pinMode(PWM_L,OUTPUT);

  //赋初始状态值
  digitalWrite(right_R1,1);
  digitalWrite(right_R2,0);
  digitalWrite(left_L1,0);
  digitalWrite(left_L2,1);
  analogWrite(PWM_R,0);
  analogWrite(PWM_L,0);

  // 加入I2C总线
  Wire.begin();                            //加入 I2C 总线序列
  Serial.begin(9600);                       //开启串口，设置波特率为9600
  delay(1500);
  mpu6050.initialize();                       //初始化MPU6050
  delay(2);

  //5ms定时中断设置是使用timer2 (注意：使用timer2会对pin3 pin11的PWM输出有影响.)
  MsTimer2::set(5, DSzhongduan);    //5ms执行一次DSzhongduan这个函数
  MsTimer2::start();    //开启中断
}

void loop() 
{
  Serial.print("angle = ");
  Serial.println(angle);
  Serial.print("Angle = ");
  Serial.println(Angle);

  /*Serial.print("Gyro_x = ");
  Serial.println(Gyro_x);
  Serial.print("K_Gyro_x = ");
  Serial.println(angle_speed);*/
  
  //Serial.println(PD_pwm);
  //Serial.println(pwm1);
  //Serial.println(pwm2);
}

/////////////////////////////////中断////////////////////////////
void DSzhongduan()
{
  sei();  //允许全局中断
  mpu6050.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);     //IIC获取MPU6050六轴数据 ax ay az gx gy gz
  angle_calculate(ax, ay, az, gx, gy, gz, dt, Q_angle, Q_gyro, R_angle, C_0, K1);      //获取angle 角度和卡曼滤波
  PD();         //角度环 PD控制
  anglePWM();
}
///////////////////////////////////////////////////////////

/////////////////////////////倾角计算///////////////////////
void angle_calculate(int16_t ax,int16_t ay,int16_t az,int16_t gx,int16_t gy,int16_t gz,float dt,float Q_angle,float Q_gyro,float R_angle,float C_0,float K1)
{
  Angle = -atan2(ay , az) * (180/ PI);           //弧度转角度计算公式,负号为方向处理
  Gyro_x = -gx / 131;              //陀螺仪计算的X轴角速度，负号为方向处理
  Kalman_Filter(Angle, Gyro_x);            //卡曼滤波
  //旋转角度Z轴参数
  Gyro_z = -gz / 131;                      //Z轴角速度
  //accelz = az / 16.4;

  float angleAx = -atan2(ax, az) * (180 / PI); //计算与x轴夹角
  Gyro_y = -gy / 131.00; //Y轴角速度
  Yiorderfilter(angleAx, Gyro_y); //一阶滤波
}
////////////////////////////////////////////////////////////////

///////////////////////////////KalmanFilter/////////////////////
void Kalman_Filter(double angle_m, double gyro_m)
{
  angle += (gyro_m - q_bias) * dt;          //先验估计
  angle_err = angle_m - angle;
  
  Pdot[0] = Q_angle - P[0][1] - P[1][0];    //先验估计误差协方差的微分
  Pdot[1] = - P[1][1];
  Pdot[2] = - P[1][1];
  Pdot[3] = Q_gyro;
  
  P[0][0] += Pdot[0] * dt;    //先验估计误差协方差微分的积分
  P[0][1] += Pdot[1] * dt;
  P[1][0] += Pdot[2] * dt;
  P[1][1] += Pdot[3] * dt;
  
  //矩阵乘法的中间变量
  PCt_0 = C_0 * P[0][0];
  PCt_1 = C_0 * P[1][0];
  //分母
  E = R_angle + C_0 * PCt_0;
  //增益值
  K_0 = PCt_0 / E;
  K_1 = PCt_1 / E;
  
  t_0 = PCt_0;  //矩阵乘法的中间变量
  t_1 = C_0 * P[0][1];
  
  P[0][0] -= K_0 * t_0;    //后验估计误差协方差
  P[0][1] -= K_0 * t_1;
  P[1][0] -= K_1 * t_0;
  P[1][1] -= K_1 * t_1;
  
  q_bias += K_1 * angle_err;    //后验估计
  angle_speed = gyro_m - q_bias;   //输出值的微分，得出最优角速度
  angle += K_0 * angle_err; ////后验估计，得出最优角度
}

/////////////////////一介滤波/////////////////
void Yiorderfilter(float angle_m, float gyro_m)
{
  angleY_one = K1 * angle_m + (1 - K1) * (angleY_one + gyro_m * dt);
}

//////////////////角度PD////////////////////
void PD()
{
  PD_pwm = kp * (angle + angle0) + kd * angle_speed; //PD 角度环控制
}


////////////////////////////PWM终值/////////////////////////////
void anglePWM()
{
  pwm2=-PD_pwm;            //赋给电机PWM的最终值
  pwm1=-PD_pwm;
  
  if(pwm1>255)             //限定PWM值不能超过255
  {
    pwm1=255;
  }
  if(pwm1<-255) 
  {
    pwm1=-255;
  }
  if(pwm2>255)
  {
    pwm2=255;
  }
  if(pwm2<-255)
  {
    pwm2=-255;
  }

  if(angle>80 || angle<-80)      //自平衡小车倾斜角度大于45度，电机就会停转
  {
    pwm1=pwm2=0;
  }

  if(pwm2>=0)         //根据PWM的正负来确定电机的转向和转速
  {
    digitalWrite(left_L1,LOW);
    digitalWrite(left_L2,HIGH);
    analogWrite(PWM_L,pwm2);
  }
  else
  {
    digitalWrite(left_L1,HIGH);
    digitalWrite(left_L2,LOW);
    analogWrite(PWM_L,-pwm2);
  }

  if(pwm1>=0)
  {
    digitalWrite(right_R1,LOW);
    digitalWrite(right_R2,HIGH);
    analogWrite(PWM_R,pwm1);
  }
  else
  {
    digitalWrite(right_R1,HIGH);
    digitalWrite(right_R2,LOW);
    analogWrite(PWM_R,-pwm1);
  }
}
