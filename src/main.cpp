
#include <Arduino.h>
#include <PS4Controller.h>
#include <cmath>

int check(const int border, const int var);

const int DIR0PIN = 14;
const int PWM0PIN = 12;
const int DIR1PIN = 33;
const int PWM1PIN = 25;
const int DIR2PIN = 27;
const int PWM2PIN = 26;
bool dir0;
bool dir1;
bool dir2;
int pwm0;
int pwm1;
int pwm2;

void setup() {
    Serial.begin(115200);
    PS4.begin("C0:49:EF:E4:81:88");
    Serial.println("Ready.");
    pinMode(DIR0PIN, OUTPUT);
    pinMode(DIR1PIN, OUTPUT);
    pinMode(DIR2PIN, OUTPUT);
    ledcSetup(0, 12800, 8);
    ledcSetup(1, 12800, 8);
    ledcSetup(2, 12800, 8);
    ledcAttachPin(PWM0PIN, 0);
    ledcAttachPin(PWM1PIN, 1);
    ledcAttachPin(PWM2PIN, 2);
}

void loop() {

  if (!PS4.isConnected()) {
    ledcWrite(0, 0);
    ledcWrite(1, 0);
    ledcWrite(2, 0);
    return;
  }

  int l_x = map(PS4.LStickX(), -127, 127, -255, 255);
  int l_y = map(PS4.LStickY(), -127, 127, -255, 255);
  int r_x = map(PS4.RStickX(), -127, 127, -255, 255);
  int r_y = map(PS4.RStickY(), -127, 127, -255, 255);

  if (PS4.R1()) {
      digitalWrite(DIR0PIN, 0);
      digitalWrite(DIR1PIN, 0);
      digitalWrite(DIR2PIN, 0);
      ledcWrite(0, 255);
      ledcWrite(1, 255);
      ledcWrite(2, 255);
  } else if (PS4.L1()) {
      digitalWrite(DIR0PIN, 1);
      digitalWrite(DIR1PIN, 1);
      digitalWrite(DIR2PIN, 1);
      ledcWrite(0, 255);
      ledcWrite(1, 255);
      ledcWrite(2, 255);
  } else {

      //l_x = abs(l_x) < 20 ? 0 : l_x;
      //l_y = abs(l_y) < 20 ? 0 : l_y;
      //r_x = abs(r_x) < 20 ? 0 : r_x;
      //r_y = abs(r_y) < 20 ? 0 : r_y;

      pwm0 = l_x;
      pwm1 = -l_x / 2 + l_y * sqrt(3) / 2;
      pwm2 = -l_x / 2 - l_y * sqrt(3) / 2;

      pwm0 = check(20, pwm0);
      pwm1 = check(20, pwm1);
      pwm2 = check(20, pwm2);

      dir0 = pwm0 < 0 ? 1 : 0;
      dir1 = pwm1 < 0 ? 1 : 0;
      dir2 = pwm2 < 0 ? 1 : 0;

      digitalWrite(DIR0PIN, dir0);
      digitalWrite(DIR1PIN, dir1);
      digitalWrite(DIR2PIN, dir2);

      ledcWrite(0, abs(pwm0));
      ledcWrite(1, abs(pwm1));
      ledcWrite(2, abs(pwm2));

      // Serial.printf("abs(pwm0) is %d\n", abs(pwm0));
      // Serial.printf("abs(pwm1) is %d\n", abs(pwm1));
      // Serial.printf("abs(pwm2) is %d\n", abs(pwm2));
  }
}

int check(const int border, const int var){
  return abs(var) < border ? 0 : var;
}