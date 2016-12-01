# LASS-7688Duo

這是一個以 Linkit 7688 Duo 所製作的空氣品質監測系統


## How to start
### MCU (arduino端)
負責將感測器資料透過Serial1送至MPU
1. include DHT Arduino Library
2. Upload LASS-7688Duo.ino

### MPU (Linux端)
接收MCU端的資料包裝傳至MQTT
```
opkg update
opkg install git mosquitto mosquitto-client libmosquitto
git clone https://github.com/jd615645/LASS-7688Duo.git
cd ./LASS-7688Duo/mpu
npm install
```

## 所需材料
* Linkit 7688 Duo
* DHT11/DHT22
* G3/PMS5003/PMS5003S
* 20*4 LCD(選用)

## 參考資料
* [lanma/LASS-7688Duo](https://github.com/lanma/LASS-7688Duo)
* [Custom Character Generator](https://omerk.github.io/lcdchargen/)
* [LCD_test](https://create.arduino.cc/editor/YuChien/86a0797e-1eb9-477b-9f03-57d1310150ae/preview)

## 其他
G3/PMS5003/PMS5003S Pin腳
![PM2.5 感測器](http://imgur.com/OGU3H8w.png)
