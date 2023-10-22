/*
 * File: RAK4631-R_Sleep_Test_BLE.ino
 *
 * Monitoring the voltage of the solar cell and battery 
 * to get information on the battery's state of charge
 * Sleep time is 30 seconds.
 *
 * 2023-10-22 Claus Kühnel info@ckuehnel.ch
 */
 
char data_array[100] = {0};

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

  Serial.println("\nMonitoring Vsol & Vbat - BLE output");
  Serial.println("-----------------------------------");

	// Create a unified timer
  if (api.system.timer.create(RAK_TIMER_0, (RAK_TIMER_HANDLER) handler, RAK_TIMER_PERIODIC) != true) 
  {
    Serial.printf("Creating timer failed.\r\n");
    return;
  }
  // Set timer periode to 30 s and start timer
  if (api.system.timer.start(RAK_TIMER_0, 1000 * 30 , NULL) != true) 
  {
    Serial.printf("Starting timer failed.\r\n");
    return;
  }
  //If you want to read and write data through BLE API operations, you need to set BLE Serial (Serail6) to Custom Mode
  Serial6.begin(115200, RAK_CUSTOM_MODE);

  uint8_t pairing_pin[] = "123456";   // change it 
  Serial.print("Setting pairing PIN to: ");
  Serial.println((char *) pairing_pin);
  api.ble.uart.setPIN(pairing_pin, 6);	//pairing_pin = 6-digit (digit 0..9 only)

  // Set Permission to access BLE Uart is to require man-in-the-middle protection
  // This will cause apps to perform pairing with static PIN we set above
  // now support SET_ENC_WITH_MITM and SET_ENC_NO_MITM
  api.ble.uart.setPermission(RAK_SET_ENC_WITH_MITM);
  api.ble.uart.start(0);
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
  
  String data = "\nVsol = " + String(Vsol, 3) + " V  Vbat = " + String(Vbat, 3) + " V  State = " + String(State, 1);
  uint16_t len = data.length();
  data.toCharArray(data_array, len+1);
  api.ble.uart.write((uint8_t*)data_array, len);
}

// This example is complete timer driven. 
// The loop() does nothing than sleep.
void loop()
{
	api.system.sleep.all();
  api.system.scheduler.task.destroy();
}