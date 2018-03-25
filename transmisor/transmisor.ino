#include <LowPower.h>
#include <VirtualWire.h>


const int dataPin = 9;
const int rainSensor = 0;
int sensor = 0;
int constantCicles = 0;
int oldData = 0;
const char* oseg = "l";
const char* cseg = "s";

void setup()
{
  Serial.begin(9600);
  vw_setup(2000);
  vw_set_tx_pin(dataPin);
  oldData = analogRead(rainSensor);
}

void loop()
{
  String strSensor;
  char buf[VW_MAX_MESSAGE_LEN];
  sensor = analogRead(rainSensor);
  strSensor = "k" + String(sensor);
  strSensor.toCharArray(buf, sizeof(buf));
  if ((oldData - sensor) > 20) {
    constantCicles = 0;
  } else {
    constantCicles++;
  }
  vw_send((uint8_t *)buf, strlen(buf));
  vw_wait_tx();
  LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF);
  if (constantCicles >= 40) {
    vw_send((uint8_t *)oseg, strlen(oseg));
    vw_wait_tx();
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  } else if (constantCicles >= 20) {
    vw_send((uint8_t *)cseg, strlen(cseg));
    vw_wait_tx();
    LowPower.powerDown(SLEEP_4S, ADC_OFF, BOD_OFF);
  }
  oldData = sensor;
}
