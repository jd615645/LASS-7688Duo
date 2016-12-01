#include <SoftwareSerial.h>
#include <dht.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set the LCD I2C address, 不同版本I2C address會不同要注意
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

//設定pin腳, Dht用Pin15, G3用Pin9,10
#define dht_dpin 15 
SoftwareSerial seG3(9,10);

dht DHT;

// init air quality num
long pmcf10=0;
long pmcf25=0;
long pmcf100=0;
long pmat10=0;
long pmat25=0;
long pmat100=0;

// Create a set of new characters
byte thermometer[8] = {
  B00100,
  B01010,
  B01010,
  B01010,
  B01010,
  B10001,
  B10001,
  B01110
};
byte temperatureSign[8] = {
  B01100,
  B10010,
  B10010,
  B01100,
  B00000,
  B00000,
  B00000,
  B00000
};
byte humiditySign[8] = {
  B00000,
  B00100,
  B01010,
  B10001,
  B10001,
  B11111,
  B01110,
  B00000
};
byte mirco[8] = {
  B00000,
  B10001,
  B10001,
  B10001,
  B11110,
  B10000,
  B10000,
  B10000
};
byte upperThree[8] = {
  B01100,
  B00010,
  B01100,
  B00010,
  B01100,
  B00000,
  B00000,
  B00000
};

char buf[50];

void setup() {
  seG3.begin(9600);//G3
  Serial.begin(9600);//Debug訊息
  Serial1.begin(57600);//給MPU訊息
  Serial.println("Start LASS");

  Wire.begin();
  // Initialize LCD 4x20
  lcd.begin(20, 4);
  
  // createChar
  lcd.createChar (0, thermometer);
  lcd.createChar (1, temperatureSign);
  lcd.createChar (2, humiditySign);
  lcd.createChar (3, mirco);
  lcd.createChar (4, upperThree);
  
  //clear the screen
  lcd.clear(); 
  // turn on back light
  lcd.backlight(); 

  delay(300);
}

void loop() {
  int count = 0;
  unsigned char c;
  unsigned char high;
  //debug count
  Serial.print("---\n");

  //get G3 data
  while (seG3.available()) {
    c = seG3.read();
    if((count==0 && c!=0x42) || (count==1 && c!=0x4d)){
      Serial.println("check failed");
      break;
    }
    if(count > 15){
      Serial.println("complete");
      break;
    }
    else if(count == 4 || count == 6 || count == 8 || count == 10 || count == 12 || count == 14) {
      high = c;
    }
    else if(count == 5){
      pmcf10 = 256*high + c;
      Serial.print("CF=1, PM1.0=");
      Serial.print(pmcf10);
      Serial.println(" ug/m3");
    }
    else if(count == 7){
      pmcf25 = 256*high + c;
      Serial.print("CF=1, PM2.5=");
      Serial.print(pmcf25);
      Serial.println(" ug/m3");
    }
    else if(count == 9){
      pmcf100 = 256*high + c;
      Serial.print("CF=1, PM10=");
      Serial.print(pmcf100);
      Serial.println(" ug/m3");
    }
    else if(count == 11){
      pmat10 = 256*high + c;
      Serial.print("atmosphere, PM1.0=");
      Serial.print(pmat10);
      Serial.println(" ug/m3");
    }
    else if(count == 13){
      pmat25 = 256*high + c;
      Serial.print("atmosphere, PM2.5=");
      Serial.print(pmat25);
      Serial.println(" ug/m3");
    }
    else if(count == 15){
      pmat100 = 256*high + c;
      Serial.print("atmosphere, PM10=");
      Serial.print(pmat100);
      Serial.println(" ug/m3");
    }
    count++;
  }
  while(seG3.available()) seG3.read();

  // 取得溫濕度
  //去library裡面找DHT.read11
  DHT.read11(dht_dpin);
  Serial.print("Humidity = ");   
  Serial.print(DHT.humidity);   
  Serial.print("% ");   
  Serial.print("temperature = ");   
  Serial.print(DHT.temperature);   
  Serial.println("C ");   

  //將溫濕度與PM2.5資訊傳給MPU
  Serial1.print("|s_t0=");//Temperature=
  Serial1.print(DHT.temperature);  
  Serial1.print("|s_h0=");//Humi=
  Serial1.print(DHT.humidity);  
  Serial1.print("|s_d0=");//PM2.5=
  Serial1.print(pmat25);
  Serial1.print("|s_d1=");//PM10=
  Serial1.println(pmat100);

  // print LCD
  printData(DHT.humidity, DHT.humidity, pmat10, pmat25, pmat100);

  //間隔5秒
  delay(5000);
}

void printData(long temperature, long humidity, long pm1, long pm25, long pm10) {
  //clear the screen
  lcd.clear(); 
  // turn on back light
  lcd.backlight(); 

  // print temperature
  lcd.setCursor(1, 0);
  lcd.print(char(0));
  lcd.print(" ");
  lcd.print(temperature);
  lcd.print(char(1));
  lcd.print("C");
  
  // print humidity
  lcd.setCursor(11, 0);
  lcd.print(char(2));
  lcd.print(" ");
  lcd.print(humidity);
  lcd.print("%");

  // print pm1.0
  lcd.setCursor(1, 1);
  lcd.print("pm1.0 ");
  lcd.print(pm1);
  lcd.setCursor(11, 1);
  lcd.print(char(3));
  lcd.print("g/m");
  lcd.print(char(4));

  // print pm2.5
  lcd.setCursor(1, 2);
  lcd.print("pm2.5 ");
  lcd.print(pm25);
  lcd.setCursor(11, 2);
  lcd.print(char(3));
  lcd.print("g/m");
  lcd.print(char(4));

  // print pm10
  lcd.setCursor(1, 3);
  lcd.print("pm10  ");
  lcd.print(pm10);
  lcd.setCursor(11, 3);
  lcd.print(char(3));
  lcd.print("g/m");
  lcd.print(char(4));
}
