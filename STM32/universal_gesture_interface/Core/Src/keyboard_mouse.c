/*
 * keyboard_mouse.c
 *
 *  Created on: Apr 28, 2026
 *      Author: kobes
 */

#include "keyboard_mouse.h"

static KB_State   kb_state      = KB_IDLE;
static uint32_t   kb_timer      = 0;
static uint8_t    kb_mod        = 0;
static uint8_t    kb_key        = 0;
static uint8_t    kb_pending    = 0;

// to prevent blocking:
/* Call this every iteration of while(1) */
void HID_Task(void)
{
  switch (kb_state)
  {
    case KB_IDLE:
      if (kb_pending && USBD_HID_IsReady(&hUsbDeviceFS))
      {
        uint8_t press[9] = {0x02, kb_mod, 0x00,
                            kb_key, 0x00, 0x00, 0x00, 0x00, 0x00};
        if (USBD_HID_SendReport(&hUsbDeviceFS, press, 9) == USBD_OK)
        {
          kb_pending = 0;
          kb_timer   = HAL_GetTick();
          kb_state   = KB_PRESS_SENT;
        }
      }
      break;

    case KB_PRESS_SENT:
      if (USBD_HID_IsReady(&hUsbDeviceFS) &&
          (HAL_GetTick() - kb_timer) >= HOLD_TIME)
      {
        uint8_t release[9] = {0x02, 0x00, 0x00,
                              0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        if (USBD_HID_SendReport(&hUsbDeviceFS, release, 9) == USBD_OK)
        {
          kb_timer = HAL_GetTick();
          kb_state = KB_RELEASE_SENT;
        }
      }
      break;

    case KB_RELEASE_SENT:
      /* Wait for release to finish before allowing next keypress */
      if (USBD_HID_IsReady(&hUsbDeviceFS) &&
          (HAL_GetTick() - kb_timer) >= HOLD_TIME)
      {
        kb_state = KB_IDLE;
        kb_mod = 0;
      }
      break;
  }
}

//AUTOMATED:
uint8_t HID_SendKey(uint8_t modifier, uint8_t keycode)
{
  if (kb_state != KB_IDLE) return 0;
  kb_mod     = modifier;
  kb_key     = keycode;
  kb_pending = 1;
  return 1;
}


//FOR MANUAL CONTROL (deal with blocking ~keypresses take about 20ms before release.
uint8_t HID_KeyPress(uint8_t modifier, uint8_t keycode)
{
    if (!USBD_HID_IsReady(&hUsbDeviceFS)) return (uint8_t)USBD_BUSY;
    uint8_t report[9] = {KEYBOARD_PREFIX, modifier, 0x00,
                         keycode, 0x00, 0x00, 0x00, 0x00, 0x00};
    return USBD_HID_SendReport(&hUsbDeviceFS, report, 9);
}

uint8_t HID_KeyRelease(void)
{
    if (!USBD_HID_IsReady(&hUsbDeviceFS)) return (uint8_t)USBD_BUSY;
    uint8_t report[9] = {KEYBOARD_PREFIX, 0x00, 0x00,
                         0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    return USBD_HID_SendReport(&hUsbDeviceFS, report, 9);
}




uint8_t HID_SendMouse(uint8_t buttons, int8_t x, int8_t y, int8_t wheel)
{
  if (!USBD_HID_IsReady(&hUsbDeviceFS)) return (uint8_t)USBD_BUSY;
  uint8_t report[5] = {MOUSE_PREFIX, buttons, (uint8_t)x, (uint8_t)y, (uint8_t)wheel};
  return USBD_HID_SendReport(&hUsbDeviceFS, report, 5);
}




uint8_t USBD_HID_SendRaw(USBD_HandleTypeDef *pdev, uint8_t *data, uint8_t len)
{
  static uint8_t report[HID_RAW_REPORT_SIZE];
  memset(report, 0, sizeof(report));
  report[0] = HID_REPORT_ID_RAW;
  memcpy(&report[1], data, (len > 63U) ? 63U : len);
  return USBD_HID_SendReport(pdev, report, HID_RAW_REPORT_SIZE);
}
