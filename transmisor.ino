#include <VirtualWire.h>
 
const int dataPin = 9;
const int rainSensor = 0;
int sensor = 0;
 
void setup()
{
    Serial.begin(9600);     
    vw_setup(2000);
    vw_set_tx_pin(dataPin);
    
}
 
void loop()
{
    String strSensor;
    char buf[VW_MAX_MESSAGE_LEN];
      sensor = analogRead(rainSensor);
      strSensor = "k"+String(sensor);
      strSensor.toCharArray(buf,sizeof(buf));
      vw_send((uint8_t *)buf, strlen(buf)); // Enviar array
      vw_wait_tx(); // Esperar envio         
      Serial.print("MENSAJE ENVIADO: ");
      Serial.println(buf);
      delay(1000);
}
