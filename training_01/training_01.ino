#include <PS2X_lib.h> // thư viện tay cầm PS2
#include <Adafruit_PWMServoDriver.h> // thư viện chip điều khiển động cơ

PS2X ps2; // tay cầm PS2
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(); // chip ĐK động cơ

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); // khởi tạo truyền dữ liệu với máy tính qua serial console (115200 baud)

  Serial.println("Hello, World!"); // in ra serial console

  while(ps2.config_gamepad(13, 11, 10, 12) != 0) { // khởi tạo tay cầm
    Serial.println("PS2 error");
    delay(500);
  }

  /* khởi tạo chip ĐK động cơ */
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(50);
  
  Serial.println("OK");
}

void loop() {
  // put your main code here, to run repeatedly:
  ps2.read_gamepad(); // đọc từ tay cầm

  int v = map(ps2.Analog(PSS_LY), 0, 255, 4095, -4095); // lấy giá trị trục Y joystick trái và map từ 0..255 -> 4095..-4095
  if(v > 0) {
    pwm.setPWM(0, 0, v); // ghi cấu hình độ rộng xung cho kênh PWM
    pwm.setPWM(1, 0, 0);
  } else {
    pwm.setPWM(0, 0, 0);
    pwm.setPWM(1, 0, -v);
  }
}
