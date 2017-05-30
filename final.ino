#include <Wire.h> //IIC库SDA接A4，SCL接A5
#include <math.h> 
#include </Users/zby/Desktop/final/dht11.h>
int BH1750address = 0x23;//芯片地址为16位23
byte buff[2];
int num = 0;  
//DTH11
int T[4];

int f1=0;
int f2=0;
 
dht11 DHT11;
#define DHT11PIN 2 //湿度温度口
//soil
#define Moisture A0 //定义AO 引脚 为 IO-A0  
#define DO 7        //定义DO 引脚 为 IO-7  
//delay(150); 
void setup()
{
  Wire.begin();
  pinMode(Moisture, INPUT);//土壤湿度ao口
  pinMode(DO, INPUT); //土壤湿度do口
  pinMode(12,OUTPUT);//风扇口
  pinMode(11,OUTPUT);//加热片口
  pinMode(3,OUTPUT);//电动机1口
  pinMode(4,OUTPUT);//电动机2口
  pinMode(8,OUTPUT);//led灯管
  pinMode(10,OUTPUT);//水泵口
  Serial.begin(9600);
  T[0]=30;//Light
  T[1]=23;//Temp
  T[2]=55;//kongqi,A
  T[3]=900;//S
}
void loop()
{ 
  //digitalWrite(12,HIGH);
  //delay(100);
  String comdata = "";
  String light="";
  String a1,a2,a3,a4;
  //GY-30
  // int i;
  //int f1=0;
  uint16_t val=0;
  if(f1==1)
  {
     digitalWrite(3,LOW);digitalWrite(4,LOW);digitalWrite(8,LOW);
     digitalWrite(10,LOW);digitalWrite(12,LOW);
     digitalWrite(11,HIGH); //加热片加热20s
     Serial.println("加热片加热20s");
     delay(10000);
     digitalWrite(10,HIGH); //加热片加热20s
     Serial.println("水泵工作10s");
     delay(10000);
     digitalWrite(10,LOW); //加热片加热20s
     Serial.println("结束");
     digitalWrite(12,HIGH);//风扇开始工作,和加热片一起工作20s
     Serial.println("风扇开始工作,和加热片一起工作20s");
     delay(20000);
     Serial.println("结束");
     digitalWrite(11,LOW);//关闭加热片,风扇继续工作
     Serial.println("关闭加热片,风扇继续工作,帘开始铺开,检测并输出光强");
     do
     {
        if(2==BH1750_Read(BH1750address))
          {
            val=((buff[0]<<8)|buff[1])/1.2;
            Serial.println(val);
            digitalWrite(3,HIGH); digitalWrite(4,LOW); //帘开始铺开,风扇继续工作
            delay(200);
                 if (val<100) 
                 {
                    Serial.println("检测到遮住了光强传感器,停止铺帘,风扇停止,等待5s");
                    digitalWrite(12,LOW);//如果光强低于50,停止铺帘,风扇停止
                    digitalWrite(3,LOW); digitalWrite(4,LOW); 
                    delay(5000);//等待5s
                    digitalWrite(4,HIGH); digitalWrite(3,LOW);//帘卷上
                    Serial.println("帘子开始卷上");
                    delay(4000); 
                    digitalWrite(4,LOW);//帘子停止工作
                    digitalWrite(8,HIGH);//led灯开始量10s
                    Serial.println("帘子停止工作,led开始亮起");
                    delay(10000);
                    digitalWrite(8,LOW);
                    Serial.println("演示结束");
                    break; //演示结束
                 }
          }
      }while(1);
      f1=0;
  }
  
 //////////////////////////////////////////////////////////////////
 ///////////////////////光强模块调控////////////////////////////////
 //////////////////////////////////////////////////////////////////
 BH1750_Init(BH1750address);
 //delay(1000);
 if(2==BH1750_Read(BH1750address))
  {
   val=((buff[0]<<8)|buff[1])/1.2;
   Serial.print("L");
   Serial.print(val); 
   Serial.print("L");  
   //digitalWrite(3,LOW); digitalWrite(4,LOW);
   //Serial.print("\n");  
   //Serial.println("[lx]");
   if (val>=500) 
   {
      digitalWrite(3,HIGH); digitalWrite(4,LOW); 
       
   }
   if (val<500&&val>40) 
   {
      digitalWrite(3,LOW); digitalWrite(4,LOW);   
   }
   if (val<=40) 
   {
      digitalWrite(4,HIGH); digitalWrite(3,LOW); 
      //delay(9000);
   }
   if(val<=T[0])
   {
      digitalWrite(8,HIGH);
    }
    else
    {
      digitalWrite(8,LOW);
      }
  }
 
 ///DTH11
 //Serial.println("\n");
  //////////////////////////////////////////////////////////////////
 ///////////////////////温度和湿度///////////////////////////////////
 //////////////////////////////////////////////////////////////////
  int chk = DHT11.read(DHT11PIN);
 /*
  //Serial.print("Read sensor: ");
  switch (chk)
  {
    case DHTLIB_OK: 
                //Serial.println("OK"); 
                break;
    case DHTLIB_ERROR_CHECKSUM: 
                Serial.println("Checksum error"); 
                break;
    case DHTLIB_ERROR_TIMEOUT: 
                Serial.println("Time out error"); 
                break;
    default: 
                Serial.println("Unknown error"); 
                break;
  }*/
  int aaa=DHT11.humidity;
  int bbb=DHT11.temperature;
  if (bbb<T[1]) {digitalWrite(11,HIGH);digitalWrite(12,LOW); }
  if (bbb==T[1]) {digitalWrite(11,HIGH);digitalWrite(12,HIGH); }
  if (bbb>T[1]) {digitalWrite(11,LOW);digitalWrite(12,HIGH); }
  if(aaa<T[2]) f2=1; 
  //Serial.print("dd");
  //Serial.print("Humidity (%): ");
  Serial.print("A");
  Serial.print(aaa);
  Serial.print("A");
  //Serial.print("Temperature (oC): ");
  Serial.print("T");
  Serial.print(bbb);
  Serial.print("T");
  //Serial.print("Temperature (oF): ");
  //Serial.println(Fahrenheit(DHT11.temperature), 2);
 
  //Serial.print("Temperature (K): ");
  //Serial.println(Kelvin(DHT11.temperature), 2);
 
  //Serial.print("Dew Point (oC): ");
  //Serial.println(dewPoint(DHT11.temperature, DHT11.humidity));
 
  //Serial.print("Dew PointFast (oC): ");
  //Serial.println(dewPointFast(DHT11.temperature, DHT11.humidity));
  // delay(2000);
  //////////////////////////////////////////////////////////////////
 ///////////////////////土壤湿度/////////////////////////////////////
 //////////////////////////////////////////////////////////////////
  //Serial.print("Moisture="); 
  //digitalWrite(10,HIGH); 
  Serial.print("S");
  Serial.print(analogRead(Moisture));//读取AO的数值 
  Serial.print("S");
  if(analogRead(Moisture)>T[3])digitalWrite(10,HIGH);
  else digitalWrite(10,LOW);
  if(f2) 
  {Serial.print("dd"); f2=0;}
  Serial.print("\n");
  //Serial.print("dd"); 
  //if (analogRead(Moisture)<950) digitalWrite(12,LOW);
  //else digitalWrite(12,HIGH); 
  //Serial.print("|DO=");  
  //Serial.println(digitalRead(DO));//读取DO的数值  
  delay(1000);  
  /*while(Serial.available())  
   {  
     char c=Serial.read();  
      if(c=='0')  
        {  
          num++;  
          Serial.print("Hello:num=");  
          Serial.println(num);  
        }  
   }  */
   /*
   while(Serial.available())  
   {  
     char c=Serial.read();  
      if(c=='0')  
        {  
          digitalWrite(12,LOW);
          digitalWrite(4,LOW);
          digitalWrite(8,LOW);
          num++;  
          Serial.print("Hello:num=");  
          Serial.println(num);  
        } 
      if 
   }*/
    //////////////////////////////////////////////////////////////////
 ///////////////////////接受app发射////////////////////////////////
 //////////////////////////////////////////////////////////////////
   int commaPosition;
   String th;
   int j=0;
   while (Serial.available() > 0)  
    {
        comdata += char(Serial.read());
        delay(2);
    }
   if (comdata.length() > 0)
    {
        /*
        Serial.println(comdata);
        //comdata = "";
        a1 = comdata[0];
        a2 = comdata[1];
        light = a1 + a2;
        comdata = "";
        Serial.println(light);
        int n;
        n = light.toInt();
        Serial.println(light);*/
        Serial.println(comdata);
        do
        {
          commaPosition = comdata.indexOf(',');
           if(commaPosition != -1)
             {
               th=comdata.substring(0,commaPosition);
               T[j]=th.toInt();
               Serial.println(T[j]);
               //Serial.println(comdata.substring(0,commaPosition));
               comdata=comdata.substring(commaPosition+1,comdata.length()); 
               //  Serial.println(message);
               j++;
             }
           else
             {
               if(comdata.length()>0)
                 {
                   Serial.println(comdata);
                 }
             }
         }while(commaPosition>=0);
         if(T[0]==0)
         {
            f1=1;
          }
    }
}

 //////////////////////////////////////////////////////////////////
 ///////////////////////具体函数部分////////////////////////////////
 //////////////////////////////////////////////////////////////////
int BH1750_Read(int address) //
{
  int i=0;
  Wire.beginTransmission(address);
  Wire.requestFrom(address, 2);
  while(Wire.available()) //
  {
    buff[i] = Wire.read();  // read one byte
    i++;
  }
  Wire.endTransmission();  
  return i;
}
void BH1750_Init(int address) 
{
  Wire.beginTransmission(address);
  Wire.write(0x10);//1lx reolution 120ms
  Wire.endTransmission();
}
double Fahrenheit(double celsius) 
{
        return 1.8 * celsius + 32;
}    //摄氏温度度转化为华氏温度
 
double Kelvin(double celsius)
{
        return celsius + 273.15;
}     //摄氏温度转化为开氏温度
 
// 露点（点在此温度时，空气饱和并产生露珠）
// 参考: [url=http://wahiduddin.net/calc/density_algorithms.htm]http://wahiduddin.net/calc/density_algorithms.htm[/url] 
double dewPoint(double celsius, double humidity)
{
        double A0= 373.15/(273.15 + celsius);
        double SUM = -7.90298 * (A0-1);
        SUM += 5.02808 * log10(A0);
        SUM += -1.3816e-7 * (pow(10, (11.344*(1-1/A0)))-1) ;
        SUM += 8.1328e-3 * (pow(10,(-3.49149*(A0-1)))-1) ;
        SUM += log10(1013.246);
        double VP = pow(10, SUM-3) * humidity;
        double T = log(VP/0.61078);   // temp var
        return (241.88 * T) / (17.558-T);
}
 
// 快速计算露点，速度是5倍dewPoint()
// 参考: [url=http://en.wikipedia.org/wiki/Dew_point]http://en.wikipedia.org/wiki/Dew_point[/url]
double dewPointFast(double celsius, double humidity)
{
        double a = 17.271;
        double b = 237.7;
        double temp = (a * celsius) / (b + celsius) + log(humidity/100);
        double Td = (b * temp) / (a - temp);
        return Td;
}
