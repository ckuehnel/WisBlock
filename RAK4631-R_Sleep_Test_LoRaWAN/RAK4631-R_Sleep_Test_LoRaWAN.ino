/*
 * File: RAK4631-R_Sleep_Test_LoRaWAN.ino
 *
 * Monitoring the voltage of the solar cell and battery 
 * to get information on the battery's state of charge
 * Sleep time is 30 seconds.
 *
 * 2023-10-22 Claus Kühnel info@ckuehnel.ch
 */

// OTAA Device EUI MSB
uint8_t node_device_eui[8] = {0xAC, 0x1F, 0x09, 0xFF, 0xFE, 0x08, 0xEA, 0x86};
// OTAA Application EUI MSB
uint8_t node_app_eui[8] = {0xAC, 0x1F, 0x09, 0xFF, 0xF8, 0x68, 0x46, 0x31};
// OTAA Application Key MSB
uint8_t node_app_key[16] = {0xAC, 0x1F, 0x09, 0xFF, 0xFE, 0x08, 0xEA, 0x86, 0xAC, 0x1F, 0x09, 0xFF, 0xF8, 0x68, 0x46, 0x31};

void recv_cb(SERVICE_LORA_RECEIVE_T *data) 
{
  Serial.println("Something received!");
  for (int i = 0 ; i < data->BufferSize ; i++) 
  {
    Serial.printf("%x", data->Buffer[i]);
  }
  Serial.print("\r\n");
}

void join_cb(int32_t status) 
{
  Serial.printf("Join status: %d\r\n", status);
}

void send_cb(int32_t status) 
{
  Serial.printf("Send status: %d\r\n", status);
}

uint8_t data_array[5] = {0};

uint8_t AIN1 = WB_A1;   // for RAK19007

#define DEBUG 0

void setup()
{
  uint32_t t = millis();
  uint32_t timeout = 5000;

	pinMode(LED_GREEN, OUTPUT);
	digitalWrite(LED_GREEN, LOW);

	Serial.begin(115200);
  while (!Serial) if (millis() - t > timeout) break;

  Serial.println("\nMonitoring Vsol & Vbat - LoRaWAN output");
  Serial.println("---------------------------------------");
  
  // Setup the callbacks for joined and send finished
  api.lorawan.registerRecvCallback(recv_cb);
  api.lorawan.registerSendCallback(send_cb);
  api.lorawan.registerJoinCallback(join_cb);

	// Create a unified timer
  if (api.system.timer.create(RAK_TIMER_0, (RAK_TIMER_HANDLER) handler, RAK_TIMER_PERIODIC) != true) 
  {
    Serial.printf("Creating timer failed.\r\n");
    return;
  }
  // Set timer periode to 5 min and start timer
  if (api.system.timer.start(RAK_TIMER_0, 1000 * 300 , NULL) != true) 
  {
    Serial.printf("Starting timer failed.\r\n");
    return;
  }
  api.lorawan.appeui.set(node_app_eui, 8);
  api.lorawan.appkey.set(node_app_key, 16);
  api.lorawan.deui.set(node_device_eui, 8);

  Serial.printf("Set adaptive data rate %s\r\n", api.lorawan.adr.set(1) ? "Success" : "Fail");
  Serial.printf("Set EU868 region %s\r\n", api.lorawan.band.set(4) ? "Success" : "Fail");
  Serial.printf("Set join mode to OTAA %s\r\n", api.lorawan.njm.set(1) ? "Success" : "Fail");
  
  api.lorawan.join();

  //wait for Join success
  while (api.lorawan.njs.get() == 0)
  {
    Serial.print("Waiting for Lorawan join...");
    api.lorawan.join();
    delay(10000);
  }
  initADC();
  delay(100);
}

void handler(void *)
{
  float Vsol = readADC(AIN1)*.987;
  float Vbat = api.system.bat.get(); 
  float State = Vsol/Vbat;
	Serial.printf("Vsol = %5.3f V  Vbat = %5.3f V  State = %3.1f\r\n", Vsol, Vbat, State );

  digitalWrite(LED_GREEN, HIGH);
  delay(20);
  digitalWrite(LED_GREEN, LOW);

  Vsol *= 1000; Vbat *= 1000; bool bState = (State < 1)? false : true;
  data_array[0] = highByte((uint16_t) Vsol);
  data_array[1] = lowByte((uint16_t) Vsol);
  data_array[2] = highByte((uint16_t) Vbat);
  data_array[3] = lowByte((uint16_t) Vbat);
  data_array[4] = bState;

  //wait for Join success
  while (api.lorawan.njs.get() == 0)
  {
    Serial.print("Waiting for Lorawan join...");
    api.lorawan.join();
    delay(10000);
  }
  if (api.lorawan.send(sizeof(data_array), data_array, 2, false, 1)) Serial.println("Send Success");  // Port = 2, confirmation false, retry = 1
  else Serial.println("Send fail");
}

// This example is complete timer driven. 
// The loop() does nothing than sleep.
void loop()
{
	api.system.sleep.all();
  api.system.scheduler.task.destroy();
}