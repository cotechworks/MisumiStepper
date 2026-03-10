#pragma once

#include <Arduino.h>

enum MovingMode { CONTINUOUS, STEP };

class MisumiStepper {
 public:
  /**
   * コンストラクタ
   * @param enaPin ENAピン
   * @param dirPin DIRピン
   * @param pulPin PULピン
   * @param mode 動作モード (CONTINUOUS or STEP)
   * @param handler_period_us ハンドラの呼び出し周期(us)
   * @param pulse_count 1回転あたりのパルス数
   * @param speed 速度 (rpm)
   * @param target_speed 目標速度 (rpm)
   */
  MisumiStepper(int enaPin, int dirPin, int pulPin, MovingMode mode,
                uint32_t period_us, uint32_t pulse_num, uint32_t speed = 60.0,
                float acceleration = 0.0);

  /**
   * 動作モードの設定
   * @param m 動作モード (CONTINUOUS or STEP)
   */
  void setMovingMode(MovingMode m);
  /**
   * 速度の設定
   * @param speed 速度 (rpm)
   */
  void setSpeed(float speed);
  void setSpeed(int speed);
  void setSpeed(double speed);
  /**
   * 速度の取得
   * @return 速度 (rpm)
   */
  float getSpeed();
  /**
   * 動作開始
   */
  void start();
  /**
   * 動作停止 (停止時の位置を保持)
   */
  void hold();
  /**
   * 動作停止
   */
  void stop();
  /**
   * ステッパーモーターのハンドラ (タイマー割り込み等で定期的に呼び出す)
   */
  void StepperHandler();

 private:
  bool _state = false;            // true: 動作中, false: 停止中
  MovingMode _mode = CONTINUOUS;  // 動作モード
  int _enaPin;                    // ENAピン
  int _dirPin;                    // DIRピン
  int _pulPin;                    // PULピン
  uint32_t _handler_period_us;    // ハンドラの呼び出し周期(us)
  float _speed;                   // 速度(rpm)
  float _target_speed;            // 目標速度(rpm)
  float _acceleration;            // 加速度(rpm/_handler_period_us)
  uint32_t _time_count = 0;       // ハンドラ呼び出し間隔の積算値(us)
  uint32_t _pulse_count;          // 1回転あたりのパルス数
};
