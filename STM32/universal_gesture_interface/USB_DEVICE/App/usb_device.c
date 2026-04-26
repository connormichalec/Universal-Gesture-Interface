#include "usb_device.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_hid.h"

USBD_HandleTypeDef hUsbDeviceFS;

void MX_USB_DEVICE_Init(void)
{
  if (USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS) != USBD_OK)
    Error_Handler();

  if (USBD_RegisterClass(&hUsbDeviceFS, &USBD_HID) != USBD_OK)
    Error_Handler();

  if (USBD_Start(&hUsbDeviceFS) != USBD_OK)
    Error_Handler();
}
