#include <LowPower.h>
#include <VirtualWire.h>


const int dataPin = 9;
const int rainSensor = 0;
int sensor = 0;
int constantCicles = 0;
int oldData = 0;
char oseg[VW_MAX_MESSAGE_LEN];
char cseg[VW_MAX_MESSAGE_LEN];

void setup()
{
  Serial.begin(9600);
  vw_setup(2000);
  vw_set_tx_pin(dataPin);
  oldData = analogRead(rainSensor);
  String sleepMsg;
  sleepMsg = "p" + 8;
  sleepMsg.toCharArray(oseg,sizeof(oseg));
  sleepMsg = "p" + 4;
  sleepMsg.toCharArray(cseg,sizeof(cseg));
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
  Serial.print("LECTURA SENSOR: ");
  Serial.println(buf);
  Serial.print("LECTURA ANTIGUA: ");
  Serial.println(oldData);
  if (constantCicles >= 40) {
    Serial.println("8 segundos");
    vw_send((uint8_t *)oseg, strlen(oseg));
    vw_wait_tx();
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  } else if (constantCicles >= 20) {
    Serial.println("4 segundos");
    vw_send((uint8_t *)cseg, strlen(cseg));
    vw_wait_tx();
    LowPower.powerDown(SLEEP_4S, ADC_OFF, BOD_OFF);
  }
  oldData = sensor;
}
