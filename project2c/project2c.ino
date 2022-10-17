#include <ArduinoBLE.h>
#include <Arduino_LSM6DS3.h>

#define BLE_UUID_ACCELEROMETER_SERVICE "1101"
#define BLE_UUID_ACCELEROMETER_X "2101"
#define BLE_UUID_ACCELEROMETER_Y "2102"
#define BLE_UUID_ACCELEROMETER_Z "2103"

#define BLE_DEVICE_NAME "Elfo"
#define BLE_LOCAL_NAME "Elfo"

BLEService accelerometerService(BLE_UUID_ACCELEROMETER_SERVICE);
//BLEByteCharacteristic switchCharacteristic("2A57", BLERead | BLEWrite);
BLEFloatCharacteristic accelerometerCharacteristicX(BLE_UUID_ACCELEROMETER_X, BLERead | BLENotify);
BLEFloatCharacteristic accelerometerCharacteristicY(BLE_UUID_ACCELEROMETER_Y, BLERead | BLENotify);
BLEFloatCharacteristic accelerometerCharacteristicZ(BLE_UUID_ACCELEROMETER_Z, BLERead | BLENotify);

float ax, ay, az;
// float gx, gy, gz;

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;

  // initialize IMU
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1)
      ;
  }

  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println("Hz");

  // initialize BLE
  if (!BLE.begin()) {
    Serial.println("Starting Bluetooth® Low Energy module failed!");
    while (1)
      ;
  }

  // set advertised local name and service UUID
  //
  // <add your code here>
  //
  BLE.setLocalName("Nano 33 IoT");
  BLE.setAdvertisedService(accelerometerService);

  // add characteristics and service
  //
  // <add your code here>
  //
  accelerometerService.addCharacteristic(accelerometerCharacteristicX);
  accelerometerService.addCharacteristic(accelerometerCharacteristicY);
  accelerometerService.addCharacteristic(accelerometerCharacteristicZ);

  BLE.addService(accelerometerService);
  // start advertising
  //
  // <add your code here>
  //
  BLE.advertise();
  Serial.println("BLE Accelerometer Peripheral");
}

void loop() {
  BLEDevice central = BLE.central();

  // obtain and write accelerometer data
  // 
  // <add your code here>
  // 
  if (central && IMU.accelerationAvailable() && IMU.gyroscopeAvailable()) 
  {
    IMU.readAcceleration(ax, ay, az);
    // IMU.readGyroscope(gx, gy, gz);
    while(central.connected()){
      accelerometerCharacteristicX.writeValue(ax);
      accelerometerCharacteristicY.writeValue(ay);
      accelerometerCharacteristicZ.writeValue(az);
      delay(100);
    }
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
  
}