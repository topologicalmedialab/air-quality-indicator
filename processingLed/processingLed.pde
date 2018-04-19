import controlP5.*;

ControlP5 cp5;

import processing.serial.*;

Serial myPort;  // Create object from Serial class
int val;        // Data received from the serial port
int r = 0;
int g = 0;
int b = 0;

public void controlEvent(ControlEvent theEvent) {
  int rr = (int(cp5.getController("sliderR").getValue()));
  int gg = (int(cp5.getController("sliderG").getValue()));
  int bb = (int(cp5.getController("sliderB").getValue()));
  r = (int)lerp(r, rr, 0.1);
  g = (int)lerp(g, gg, 0.1);
  b = (int)lerp(b, bb, 0.1);

  myPort.write("setColor ");
  myPort.write(nf(rr, 3));
  myPort.write(nf(gg, 3));
  myPort.write(nf(bb, 3));
  myPort.write(10);

  println(r + " " + g + " " + b);
}

void setup() 
{
  size(800, 800);

  cp5 = new ControlP5(this);
  // name, minValue, maxValue, defaultValue, x, y, width, height
  cp5.addSlider("sliderR", 0, 255, 0, 100, 260, 600, 30).setId(1);
  cp5.addSlider("sliderG", 0, 255, 0, 100, 300, 600, 30).setId(2);
  cp5.addSlider("sliderB", 0, 255, 0, 100, 340, 600, 30).setId(3);

  // I know that the first port in the serial list on my mac
  // is always my  FTDI adaptor, so I open Serial.list()[0].
  // On Windows machines, this generally opens COM1.
  // Open whatever port is the one you're using.
  println((Object[])Serial.list());
  String portName = Serial.list()[Serial.list().length - 1];
  myPort = new Serial(this, portName, 115200);
}

void draw() {
  background(255);

  while ( myPort.available() > 0) {  // If data is available,
    val = myPort.read();         // read it and store it in val
    println(val);
  }

  rect(50, 50, 100, 100);         // Draw a square
}