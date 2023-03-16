/* Code mâu robot Mock VRC 2023 - Code robot đội Xanh */

// Khai báo các thư viện cần dùng
#include <PS2X_lib.h>
#include <Adafruit_PWMServoDriver.h>

// Khai báo linh kiện
PS2X ps2;
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Cấu hình robot
// 1. Chân cắm đầu thu tay cầm PS2
#define PS2_ATT             10
#define PS2_CMD             11
#define PS2_DAT             12
#define PS2_CLK             13
// 2. Kênh PWM điều khiển động cơ
#define PWM_DC0A            0
#define PWM_DC0B            1
#define PWM_DC1A            2
#define PWM_DC1B            3
#define PWM_DC2A            4
#define PWM_DC2B            5
#define PWM_DC3A            6
#define PWM_DC3B            7
#define PWM_SERVO0          8
#define PWM_SERVO1          9
#define PWM_SERVO2          10
#define PWM_SERVO3          11
#define PWM_SERVO4          12
// 3. Động cơ DC
#define MOT_LEFT            1
#define MOT_RIGHT           2
// 4. Tốc độ động cơ
#define SPD_FAST            2047
#define SPD_SLOW            850
// Hàm điều khiển động cơ DC (nhận số động cơ từ 0->3 ứng với cặp kênh PWM 0-1/2-3/4-5/6-7, giá trị tốc độ từ -4095 đến 4095)
void ctrl_dc(uint8_t motor, int16_t speed) {
  switch(motor) {
    case 0:
      pwm.setPWM(PWM_DC0A, 0, (speed > 0) ? speed : 0);
      pwm.setPWM(PWM_DC0B, 0, (speed < 0) ? (-speed) : 0);
      break;
    case 1:
      pwm.setPWM(PWM_DC1A, 0, (speed > 0) ? speed : 0);
      pwm.setPWM(PWM_DC1B, 0, (speed < 0) ? (-speed) : 0);
      break;
    case 2:
      pwm.setPWM(PWM_DC2A, 0, (speed > 0) ? speed : 0);
      pwm.setPWM(PWM_DC2B, 0, (speed < 0) ? (-speed) : 0);
      break;
    case 3:
      pwm.setPWM(PWM_DC3A, 0, (speed > 0) ? speed : 0);
      pwm.setPWM(PWM_DC3B, 0, (speed < 0) ? (-speed) : 0);
      break;
  }
}

// Hàm điều khiển vị trí servo 180 (nhận số động cơ servo từ 0->4 ứng với kênh PWM 8->12, giá trị góc từ 0 đến 180)
void ctrl_servo180(uint8_t motor, float angle) {
  uint16_t us = (uint16_t) (1000 * (1.0 + angle / 180.0));
  switch(motor) {
    case 0: pwm.writeMicroseconds(PWM_SERVO0, us); break;
    case 1: pwm.writeMicroseconds(PWM_SERVO1, us); break;
    case 2: pwm.writeMicroseconds(PWM_SERVO2, us); break;
    case 3: pwm.writeMicroseconds(PWM_SERVO3, us); break;
    case 4: pwm.writeMicroseconds(PWM_SERVO4, us); break;
  }
}

// Hàm điều khiển tốc độ servo 360 (nhận số động cơ servo từ 0->4 ứng với kênh PWM 8->12, giá trị tốc độ từ -100 đến 100)
void ctrl_servo360(uint8_t motor, float speed) {
  uint16_t us = (uint16_t) (1000 * (1.5 + (speed / 100) * 0.5));
  switch(motor) {
    case 0: pwm.writeMicroseconds(PWM_SERVO0, us); break;
    case 1: pwm.writeMicroseconds(PWM_SERVO1, us); break;
    case 2: pwm.writeMicroseconds(PWM_SERVO2, us); break;
    case 3: pwm.writeMicroseconds(PWM_SERVO3, us); break;
    case 4: pwm.writeMicroseconds(PWM_SERVO4, us); break;
  }
}

// Chương trình khởi tạo
void setup() {
  Serial.begin(9600);
  
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(50);

  while(ps2.config_gamepad(PS2_CLK, PS2_CMD, PS2_ATT, PS2_DAT) != 0);

  Serial.println(F("OK"));
}

int drive_speed = SPD_FAST;

bool speed_toggled = false;

// Chương trình lặp
void loop() {
  // put your main code here, to run repeatedly:

  ps2.read_gamepad();
  
  ctrl_dc(MOT_LEFT, map(ps2.Analog(PSS_LY), 0, 255, -drive_speed, drive_speed));
  ctrl_dc(MOT_RIGHT, map(ps2.Analog(PSS_RY), 0, 255, drive_speed, -drive_speed));

  if(ps2.Button(PSB_L2)) {
    // Nhấn L2 để thay đổi cấu hình robot
    if(ps2.Button(PSB_TRIANGLE)) {
      // Đổi tốc độ động cơ
      if(!speed_toggled) {
        speed_toggled = true;
        drive_speed = (drive_speed == SPD_FAST) ? SPD_SLOW : SPD_FAST;  
      }
    } else speed_toggled = false;
  } else {
    speed_toggled = false; // tránh trường hợp nhả 2 nút cùng lúc
  }
}
