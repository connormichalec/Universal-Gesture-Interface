#ifndef __USB_HID_H
#define __USB_HID_H

#ifdef __cplusplus
extern "C" {
#endif

#include "usbd_ioreq.h"

/* Endpoint */
#ifndef HID_EPIN_ADDR
#define HID_EPIN_ADDR                   0x81U
#endif

#define HID_EPIN_SIZE                   0x09U   /* Max report size = keyboard = 9 bytes */

/* Descriptor sizes */
#define USB_HID_CONFIG_DESC_SIZ         34U
#define USB_HID_DESC_SIZ                9U

/* Descriptor types */
#define HID_DESCRIPTOR_TYPE             0x21U
#define HID_REPORT_DESC                 0x22U

/* Polling intervals */
#ifndef HID_HS_BINTERVAL
#define HID_HS_BINTERVAL                0x07U
#endif
#ifndef HID_FS_BINTERVAL
#define HID_FS_BINTERVAL                0x01U   /* 1ms — fastest allowed, reduces latency */
#endif

/* HID class requests */
#define USBD_HID_REQ_SET_PROTOCOL       0x0BU
#define USBD_HID_REQ_GET_PROTOCOL       0x03U
#define USBD_HID_REQ_SET_IDLE           0x0AU
#define USBD_HID_REQ_GET_IDLE           0x02U
#define USBD_HID_REQ_SET_REPORT         0x09U
#define USBD_HID_REQ_GET_REPORT         0x01U

/* Report IDs */
#define HID_REPORT_ID_MOUSE             0x01U
#define HID_REPORT_ID_KEYBOARD          0x02U

/* Report sizes (including Report ID byte) */
#define HID_MOUSE_REPORT_SIZE           5U      /* ID + buttons + X + Y + wheel */
#define HID_KEYBOARD_REPORT_SIZE        9U      /* ID + modifier + reserved + 6 keycodes */

typedef enum
{
  USBD_HID_IDLE = 0,
  USBD_HID_BUSY,
} USBD_HID_StateTypeDef;

typedef struct
{
  uint32_t              Protocol;
  uint32_t              IdleState;
  uint32_t              AltSetting;
  USBD_HID_StateTypeDef state;
} USBD_HID_HandleTypeDef;

extern USBD_ClassTypeDef USBD_HID;
#define USBD_HID_CLASS &USBD_HID

#ifdef USE_USBD_COMPOSITE
uint8_t USBD_HID_SendReport(USBD_HandleTypeDef *pdev, uint8_t *report, uint16_t len, uint8_t ClassId);
#else
uint8_t USBD_HID_SendReport(USBD_HandleTypeDef *pdev, uint8_t *report, uint16_t len);
#endif

uint8_t USBD_HID_IsReady(USBD_HandleTypeDef *pdev);
uint32_t USBD_HID_GetPollingInterval(USBD_HandleTypeDef *pdev);

#ifdef __cplusplus
}
#endif

#endif /* __USB_HID_H */
