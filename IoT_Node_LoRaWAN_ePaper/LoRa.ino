// LoRa Call Backs

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