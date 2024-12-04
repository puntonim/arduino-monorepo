///////////////////ADAFRUIT

#include <Adafruit_MLX90614.h>
 
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
 
void setup() {
  Serial.begin(57600);
  while (!Serial) delay(100);
 
  Serial.println("Adafruit MLX90614 test");
 
  while (!mlx.begin()) {
    Serial.println("Error connecting to MLX sensor, check wiring!");
    delay(2000);
  };
 
  Serial.print("Emissivity = ");
  Serial.println(mlx.readEmissivity());
}
 
void loop() {
  Serial.print("T1 = ");
  Serial.println(mlx.readAmbientTempC());
  Serial.print("T2 = ");
  Serial.println(mlx.readObjectTempC());
  Serial.println();
  delay(2000);
}


///////////////// DFROBOT
// #include <DFRobot_MLX90614.h>

// DFRobot_MLX90614_I2C sensor;   // instantiate an object to drive our sensor
// // DFRobot_MLX90614_I2C sensor(/*uint8_t i2cAddr=*/0x5A, /*TwoWire *pWire = */&Wire, /*int sdaPin=*/SDA, /*int sclPin=*/SCL);

// void setup()
// {
//   Serial.begin(57600);
//   while (!Serial) delay(100);

//   while( NO_ERR != sensor.begin() ){
//     Serial.println("Communication with device failed, please check connection");
//     delay(3000);
//   }
//   Serial.println("Begin ok!");
//   sensor.enterSleepMode();
//   delay(50);
//   sensor.enterSleepMode(false);
//   delay(200);
// }

// void loop()
// {
//   float ambientTemp = sensor.getAmbientTempCelsius();
//   float objectTemp = sensor.getObjectTempCelsius();
//   Serial.print("Ambient celsius : "); Serial.print(ambientTemp); Serial.println(" °C");
//   Serial.print("Object celsius : ");  Serial.print(objectTemp);  Serial.println(" °C");
//   delay(500);
// }
