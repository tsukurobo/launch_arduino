 #include "cubic_arduino.h"

#define ROTATE 3
#define LIFT 6
/*モーターの利用*/
// put関数で各モータのduty比指定，duty比の符号反転で逆回転
// Dutyの最大値はデフォルトで1000

/*エンコーダの利用*/
// get関数で各エンコーダの値を取得．Inc_encは1ms辺りの変化量，Abs_encは絶対位置[0, 16383]

/*ソレノイドの利用*/
// put関数で各ソレノイドの状態指定
// get関数で各ソレノイドの状態を取得

// 各種print関数はSerial.begin()を実行したあとでないと使えない．(ROSと一緒には使えない)

// 詳しくはヘッダファイル参照

void setup() {
  // すべてのモータ，エンコーダの初期化
  Cubic::begin(10.0);
  Serial.begin(115200);
}

int duty = 0; // 750:720(3型)
int rotate_duty = 50;
bool state = 1;

void loop() {
  // シリアル入力でdutyまたはstateの値を指定
  if (Serial.available() > 0) {
    //* duty各々指定
    //uint8_t i = Serial.readStringUntil(':').toInt();
    //duty = Serial.readStringUntil('\n').toInt();
    //DC_motor::put(i, duty);
    char mode = Serial.read();

    if (mode == 'a') {
      //* duty一括指定
      duty = Serial.readStringUntil('\n').toInt();
      DC_motor::put(0, -duty);
      DC_motor::put(1, duty);
      DC_motor::put(2, -duty);
      DC_motor::put(3, -duty);
    } else if (mode == 'u') {
      //* duty上指定
      duty = Serial.readStringUntil('\n').toInt();
      DC_motor::put(1, duty);
      DC_motor::put(3, -duty);
     } else if (mode == 'd') {
      //* duty下指定
      duty = Serial.readStringUntil('\n').toInt();
      DC_motor::put(0, -duty);
      DC_motor::put(2, -duty);
    } else if (mode == 'q') {
      //* duty左指定
      duty = Serial.readStringUntil('\n').toInt();
      DC_motor::put(2, -duty);
      DC_motor::put(3, -duty);
     } else if (mode == 'w') {
      //* duty右指定
      duty = Serial.readStringUntil('\n').toInt();
      DC_motor::put(0, -duty);
      DC_motor::put(1, duty);
    } else if (mode == 'l') {
      //* 左回転
      int now = millis();
      while (millis() - now < 500) {
        DC_motor::put(ROTATE, rotate_duty);
        Cubic::update();
        // delay(1);
      }
      DC_motor::put(ROTATE, 0);
    } else if (mode == 'r') {
      //* 右回転
      int now = millis();
      while (millis() - now < 1000) {
        DC_motor::put(ROTATE, -rotate_duty);
        Cubic::update();
        // delay(1);
      }
      DC_motor::put(ROTATE, 0);
    } else if (mode == 'e') {
      //* duty昇降指定
      duty = Serial.readStringUntil('\n').toInt();
      DC_motor::put(LIFT, duty);
    }
  }

  //for (int i = 0; i < 8; i++) {
    // i番モータにdutyを指定
    // DC_motor::put(i, duty);

    // i番インクリメントエンコーダの値の差分を取得し表示
    //int enc_diff = Inc_enc::get(i);
    //Serial.print(enc_diff); Serial.print(" ");
  //}
  //Serial.print(" ");

  // すべてのアブソリュートエンコーダの値を表示
  // Abs_enc::print();

  //for (int i = 0; i < 4; i++) {
    // 各ソレノイドの状態を格納
    // Solenoid::put(i, state);
  //}
  // すべてのソレノイドの状態を表示
  //Solenoid::print();

  // すべてのモータのDuty(最大32766)を表示
  DC_motor::print(true);

  // データの送受信を行う
  Cubic::update();

  // delay(1);
}
