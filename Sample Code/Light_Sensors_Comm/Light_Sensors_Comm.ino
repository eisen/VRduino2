/*******************************************************************
    Copyright (C) 2017 Triad Semiconductor

    Example application for configuring the Triad
    Semiconductor TS4231 Light to Digital converter.

    Created by: John Seibel
    Modified by: Eisen Montalvo
*******************************************************************/
#include <ts4231.h>

#define light_timeout   500  //500ms is a placeholder as this number will be system dependent

//IMPORTANT NOTES:
//1)  If porting the TS4231 library code to a non-Arduino architecture,
//    be sure that the INPUT ports assigned to the E and D signals are configured as
//    floating inputs with NO pull-up or pull-down function.  Using a pull-up or
//    pull-down function on the inputs will cause the TS4231 to operate incorrectly.
//2)  If a microcontroller is being used that can change states on the E and D
//    outputs faster than approximately 100ns, see the IMPORTANT NOTES section
//    in file ts4231.cpp for more information.

#define device1_E_pin   9
#define device1_D_pin   10

#define device2_E_pin   12
#define device2_D_pin   11

#define device3_E_pin   13
#define device3_D_pin   14

#define device4_E_pin   18
#define device4_D_pin   19

uint8_t config_result;

TS4231  device1(device1_E_pin, device1_D_pin);  //instantiate the class as device1 and assign pins
TS4231  device2(device2_E_pin, device2_D_pin);  //instantiate the class as device2 and assign pins
TS4231  device3(device3_E_pin, device3_D_pin);  //instantiate the class as device3 and assign pins
TS4231  device4(device4_E_pin, device4_D_pin);  //instantiate the class as device4 and assign pins

bool setupTS4231(TS4231 device, String name)
{
  bool configured = false;
  Serial.println(name);
  
  //The application must verify that each TS4231 has detected light prior to
  //configuring the device.  The waitForLight() function is executed after power-up
  //on an un-configured device.  Once the device has detected light, waitForLight()
  //does not need to be executed again unless the device loses power.
  if (device.waitForLight(light_timeout)) {
    //Execute this code when light is detected
    Serial.println("\tLight DETECTED");

    config_result = device.configDevice(0x392B);

    //user can determine how to handle each return value for the configuration function
    switch (config_result) {
      case CONFIG_PASS:
        Serial.println("\tConfiguration SUCCESS");
        break;
      case BUS_FAIL:  //unable to resolve state of TS4231 (3 samples of the bus signals resulted in 3 different states)
        Serial.println("\tConfiguration Unsuccessful - BUS_FAIL");
        break;
      case VERIFY_FAIL:  //configuration read value did not match configuration write value, run configuration again
        Serial.println("\tConfiguration Unsuccessful - VERIFY_FAIL");
        break;
      case WATCH_FAIL:  //verify succeeded but entry into WATCH mode failed, run configuration again
        Serial.println("\tConfiguration Unsuccessful - WATCH_FAIL");
        break;
      default:  //value returned was unknown
        Serial.println("\tProgram Execution ERROR");
        break;
      }
      configured = true;
    }
  else {
    //insert code here for no light detection
    Serial.println("\tLight TIMEOUT");
    }
    Serial.println("");

    return configured;
}

volatile byte envelope1State = 0;
volatile byte envelope2State = 0;
volatile byte envelope3State = 0;
volatile byte envelope4State = 0;

void envelope1H()
{
  envelope1State = digitalRead(device1_E_pin);
}

void envelope1L()
{
  envelope1State = digitalRead(device1_E_pin);
}

void envelope3()
{
  envelope3State = digitalRead(device3_E_pin);
}

void envelope4()
{
  envelope4State = digitalRead(device4_E_pin);
}

void setup() {
  Serial.begin(115200);
  while (!Serial);  //wait for serial port to connect

  Serial.println("Serial Port Connected");
  Serial.println();

  bool dev1cfg = false;
  bool dev2cfg = false;
  bool dev3cfg = false;
  bool dev4cfg = false;

  do {
    dev1cfg = setupTS4231(device1, "Upper Left Sensor:");
    dev2cfg = setupTS4231(device2, "Bottom Left Sensor:");
    dev3cfg = setupTS4231(device3, "Bottom Right Sensor:");
    dev4cfg = setupTS4231(device4, "Top Right Sensor:");
  } while( !dev1cfg || !dev2cfg || !dev3cfg || !dev4cfg );

  Serial.println("");

  pinMode(device1_E_pin, INPUT);
  pinMode(device2_E_pin, INPUT);
  pinMode(device3_E_pin, INPUT);
  pinMode(device4_E_pin, INPUT);

  pinMode(device1_D_pin, INPUT);
  pinMode(device2_D_pin, INPUT);
  pinMode(device3_D_pin, INPUT);
  pinMode(device4_D_pin, INPUT);

//  attachInterrupt(digitalPinToInterrupt(device1_E_pin), envelope1H, HIGH);
//  attachInterrupt(digitalPinToInterrupt(device1_E_pin), envelope1L, LOW);
//  attachInterrupt(digitalPinToInterrupt(device3_E_pin), envelope3, CHANGE);
//  attachInterrupt(digitalPinToInterrupt(device4_E_pin), envelope4, CHANGE);

  Serial.println("E1 E2 E3 E4");
  
  }

void loop() {
  Serial.print(digitalRead(device1_E_pin));
  Serial.print(" ");
  Serial.print(digitalRead(device2_E_pin));
  Serial.print(" ");
  Serial.print(digitalRead(device3_E_pin));
  Serial.print(" ");
  Serial.print(digitalRead(device4_E_pin));
  Serial.println("");
  delayMicroseconds(20000);
}
