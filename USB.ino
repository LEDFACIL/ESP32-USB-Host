#include <ESP32-USB-Soft-Host.h>

#define DP_P0  16  // Pin "RX2" en el ESP32 DEVKIT V1, conectar al pin D+ del conector USB
#define DM_P0  17  // Pin "TX2" en el ESP32 DEVKIT V1, conectar al pin D- del conector USB
#define LED    2   // LED azul integrado en el ESP32 DEVKIT V1

usb_pins_config_t USB_Pins_Config =
{
  DP_P0, DM_P0,
  -1, -1,
  -1, -1,
  -1, -1
};

static void my_USB_DetectCB(uint8_t usbNum, void * dev)
{
  sDevDesc *device = (sDevDesc*)dev;
  Serial.printf("New device detected on USB#%d\n", usbNum);
  Serial.printf("Vendor ID: 0x%04x, Product ID: 0x%04x\n", device->idVendor, device->idProduct);
}

/*static void my_USB_PrintCB(uint8_t usbNum, uint8_t byte_depth, uint8_t* data, uint8_t data_len)
{
  Serial.print("Received data: ");
  for(int i = 0; i < data_len; i++)
  {
    Serial.printf("0x%02x ", data[i]);
  }
  Serial.println();
}*/

static void my_USB_PrintCB(uint8_t usbNum, uint8_t byte_depth, uint8_t* data, uint8_t data_len)
{
  digitalWrite(LED, 1);
  Serial.print("Received data: ");
  for(int i = 0; i < data_len; i++)
  {
    for(int bit = 7; bit >= 0; bit--)
    {
      Serial.print((data[i] >> bit) & 0x01);
    }
    Serial.print(" ");
  }
  Serial.println();
  digitalWrite(LED, 0);
}

void setup()
{
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
  delay(5000);
  Serial.println("Initializing USB Host");

  USH.setOnConfigDescCB(Default_USB_ConfigDescCB);
  USH.setOnIfaceDescCb(Default_USB_IfaceDescCb);
  USH.setOnHIDDevDescCb(Default_USB_HIDDevDescCb);
  USH.setOnEPDescCb(Default_USB_EPDescCb);

  USH.init(USB_Pins_Config, my_USB_DetectCB, my_USB_PrintCB);
}

void loop()
{
  // Nada que hacer en el loop principal - las tareas principales se realizan en segundo plano, en el núcleo #1 del ESP
}