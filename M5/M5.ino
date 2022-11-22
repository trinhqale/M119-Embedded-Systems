#include <ArduinoBLE.h>
#include <Arduino_LSM6DS3.h>

#define BLE_UUID_ACCELEROMETER_SERVICE "1101"
#define SIGNAL_UUID "2101"
// #define BLE_UUID_ACCELEROMETER_Y "2102"
// #define BLE_UUID_ACCELEROMETER_Z "2103"

#define BLE_DEVICE_NAME "Elfo"
#define BLE_LOCAL_NAME "Elfo"

BLEService accelerometerService(BLE_UUID_ACCELEROMETER_SERVICE);
// //BLEByteCharacteristic switchCharacteristic("2A57", BLERead | BLEWrite);
// BLEFloatCharacteristic accelerometerCharacteristicX(BLE_UUID_ACCELEROMETER_X, BLERead | BLENotify);
// BLEFloatCharacteristic accelerometerCharacteristicY(BLE_UUID_ACCELEROMETER_Y, BLERead | BLENotify);
// BLEFloatCharacteristic accelerometerCharacteristicZ(BLE_UUID_ACCELEROMETER_Z, BLERead | BLENotify);
BLEIntCharacteristic moveSignal(SIGNAL_UUID, BLERead | BLENotify);
float x, y, z;
int degreesX = 0;
int degreesY = 0;
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
  BLE.setLocalName("Trinh's Nano 33 IoT");
  BLE.setAdvertisedService(accelerometerService);

  // add characteristics and service
  //
  // <add your code here>
  // //
  accelerometerService.addCharacteristic(moveSignal);
  // accelerometerService.addCharacteristic(accelerometerCharacteristicY);
  // accelerometerService.addCharacteristic(accelerometerCharacteristicZ);

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
  if (central && IMU.accelerationAvailable() && IMU.gyroscopeAvailable()) {

    while (central.connected()) {
      IMU.readAcceleration(x, y, z);
      // IMU.readGyroscope(gx, gy, gz);
      // Serial.print("ax: ");
      // Serial.print(ax);
      // Serial.print(", ay: ");
      // Serial.print(ay);
      // Serial.print(", az: ");
      // Serial.print(az);
      // accelerometerCharacteristicX.writeValue(ax);
      // accelerometerCharacteristicY.writeValue(ay);
      // accelerometerCharacteristicZ.writeValue(az);
      if (x > 0.1 && y < -0.1) {
        x = 100 * x;
        degreesX = map(x, 0, 97, 0, 90);
        y = 100 * y;
        degreesY = map(y, 0, -100, 0, 90);
        if (degreesX > 30 && degreesY > 30) {
          Serial.println("UP"); // UP IS 1
          moveSignal.writeValue(1);
        }
      }
      else if (x < -0.1 && y > 0.1) {
        x = 100 * x;
        degreesX = map(x, 0, -100, 0, 90);
        y = 100 * y;
        degreesY = map(y, 0, 97, 0, 90);
        if (degreesX > 30 && degreesY > 30) {
          Serial.println("DOWN");
          moveSignal.writeValue(2);
        }
      }
      else
      {
        moveSignal.writeValue(0);
      }
      // delay(1000);
    }
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
}