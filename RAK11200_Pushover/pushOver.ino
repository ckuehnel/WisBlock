void pushover(char *pushovermessage, int priority)
{
  String message = pushovermessage;

  int length = 113 + message.length();

  if(client.connect(pushoversite,80))
  {
    client.println("POST /1/messages.json HTTP/1.1");
    client.println("Host: api.pushover.net");
    client.println("Connection: close\r\nContent-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.print(length);
    client.println("\r\n");;
    client.print("token=");
    client.print(apitoken);
    client.print("&user=");
    client.print(userkey);
    client.print("&message=");
    client.print(message);
    client.print("&priority=");
    client.print(priority);
    client.print("&retry=60");
    client.print("&expire=3600");
    while(client.connected())  
    {
      while(client.available())
      {
        char ch = client.read();
        Serial.write(ch);
      }
    }
    client.stop();
  }  
}

void sendMessage(void)
{
  if (WiFi.status() != WL_CONNECTED) 
  {
    Serial.println("Reconnecting to WiFi...");
    //ESP.restart();
  }
    
  String message  = "Message from RAK11200";

  char arr[message.length() + 1];
  for (int i = 0; i < sizeof(arr); i++) arr[i] = message[i]; 
  pushover(arr, 0);
}
