#include <Wire.h> // KÜTÜPHANEYİ İMPORT EDİYORUZ

const int ADDRESS = 0x40; // 64 DEMEK DECİMAL //DEĞİŞTİRİLECEK / ADDRESS HEXADECIMAL CODE

// SADECE BAŞLANGIÇTA ÇALIŞTIRILACAK KODLARI GİRİYORUZ
void setup() {
  int ret;

  Serial.begin(9600); // initialize serial communication
  Wire.begin();       // join i2c bus (address optional for master)

  do {
    // Soft reset the sensor
    Wire.beginTransmission(ADDRESS); // VERİ AKTARIMI BAŞLIYOR
    Wire.write(0xFE); // 254 demek // SCL PİN A5 VE SDA PİN A4 ÜZERİNDEN İŞLEM GERÇEKLEŞİYOR
    ret = Wire.endTransmission(); // 0 = başarılı // sıfır olmayan her durumda hata mesajı ver
    if (ret != 0) {
      Serial.println("Error while sending soft reset command, retrying...");
    }
  } while (ret != 0);

  delay(50); // chip'in resetlenmesi için 0.05 saniye bekle
}

// PROGRAM AKTİFKEN ÇALIŞACAK KISIM
void loop() {
  int ret; //ret bir tamsayı olacak
  uint16_t raw_sensor_value; // 16 bitlik bu değer sadece pozitif tam sayı alabilir(oluşacak hataları önlemek adına)
  int16_t signed_sensor_value; // 16 bitlik bu değer tam sayı alacak

  // ÖLÇÜM GERÇEKLEŞTİRMEK İÇİN İLK OLARAK 243 Ü ÖLÇÜM MODUNA ALIYORUZ
  // // SONRA 2 BYTE + 1 CRC BYTE'I SENSÖRDEN OKUYORUZ
  Wire.beginTransmission(ADDRESS); //
  Wire.write(0xF1); // 243 demek
  ret = Wire.endTransmission(); 
  if (ret != 0) { // BU SEFER DE PROGRAM ÇALIŞIRKEN DURUMU KONTROL EDİP ONA GÖRE HATA VERİCEK
    Serial.println("Error during write measurement mode command");

  } else { // RET 0 İSE, YANİ İŞLEM BAŞARILIYSA ÇALIŞTIR ;
    Wire.requestFrom(ADDRESS, 2);       // 2 BYTE OKUYOR // CRC BYTE'INI İGNORE LUYOR
    if (Wire.available() < 2) {  // I2C BUFFER'ININ UZZUNLUĞU 2 DEN KÜÇÜKSE ;
      Serial.println("Error while reading flow measurement"); // ERROR YAZ

    } else { //EĞER 2 DEN BÜYÜKSE ;
      raw_sensor_value  = Wire.read() << 8; // SENSÖRDEN MSB'Yİ OKUYOR VE << 8 FONKSİYONUYLA TAM SAYI GELEN VERİYİ 8 BİTE ÇEVİRİYOR
      raw_sensor_value |= Wire.read();      // SENSÖRDEN LSB'Yİ OKUYOR

      // ÖNEMLİ !!!
      // raw_sensor_value'yu üstte tanımladık. fakat tekrar tanım koyduk ve değerimize dedik ki
      // raw_sensor_value = 8 bitlik msb. hemen altında ise 'raw_sensor_value |=' komutu şu anlama geldi
      // raw_sensor_value = raw_sensor_value(8 bitlik hali) YA DA raw_sensor_value = raw_Sensor_value(tamsayı hali)
      // burdaki ana konsepti şimdilik çözemedim fakat veriyi her türlü alıcağımız kesin.



      Serial.print("raw value from sensor: "); // ARDUİNO CONSOLE'UNA RAW VALUE FROM SENSÖR'Ü YAZDIRIYOR
      Serial.print(raw_sensor_value); // VE RAW_SENSOR_VALUE DEĞERİNİ YAZIYOR

      signed_sensor_value = (int16_t) raw_sensor_value; // RAW_SENSOR_VALUE DEĞERİNİ 16 BİTLİK TAM SAYIYA ÇEVİRİP SIGNED_SENSOR_VALUE DEĞERİNE EŞİTLİYOR
      Serial.print(", signed value: "); // SIGNED VALUE YAZDIRIYOR
      Serial.println(signed_sensor_value); // SIGNED SENSOR VALUE DEĞERİNİ YAZDIRIYOR
    }
  }

  delay(1000); // HER İŞLEMİ 1 SANİYELİK ARALIKLARLA KONTROL EDİYOR. YANİ SANİYEDE 1 TÜM DEĞERLERİ YENİLİYOR
}
