#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>    

#define RST_PIN A0    // 讀卡機的重製腳位
#define SS_PIN 10     // 晶片選擇腳位

struct RFIDTag {      // 定義結構
  byte uid[4];
  char *name;
};

struct RFIDTag tags[] = {                           
  {{ 80 , 241 , 52 , 30 }, "shiroikaado"}
};
byte totalTags = sizeof(tags) / sizeof(RFIDTag);    //計算結構資料筆數，結果為3
MFRC522 mfrc522(SS_PIN, RST_PIN);                   // 建立 MFRC522 物件
Servo servo;                                        // 建立 Servo 物件

void setup() {
  Serial.begin(9600);

  Serial.println("RFID reader is ready");

  SPI.begin();
  mfrc522.PCD_Init();                               // 初始化 MFRC522 讀卡機

//  servo.attach( $ );
//
//  servo.write(0);
  delay(200);
}
void loop() {
  // 確認是否有新卡片
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    byte *id = mfrc522.uid.uidByte;                  // 取得卡片UID
    byte idSize = mfrc522.uid.size;                  // 取得卡片UID長度
    bool foundTag = false;                           // 是否找到卡片的標籤，預設值為否

    for (byte i = 0; i < totalTags; i++) {
      if (memcmp(tags[i].uid, id, idSize) == 0) {    // 比對陣列中的資料
        Serial.println(tags[i].name);                // 顯示標籤名稱
        foundTag =  true  ;

 // 讀到卡的時候要 ......
      }
    }

    if (!foundTag) {                                // foundTag 或 !foundTag 二選一
      // 若掃描到記錄之外的標籤
      Serial.println("Wrong card");                  // 則顯示 Wrong card
    }
    mfrc522.PICC_HaltA();                            // 讓卡面進入停止狀態
  }
}
