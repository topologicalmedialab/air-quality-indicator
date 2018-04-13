#include <SerialCommand.h>

int led = 9;           // the PWM pin the LED is attached to

SerialCommand sCmd;     // The SerialCommand object

int delayval = 500; // delay for half a second

void setup() {
  Serial.begin(115200);
  pinMode(led, OUTPUT);
  sCmd.addCommand("setColor",    setColor);          // Set LED color
}

int cr = 0, cg = 0, cb = 0;
void loop() {
  sCmd.readSerial();
  delay(10);
  analogWrite(led, cr);
}

int toDigit(char a) {
  int d = a - '0';
  if (d < 0) d = 0;
  if (d > 9) d = 0;
  return d;
}
void setColor() {
  char *arg;
  arg = sCmd.next();
  Serial.println(arg);
  cr = toDigit(arg[0]) * 100 + toDigit(arg[1]) * 10 + toDigit(arg[2]);
  cg = toDigit(arg[3]) * 100 + toDigit(arg[4]) * 10 + toDigit(arg[5]);
  cb = toDigit(arg[6]) * 100 + toDigit(arg[7]) * 10 + toDigit(arg[8]);

}
