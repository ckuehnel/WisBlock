/*
 * File: RUI_Sleep_Test_LoRaWAN.ino
 *
 * Monitoring the power consumption of LoRaWAN nodes
 * for different core modules running RUI3.
 * WisToolBox is used for Configuration of LoRaWAN parameters.
 * Sensor data are simulated. 
 * Sleep time is 300 seconds.
 *
 * Select the right core module and serial port for download.
 *
 * 2023-12-08 Claus Kühnel info@ckuehnel.ch
 */

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

// Simulated sensor data
float temperature, humidity, pressure;

uint8_t data_array[8] = {0};

#define DEBUG 0   // DEBUG controls serial output

void setup()
{
  uint32_t t = millis();
  uint32_t timeout = 5000;

	pinMode(LED_GREEN, OUTPUT);
	digitalWrite(LED_GREEN, LOW);

	Serial.begin(115200);
  while (!Serial) if (millis() - t > timeout) break;

  Serial.println("\nMonitoring Power Consumption of Core Modules - LoRaWAN output");
  Serial.println("-------------------------------------------------------------");

  Serial.printf("Set the low power mode %s\n\r", api.system.lpm.set(1) ? "Success" : "Fail");
  
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
  // Set timer periode to 1 min and start timer
  if (api.system.timer.start(RAK_TIMER_0, 1000 * 300 , NULL) != true) 
  {
    Serial.printf("Starting timer failed.\r\n");
    return;
  }

  api.lorawan.join();

  //wait for Join success
  while (api.lorawan.njs.get() == 0)
  {
    Serial.print("Waiting for Lorawan join...");
    api.lorawan.join();
    delay(10000);
  }
  initSensor();
  delay(100);
}

void handler(void *)
{
  digitalWrite(LED_GREEN, HIGH);
  delay(20);
  digitalWrite(LED_GREEN, LOW);

  readSensor();

  uint16_t core = 4631;  // this is the used core module
  int16_t temp = temperature * 10;
  uint16_t humi = humidity * 10;
  uint16_t press = pressure * 10; 
  if (DEBUG) Serial.printf("Payload: %d %d %d %d\n", core, temp, humi, press);

  data_array[0] = highByte(core);
  data_array[1] = lowByte(core);
  data_array[2] = highByte(temp);
  data_array[3] = lowByte(temp);
  data_array[4] = highByte(humi);
  data_array[5] = lowByte(humi);
  data_array[6] = highByte(press);
  data_array[7] = lowByte(press);

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