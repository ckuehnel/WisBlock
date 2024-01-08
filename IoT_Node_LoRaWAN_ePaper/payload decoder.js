function decodeUplink(input) 
{
  var value = (input.bytes[0]<<8 | input.bytes[1]);
  var core = "RAK" + String(value);  // RAK Core Modul
  //var core = value;  // RAK Core Modul
   
  value = input.bytes[2]<<8 | input.bytes[3];
  var temp = (value <<24 >>24)/10;   // signed Temperature, units:°C
  
  value = input.bytes[4]<<8 | input.bytes[5];
  var humi = value;   				// Humidity, units: %rH

  value = input.bytes[6]<<8 | input.bytes[7];
  var vbat = value/1000;   			// Battery voltage, units: V
  
  return 
  {
    data: 
	{
      Core: core,
      Temperature: temp,
      Humidity: humi,
      Vbat: vbat
    },
    warnings: [],
    errors: []
  };
}