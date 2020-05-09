#include <Arduino.h>

void AE_HX711_Init(void);
void AE_HX711_Reset(void);
long AE_HX711_Read1(void);
long AE_HX711_Read2(void);
long AE_HX711_Averaging1(long adc,char num);
long AE_HX711_Averaging2(long adc,char num);
float AE_HX711_getGram1(char num);
float AE_HX711_getGram2(char num);

//---------------------------------------------------//
// ピンの設定
//---------------------------------------------------//
#define pin_dout1  8
#define pin_slk1   9
#define pin_dout2  10
#define pin_slk2   11

//---------------------------------------------------//
// ロードセル　Ｓ字型　ＳＣ３０１Ａ　１００ｋＧ [P-12036]
//---------------------------------------------------//
//#define OUT_VOL   0.002f      //定格出力 [V]
//#define LOAD      100000.0f   //定格容量 [g]

//---------------------------------------------------//
// ロードセル　シングルポイント（ ビーム型）　ＳＣ６０１　１２０ｋＧ [P-12035]
//---------------------------------------------------//
//#define OUT_VOL   0.001f      //定格出力 [V]
//#define LOAD      120000.0f   //定格容量 [g]

//---------------------------------------------------//
// ロードセル　シングルポイント（ ビーム型）　ＳＣ１３３　２０ｋＧ [P-12034]
//---------------------------------------------------//
//#define OUT_VOL   0.002f      //定格出力 [V]
//#define LOAD      20000.0f    //定格容量 [g]

//---------------------------------------------------//
// ロードセル　シングルポイント（ ビーム型）　ＳＣ１３３　２ｋＧ [P-13041]
//---------------------------------------------------//
#define OUT_VOL1   0.001f      //定格出力 [V]
#define LOAD1      2000.0f    //定格容量 [g]

//---------------------------------------------------//
// ロードセル　シングルポイント（ビーム型）　ＳＣ６１６Ｃ　５００ｇ[P-12532]
//---------------------------------------------------//
#define OUT_VOL2   0.0007f   //定格出力 [V]
#define LOAD2      500.0f    //定格容量 [g]




float offset1;
float offset2;

void setup() {
  Serial.begin(9600);
  //Serial.println("AE_HX711 test");
  AE_HX711_Init();
  AE_HX711_Reset();
  offset1 = AE_HX711_getGram1(30);
  offset2 = AE_HX711_getGram2(30);
}

void loop() 
{ 
  float data1;
  float data2;
  char S1[20];
  char s[20];
  char T1[20];
  char t[20];
  data1 = AE_HX711_getGram1(5);
  data2 = AE_HX711_getGram2(5);
  //sprintf(S1,"%s [g] (0x%4x)",dtostrf((data-offset), 5, 3, s),AE_HX711_Read());
  sprintf(S1,"%s",dtostrf((data1-offset1), 5, 3, s),AE_HX711_Read1());
  sprintf(T1,"%s",dtostrf((data2-offset2), 5, 3, t),AE_HX711_Read2());
  //Serial.print("weight");
  Serial.print(S1);
  Serial.print(",");
  Serial.println(T1);
  //Serial.write(S1);
}



void AE_HX711_Init(void)
{
  pinMode(pin_slk1, OUTPUT);
  pinMode(pin_dout1, INPUT);
  pinMode(pin_slk2, OUTPUT);
  pinMode(pin_dout2, INPUT);
}

void AE_HX711_Reset(void)
{
  digitalWrite(pin_slk1,1);
  digitalWrite(pin_slk2,1);
  delayMicroseconds(100);
  digitalWrite(pin_slk1,0);
  digitalWrite(pin_slk2,0);
  delayMicroseconds(100); 
}

long AE_HX711_Read1(void)
{
  long data1=0;
  while(digitalRead(pin_dout1)!=0);
  delayMicroseconds(10);
  for(int i=0;i<24;i++)
  {
    digitalWrite(pin_slk1,1);
    delayMicroseconds(5);
    digitalWrite(pin_slk1,0);
    delayMicroseconds(5);
    data1 = (data1<<1)|(digitalRead(pin_dout1));
  }
  //Serial.println(data,HEX);   
  digitalWrite(pin_slk1,1);
  delayMicroseconds(10);
  digitalWrite(pin_slk1,0);
  delayMicroseconds(10);
  return data1^0x800000;
}

long AE_HX711_Read2(void)
{
  long data2=0;
  while(digitalRead(pin_dout2)!=0);
  delayMicroseconds(10);
  for(int i=0;i<24;i++)
  {
    digitalWrite(pin_slk2,1);
    delayMicroseconds(5);
    digitalWrite(pin_slk2,0);
    delayMicroseconds(5);
    data2 = (data2<<1)|(digitalRead(pin_dout2));
  }
  //Serial.println(data,HEX);   
  digitalWrite(pin_slk2,1);
  delayMicroseconds(10);
  digitalWrite(pin_slk2,0);
  delayMicroseconds(10);
   return data2^0x800000;
}


long AE_HX711_Averaging1(long adc,char num)
{
  long sum = 0;
  for (int i = 0; i < num; i++) sum += AE_HX711_Read1();
  return sum / num;
}

long AE_HX711_Averaging2(long adc,char num)
{
  long sum = 0;
  for (int i = 0; i < num; i++) sum += AE_HX711_Read2();
  return sum / num;
}

float AE_HX711_getGram1(char num)
{
  #define HX711_R1  20000.0f
  #define HX711_R2  8200.0f
  #define HX711_VBG 1.25f
  #define HX711_AVDD      4.2987f//(HX711_VBG*((HX711_R1+HX711_R2)/HX711_R2))
  #define HX711_ADC1bit   HX711_AVDD/16777216 //16777216=(2^24)
  #define HX711_PGA 128
  #define HX711_SCALE1     (OUT_VOL1 * HX711_AVDD / LOAD1 *HX711_PGA)
  
  float data1;

  data1 = AE_HX711_Averaging1(AE_HX711_Read1(),num)*HX711_ADC1bit; 
  data1 =  data1 / HX711_SCALE1;

  return data1;
}

float AE_HX711_getGram2(char num)
{
  #define HX711_SCALE2     (OUT_VOL2 * HX711_AVDD / LOAD2 *HX711_PGA)
  
  float data2;

  data2 = AE_HX711_Averaging2(AE_HX711_Read2(),num)*HX711_ADC1bit; 
  data2 =  data2 / HX711_SCALE2;

  return data2;
}
