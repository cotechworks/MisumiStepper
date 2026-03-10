#include <Arduino.h>
#include <SimpleTask.h>

// インスタンス化
CycleTask   led_on(1000);  // 1000ms周期でフラグをセット
OneShotTask led_off;       // ワンショットタスク
OneShotTask SerialSend;    // ワンショットタスク

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // 周期タスク開始
  led_on.start();
}

void loop() {
  // タスクの更新
  // 1000Hz以上の周期で呼び出すこと
  led_on.update();
  led_off.update();

  // LED_ONタスクが発火
  if (led_on.isFired()) {
    digitalWrite(LED_BUILTIN, HIGH);  // LEDをONにする
    led_off.start(500);               // 500ms後にフラグをセット
  }

  // LED_OFFタスクが発火
  if (led_off.isFired()) {
    digitalWrite(LED_BUILTIN, LOW);  // LEDをOFFにする
    SerialSend.start();              // 引数が0なので即座にフラグをセット
  }

  // シリアル送信タスクが発火
  if (SerialSend.isFired()) {
    Serial.println("Hello, SimpleTask!");
  }
}