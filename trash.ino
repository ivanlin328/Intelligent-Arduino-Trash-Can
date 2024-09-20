#include "pitches.h"
#include <Servo.h>   //servo library
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "DHT.h"
#define DHTPIN 9
#define DHTTYPE DHT11
//#define DHTTYPE DHT22   // DHT 22 如果用的是DHT22，就用這行
//#define DHTTYPE DHT21   // DHT 21
DHT dht(DHTPIN, DHTTYPE);


LiquidCrystal_I2C lcd(0x27, 16, 2); //設定LCD位置0x27,設定LCD大小為16*2
Servo servo;
int trigPin = 5;
int echoPin = 6;
int trigPin2 = 11;
int echoPin2 = 12;
int servoPin = 7;
//LED
#define ledBluePin   A1
#define ledRedPin   A3
#define ledYellowPin  A2
long duration, dist, average;
long aver[3];   //array for average
long duration2, dist2, average2;
long aver2[3];   //array for average
int CanonTone[] = {
  NOTE_E3, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5,
  NOTE_E5, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_G4, NOTE_F4, NOTE_G4, NOTE_F4, NOTE_F4, NOTE_G4,
  NOTE_F4, NOTE_A4, NOTE_G4, NOTE_F4, NOTE_E4, NOTE_D4, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4,
  NOTE_F4, NOTE_A4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5,
  NOTE_E5, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_D5, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_B4,
  NOTE_C5, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_E4, NOTE_D4, NOTE_E4, NOTE_C5, NOTE_B4, NOTE_C5,
  NOTE_A4, NOTE_C5, NOTE_B4, NOTE_A4, NOTE_G4, NOTE_F4, NOTE_G4, NOTE_F4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5,
  NOTE_A4, NOTE_C5, NOTE_B4, NOTE_C5, NOTE_B4, NOTE_A4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_C5, NOTE_B4, NOTE_C5, NOTE_A4, NOTE_B4,
  NOTE_C5, 0
};  // 注意, 最後一個 0 是用來判斷是否曲子結束用, 非音調
// 再定義卡農的拍子長度
int noteDurations[] = {
  8, 16, 16, 8, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
  8, 16, 16, 8, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
  8, 16, 16, 8, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
  8, 16, 16, 8, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
  8, 16, 16, 8, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
  8, 16, 16, 8, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
  8, 16, 16, 8, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
  8, 16, 16, 8, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
  1
};
int toneSpeed = 1800;   // 設定音速, 我用1800ms, 即1.8 秒當全音符
void setup() {
  Serial.begin(9600);
  servo.attach(servoPin);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode (trigPin2, OUTPUT);
  pinMode (echoPin2, INPUT);
  pinMode (ledBluePin, OUTPUT);
  pinMode (ledRedPin, OUTPUT);
  pinMode (ledYellowPin, OUTPUT);
  dht.begin();  //初始化DHT
  servo.write(0);         //close cap on power on
  delay(100);
  servo.detach();
  lcd.init(); 
  lcd.backlight();
  lcd.setCursor(5, 0);
  lcd. print("garbages");
  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.setCursor(14, 1);
  lcd.print("%");

}

void measure() {
  digitalWrite(10, HIGH);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(15);
  digitalWrite(trigPin, LOW);
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  dist = (duration / 2) / 29.1;  //obtain distance
}
void measurevolme() {
  digitalWrite(10, HIGH);
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(15);
  digitalWrite(trigPin2, LOW);
  pinMode(echoPin2, INPUT);
  duration2 = pulseIn(echoPin2, HIGH);
  dist2 = (duration2 / 2) / 29.1;  //obtain distance
}
void loop() {
  for (int i = 0; i <= 2; i++) { //average distance
    measure();
    aver[i] = dist;
    delay(10);              //delay between measurements
  }
  dist = (aver[0] + aver[1] + aver[2]) / 3;

  if ( dist < 50 ) {
    servo.attach(servoPin);
    delay(1);
    servo.write(0);
    // tone(11, 524, 950); //buzzer or speaker frequency
    delay(3500);
    servo.write(180);
    delay(1000);
    servo.detach();
  }
  //Serial.println(dist);
  for (int j = 0; j <= 2; j++) { //average distance
    measurevolme();
    aver2[j] = dist2;
    delay(10);              //delay between measurements
  }
  dist2 = (aver2[0] + aver2[1] + aver2[2]) / 3;
  if ( dist2 < 5 ) {
    //Serial.println("LED Red.");
    digitalWrite(ledRedPin, HIGH);
    digitalWrite(ledBluePin, LOW);
    digitalWrite(ledYellowPin, LOW);
    lcd.setCursor(0, 0); // (colum, row)從第一排的第三個位置開始顯示
    lcd. print("many");
    int thisNote = 0;  // 在此使用While 而不用 for
    while (CanonTone[thisNote] > 0) { // 當CanonTone 內的值 > 0 時 則
      // toneSpeed / 4 當四分音符
      // / 8 當 八分音符
      // .
      int noteDuration = toneSpeed / noteDurations[thisNote];
      tone(8, CanonTone[thisNote], noteDuration);
      // delay 1.3 偣的音長
      //
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
      // 準備下一個音調
      thisNote++;


    }
    noTone(8);  // 播完後靜音
    Serial.println("take out the trash.");
  }
  else  {

    // Serial.println("LED Yellow.");
    digitalWrite(ledRedPin, LOW);
    digitalWrite(ledBluePin, LOW);
    digitalWrite(ledYellowPin, HIGH);
    lcd.setCursor(0, 0); // (colum, row)從第一排的第三個位置開始顯示
    lcd. print("less");


  }
  //Serial.println(dist2);
  delay(1000);
  float h = dht.readHumidity();   //取得濕度
  lcd.setCursor(9, 1);
  lcd.print(h);
  //if(h>60){
  //Serial.println("open dehumidifier");
  //}


}
