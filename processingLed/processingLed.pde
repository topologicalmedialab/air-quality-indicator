import oscP5.*;
import netP5.*;

import controlP5.*;

ControlP5 cp5;

OscP5 oscP5;
NetAddress myRemoteLocation;

import processing.serial.*;

Serial myPort;  // Create object from Serial class
int val;        // Data received from the serial port

public void controlEvent(ControlEvent theEvent) {
  int r = (int(cp5.getController("sliderR").getValue()));
  int g = (int(cp5.getController("sliderG").getValue()));
  int b = (int(cp5.getController("sliderB").getValue()));

  myPort.write("setColor ");
  myPort.write(nf(r, 3));
  myPort.write(nf(g, 3));
  myPort.write(nf(b, 3));
  myPort.write(10);

  println(r + " " + g + " " + b);
}

void setup() 
{
  size(800, 800);

  cp5 = new ControlP5(this);
  // name, minValue, maxValue, defaultValue, x, y, width, height
  cp5.addSlider("sliderR", 0, 50, 0, 100, 260, 600, 200).setId(1);
  cp5.addSlider("sliderG", 0, 50, 0, 100, 1300, 200, 30).setId(2);
  cp5.addSlider("sliderB", 0, 50, 0, 100, 1340, 200, 30).setId(3);

  // I know that the first port in the serial list on my mac
  // is always my  FTDI adaptor, so I open Serial.list()[0].
  // On Windows machines, this generally opens COM1.
  // Open whatever port is the one you're using.
  println((Object[])Serial.list());
  String portName = Serial.list()[Serial.list().length - 1];
  myPort = new Serial(this, portName, 115200);
  oscP5 = new OscP5(this, 12000);
}

void draw() {
  background(255);

  while ( myPort.available() > 0) {  // If data is available,
    val = myPort.read();         // read it and store it in val
    println(val);
  }

  rect(50, 50, 100, 100);         // Draw a square
}

void mouseMoved() {
  //if (mouseOverRect() == true) {  // If mouse is over square,
  //  fill(204);                    // change color and
  //  myPort.write("setColor ");
  //  myPort.write(100);
  //  myPort.write(100);
  //  myPort.write(10);
  //  myPort.write('\n');
  //} else {
  //  fill(0);
  //  myPort.write("setColor ");
  //  myPort.write(100);
  //  myPort.write(100);
  //  myPort.write(int(map(mouseX, 0, width, 0, 127)));
  //  myPort.write('\n');
  //}
}
boolean mouseOverRect() { // Test if mouse is over square
  return ((mouseX >= 50) && (mouseX <= 150) && (mouseY >= 50) && (mouseY <= 150));
}

void oscEvent(OscMessage theOscMessage) {
  /* check if theOscMessage has the address pattern we are looking for. */

  if (theOscMessage.checkAddrPattern("/muse/elements/alpha_relative")==true) {
    /* parse theOscMessage and extract the values from the osc message arguments. */
    float ch1 = theOscMessage.get(0).floatValue();  
    float ch2 = theOscMessage.get(1).floatValue();  
    float ch3 = theOscMessage.get(2).floatValue();  
    float ch4 = theOscMessage.get(3).floatValue();  
    print("### received an osc message /test with typetag" + theOscMessage.typetag());
    println(" values: "+ch1+" "+ch2+" "+ch3+" "+ch4);
    if (ch3 > 0.3) {
      myPort.write("setColor g\n");
    } else if (ch3 > 0.18) {
      myPort.write("setColor b\n");
    } else {
      myPort.write("setColor r\n");
    }
  }
}