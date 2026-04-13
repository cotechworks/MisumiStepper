#include "MisumiStepper.h"

MisumiStepper::MisumiStepper( int enaPin, int dirPin, int pulPin, MovingMode mode, uint32_t handler_period_us, uint32_t pulse_count,
                              uint32_t speed, float acceleration )
{
  _enaPin = enaPin;
  _dirPin = dirPin;
  _pulPin = pulPin;
  _mode = mode;
  _handler_period_us = handler_period_us;
  _speed = speed;
  _target_speed = speed;
  _pulse_count = pulse_count;
  _acceleration = acceleration;

  pinMode( _enaPin, OUTPUT );
  pinMode( _dirPin, OUTPUT );
  pinMode( _pulPin, OUTPUT );
  digitalWrite( _enaPin, HIGH );
  digitalWrite( _dirPin, LOW );
  digitalWrite( _pulPin, LOW );
}

void MisumiStepper::setMovingMode( MovingMode m ) { _mode = m; }

void MisumiStepper::setSpeed( float speed )
{
  _target_speed = speed;
  if( _acceleration == 0.0 ) {
    _speed = speed;
  }
  delay( 10 );
}

void MisumiStepper::setSpeed( double speed ) { setSpeed( static_cast<float>( speed ) ); }

void MisumiStepper::setSpeed( int speed ) { setSpeed( static_cast<float>( speed ) ); }

float MisumiStepper::getSpeed() { return _speed; }

bool MisumiStepper::getState() { return _state; }

void MisumiStepper::start()
{
  _state = true;
  digitalWrite( _enaPin, LOW );
  digitalWrite( _dirPin, HIGH );
}

void MisumiStepper::hold()
{
  _state = false;
  _speed = 0.0;
  digitalWrite( _enaPin, LOW );
  digitalWrite( _dirPin, HIGH );
  digitalWrite( _pulPin, LOW );
}

void MisumiStepper::stop()
{
  _state = false;
  _speed = 0.0;
  digitalWrite( _enaPin, HIGH );
  digitalWrite( _dirPin, LOW );
  digitalWrite( _pulPin, LOW );
}

void MisumiStepper::StepperHandler()
{
  if( _state == false ) {
    return;
  }

  if( _mode == CONTINUOUS ) {
    // 時間の更新
    _time_count += _handler_period_us;

    // 速度の更新
    if( _acceleration > 0.0 ) {
      if( _speed < _target_speed ) {
        _speed += _acceleration;
        if( _speed > _target_speed ) _speed = _target_speed;
      } else if( _speed > _target_speed ) {
        _speed -= _acceleration;
        if( _speed < _target_speed ) _speed = _target_speed;
      }
    }

    // 回転方向の設定
    if( _speed >= 0 ) {
      digitalWrite( _dirPin, HIGH );
    } else {
      digitalWrite( _dirPin, LOW );
    }
    float abs_speed = abs( _speed );

    // パルスの生成間隔の計算
    uint32_t wait = static_cast<uint32_t>( 60000000.0 / ( static_cast<float>( _pulse_count ) * 2.0 * abs_speed ) );
    // パルスの生成
    if( _time_count >= wait ) {
      _time_count = 0;
      digitalWrite( _pulPin, 1 - digitalRead( _pulPin ) );
    }
  }
}