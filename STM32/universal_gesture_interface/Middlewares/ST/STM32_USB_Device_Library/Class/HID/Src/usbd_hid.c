#include "usbd_hid.h"
#include "usbd_ctlreq.h"

static uint8_t USBD_HID_Init(USBD_HandleTypeDef *pdev, uint8_t cfgidx);
static uint8_t USBD_HID_DeInit(USBD_HandleTypeDef *pdev, uint8_t cfgidx);
static uint8_t USBD_HID_Setup(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);
static uint8_t USBD_HID_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum);
#ifndef USE_USBD_COMPOSITE
static uint8_t *USBD_HID_GetFSCfgDesc(uint16_t *length);
static uint8_t *USBD_HID_GetHSCfgDesc(uint16_t *length);
static uint8_t *USBD_HID_GetOtherSpeedCfgDesc(uint16_t *length);
static uint8_t *USBD_HID_GetDeviceQualifierDesc(uint16_t *length);
#endif

USBD_ClassTypeDef USBD_HID =
{
  USBD_HID_Init,
  USBD_HID_DeInit,
  USBD_HID_Setup,
  NULL,
  NULL,
  USBD_HID_DataIn,
  NULL,
  NULL,
  NULL,
  NULL,
#ifdef USE_USBD_COMPOSITE
  NULL, NULL, NULL, NULL,
#else
  USBD_HID_GetHSCfgDesc,
  USBD_HID_GetFSCfgDesc,
  USBD_HID_GetOtherSpeedCfgDesc,
  USBD_HID_GetDeviceQualifierDesc,
#endif
};

/* -----------------------------------------------------------------------
 * HID Report Descriptor — Mouse (ID 1) + Keyboard (ID 2)
 * Mouse  report: [0x01] [buttons:1] [pad:5] [X:8] [Y:8] [wheel:8]  = 5 bytes
 * Keyboard report: [0x02] [modifier:8] [reserved:8] [key0..5:8each] = 9 bytes
 * ----------------------------------------------------------------------- */
__ALIGN_BEGIN static uint8_t HID_ReportDesc[] __ALIGN_END =
{
    /* ---- Mouse (Report ID 1) ---- */
    0x05, 0x01,         /* Usage Page (Generic Desktop) */
    0x09, 0x02,         /* Usage (Mouse) */
    0xA1, 0x01,         /* Collection (Application) */
    0x85, 0x01,         /*   Report ID (1) */
    0x09, 0x01,         /*   Usage (Pointer) */
    0xA1, 0x00,         /*   Collection (Physical) */

    /* Buttons: 3 bits */
    0x05, 0x09,         /*     Usage Page (Button) */
    0x19, 0x01,         /*     Usage Minimum (1) */
    0x29, 0x03,         /*     Usage Maximum (3) */
    0x15, 0x00,         /*     Logical Minimum (0) */
    0x25, 0x01,         /*     Logical Maximum (1) */
    0x95, 0x03,         /*     Report Count (3) */
    0x75, 0x01,         /*     Report Size (1) */
    0x81, 0x02,         /*     Input (Data, Var, Abs) */

    /* Padding: 5 bits */
    0x95, 0x01,         /*     Report Count (1) */
    0x75, 0x05,         /*     Report Size (5) */
    0x81, 0x01,         /*     Input (Const) */

    /* X, Y, Wheel: 3 signed bytes */
    0x05, 0x01,         /*     Usage Page (Generic Desktop) */
    0x09, 0x30,         /*     Usage (X) */
    0x09, 0x31,         /*     Usage (Y) */
    0x09, 0x38,         /*     Usage (Wheel) */
    0x15, 0x81,         /*     Logical Minimum (-127) */
    0x25, 0x7F,         /*     Logical Maximum (127) */
    0x75, 0x08,         /*     Report Size (8) */
    0x95, 0x03,         /*     Report Count (3) */
    0x81, 0x06,         /*     Input (Data, Var, Rel) */

    0xC0,               /*   End Collection (Physical) */
    0xC0,               /* End Collection (Application) */

    /* ---- Keyboard (Report ID 2) ---- */
    0x05, 0x01,         /* Usage Page (Generic Desktop) */
    0x09, 0x06,         /* Usage (Keyboard) */
    0xA1, 0x01,         /* Collection (Application) */
    0x85, 0x02,         /*   Report ID (2) */

    /* Modifier keys: 8 bits, one per modifier */
    0x05, 0x07,         /*   Usage Page (Keyboard/Keypad) */
    0x19, 0xE0,         /*   Usage Minimum (Left Ctrl  = 0xE0) */
    0x29, 0xE7,         /*   Usage Maximum (Right GUI  = 0xE7) */
    0x15, 0x00,         /*   Logical Minimum (0) */
    0x25, 0x01,         /*   Logical Maximum (1) */
    0x75, 0x01,         /*   Report Size (1) */
    0x95, 0x08,         /*   Report Count (8) */
    0x81, 0x02,         /*   Input (Data, Var, Abs) */

    /* Reserved byte */
    0x75, 0x08,         /*   Report Size (8) */
    0x95, 0x01,         /*   Report Count (1) */
    0x81, 0x01,         /*   Input (Const) */

    /* Keycodes: 6 bytes */
    0x05, 0x07,         /*   Usage Page (Keyboard/Keypad) */
    0x19, 0x00,         /*   Usage Minimum (0) */
    0x29, 0x65,         /*   Usage Maximum (101) */
    0x15, 0x00,         /*   Logical Minimum (0) */
    0x25, 0x65,         /*   Logical Maximum (101) */
    0x75, 0x08,         /*   Report Size (8) */
    0x95, 0x06,         /*   Report Count (6) */
    0x81, 0x00,         /*   Input (Data, Array, Abs) */

    0xC0                /* End Collection (Application) */
};

#define HID_REPORT_DESC_SIZE    sizeof(HID_ReportDesc)

/* -----------------------------------------------------------------------
 * Configuration Descriptor
 * Total = 9 (config) + 9 (interface) + 9 (HID) + 7 (endpoint) = 34 bytes
 * ----------------------------------------------------------------------- */
#ifndef USE_USBD_COMPOSITE
__ALIGN_BEGIN static uint8_t USBD_HID_CfgDesc[USB_HID_CONFIG_DESC_SIZ] __ALIGN_END =
{
  /* Configuration descriptor */
  0x09,                           /* bLength */
  USB_DESC_TYPE_CONFIGURATION,    /* bDescriptorType */
  USB_HID_CONFIG_DESC_SIZ, 0x00,  /* wTotalLength */
  0x01,                           /* bNumInterfaces */
  0x01,                           /* bConfigurationValue */
  0x00,                           /* iConfiguration */
#if (USBD_SELF_POWERED == 1U)
  0xE0,                           /* bmAttributes: self powered */
#else
  0xA0,                           /* bmAttributes: bus powered, remote wakeup */
#endif
  USBD_MAX_POWER,                 /* MaxPower */

  /* Interface descriptor */
  0x09,                           /* bLength */
  USB_DESC_TYPE_INTERFACE,        /* bDescriptorType */
  0x00,                           /* bInterfaceNumber */
  0x00,                           /* bAlternateSetting */
  0x01,                           /* bNumEndpoints */
  0x03,                           /* bInterfaceClass: HID */
  0x00,                           /* bInterfaceSubClass: 0 = no boot */
  0x00,                           /* bInterfaceProtocol: 0 = none */
  0x00,                           /* iInterface */

  /* HID descriptor */
  0x09,                           /* bLength */
  HID_DESCRIPTOR_TYPE,            /* bDescriptorType: HID */
  0x11, 0x01,                     /* bcdHID: HID 1.11 */
  0x00,                           /* bCountryCode */
  0x01,                           /* bNumDescriptors */
  0x22,                           /* bDescriptorType: Report */
  /* wItemLength — patched at runtime in GetFSCfgDesc/GetHSCfgDesc
     because HID_REPORT_DESC_SIZE is computed from sizeof() which the
     linker knows but the initialiser list needs a constant.
     We write a placeholder 0x00,0x00 and fix it up below.           */
  0x00, 0x00,

  /* Endpoint descriptor */
  0x07,                           /* bLength */
  USB_DESC_TYPE_ENDPOINT,         /* bDescriptorType */
  HID_EPIN_ADDR,                  /* bEndpointAddress */
  0x03,                           /* bmAttributes: Interrupt */
  HID_EPIN_SIZE, 0x00,            /* wMaxPacketSize */
  HID_FS_BINTERVAL,               /* bInterval */
};
#endif /* USE_USBD_COMPOSITE */

/* Standalone HID descriptor (used in GET_DESCRIPTOR for HID type) */
__ALIGN_BEGIN static uint8_t USBD_HID_Desc[USB_HID_DESC_SIZ] __ALIGN_END =
{
  0x09,
  HID_DESCRIPTOR_TYPE,
  0x11, 0x01,
  0x00,
  0x01,
  0x22,
  0x00, 0x00,   /* wItemLength — patched at runtime */
};

#ifndef USE_USBD_COMPOSITE
__ALIGN_BEGIN static uint8_t USBD_HID_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC] __ALIGN_END =
{
  USB_LEN_DEV_QUALIFIER_DESC,
  USB_DESC_TYPE_DEVICE_QUALIFIER,
  0x00, 0x02,
  0x00, 0x00, 0x00,
  0x40,
  0x01,
  0x00,
};
#endif

static uint8_t HIDInEpAdd = HID_EPIN_ADDR;

/* Patch wItemLength in both descriptors at runtime */
static void HID_PatchReportDescSize(void)
{
  uint16_t sz = (uint16_t)HID_REPORT_DESC_SIZE;
#ifndef USE_USBD_COMPOSITE
  USBD_HID_CfgDesc[25] = (uint8_t)(sz & 0xFF);
  USBD_HID_CfgDesc[26] = (uint8_t)(sz >> 8);
#endif
  USBD_HID_Desc[7] = (uint8_t)(sz & 0xFF);
  USBD_HID_Desc[8] = (uint8_t)(sz >> 8);
}

/* -----------------------------------------------------------------------
 * Init / DeInit
 * ----------------------------------------------------------------------- */
static uint8_t USBD_HID_Init(USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
  UNUSED(cfgidx);

  HID_PatchReportDescSize();

  USBD_HID_HandleTypeDef *hhid =
    (USBD_HID_HandleTypeDef *)USBD_malloc(sizeof(USBD_HID_HandleTypeDef));

  if (hhid == NULL)
  {
    pdev->pClassDataCmsit[pdev->classId] = NULL;
    return (uint8_t)USBD_EMEM;
  }

  pdev->pClassDataCmsit[pdev->classId] = (void *)hhid;
  pdev->pClassData = pdev->pClassDataCmsit[pdev->classId];

#ifdef USE_USBD_COMPOSITE
  HIDInEpAdd = USBD_CoreGetEPAdd(pdev, USBD_EP_IN, USBD_EP_TYPE_INTR, (uint8_t)pdev->classId);
#endif

  pdev->ep_in[HIDInEpAdd & 0xFU].bInterval =
    (pdev->dev_speed == USBD_SPEED_HIGH) ? HID_HS_BINTERVAL : HID_FS_BINTERVAL;

  (void)USBD_LL_OpenEP(pdev, HIDInEpAdd, USBD_EP_TYPE_INTR, HID_EPIN_SIZE);
  pdev->ep_in[HIDInEpAdd & 0xFU].is_used = 1U;

  hhid->state = USBD_HID_IDLE;

  return (uint8_t)USBD_OK;
}

static uint8_t USBD_HID_DeInit(USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
  UNUSED(cfgidx);

#ifdef USE_USBD_COMPOSITE
  HIDInEpAdd = USBD_CoreGetEPAdd(pdev, USBD_EP_IN, USBD_EP_TYPE_INTR, (uint8_t)pdev->classId);
#endif

  (void)USBD_LL_CloseEP(pdev, HIDInEpAdd);
  pdev->ep_in[HIDInEpAdd & 0xFU].is_used = 0U;
  pdev->ep_in[HIDInEpAdd & 0xFU].bInterval = 0U;

  if (pdev->pClassDataCmsit[pdev->classId] != NULL)
  {
    (void)USBD_free(pdev->pClassDataCmsit[pdev->classId]);
    pdev->pClassDataCmsit[pdev->classId] = NULL;
  }

  return (uint8_t)USBD_OK;
}

/* -----------------------------------------------------------------------
 * Setup — handles GET_DESCRIPTOR for both Report and HID descriptors
 * ----------------------------------------------------------------------- */
static uint8_t USBD_HID_Setup(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req)
{
  USBD_HID_HandleTypeDef *hhid =
    (USBD_HID_HandleTypeDef *)pdev->pClassDataCmsit[pdev->classId];

  USBD_StatusTypeDef ret = USBD_OK;
  uint16_t len;
  uint8_t *pbuf;
  uint16_t status_info = 0U;

  if (hhid == NULL) return (uint8_t)USBD_FAIL;

  switch (req->bmRequest & USB_REQ_TYPE_MASK)
  {
    case USB_REQ_TYPE_CLASS:
      switch (req->bRequest)
      {
        case USBD_HID_REQ_SET_PROTOCOL:
          hhid->Protocol  = (uint8_t)(req->wValue);       break;
        case USBD_HID_REQ_GET_PROTOCOL:
          (void)USBD_CtlSendData(pdev, (uint8_t *)&hhid->Protocol, 1U); break;
        case USBD_HID_REQ_SET_IDLE:
          hhid->IdleState = (uint8_t)(req->wValue >> 8);  break;
        case USBD_HID_REQ_GET_IDLE:
          (void)USBD_CtlSendData(pdev, (uint8_t *)&hhid->IdleState, 1U); break;
        default:
          USBD_CtlError(pdev, req);
          ret = USBD_FAIL;
          break;
      }
      break;

    case USB_REQ_TYPE_STANDARD:
      switch (req->bRequest)
      {
        case USB_REQ_GET_STATUS:
          if (pdev->dev_state == USBD_STATE_CONFIGURED)
            (void)USBD_CtlSendData(pdev, (uint8_t *)&status_info, 2U);
          else { USBD_CtlError(pdev, req); ret = USBD_FAIL; }
          break;

        case USB_REQ_GET_DESCRIPTOR:
          if ((req->wValue >> 8) == HID_REPORT_DESC)
          {
            pbuf = HID_ReportDesc;
            len  = MIN((uint16_t)HID_REPORT_DESC_SIZE, req->wLength);
          }
          else if ((req->wValue >> 8) == HID_DESCRIPTOR_TYPE)
          {
            pbuf = USBD_HID_Desc;
            len  = MIN(USB_HID_DESC_SIZ, req->wLength);
          }
          else { USBD_CtlError(pdev, req); ret = USBD_FAIL; break; }
          (void)USBD_CtlSendData(pdev, pbuf, len);
          break;

        case USB_REQ_GET_INTERFACE:
          if (pdev->dev_state == USBD_STATE_CONFIGURED)
            (void)USBD_CtlSendData(pdev, (uint8_t *)&hhid->AltSetting, 1U);
          else { USBD_CtlError(pdev, req); ret = USBD_FAIL; }
          break;

        case USB_REQ_SET_INTERFACE:
          if (pdev->dev_state == USBD_STATE_CONFIGURED)
            hhid->AltSetting = (uint8_t)(req->wValue);
          else { USBD_CtlError(pdev, req); ret = USBD_FAIL; }
          break;

        case USB_REQ_CLEAR_FEATURE:
          break;

        default:
          USBD_CtlError(pdev, req); ret = USBD_FAIL; break;
      }
      break;

    default:
      USBD_CtlError(pdev, req); ret = USBD_FAIL; break;
  }

  return (uint8_t)ret;
}

/* -----------------------------------------------------------------------
 * SendReport — call only when USBD_HID_IsReady() returns 1
 * ----------------------------------------------------------------------- */
#ifdef USE_USBD_COMPOSITE
uint8_t USBD_HID_SendReport(USBD_HandleTypeDef *pdev, uint8_t *report, uint16_t len, uint8_t ClassId)
{
  USBD_HID_HandleTypeDef *hhid = (USBD_HID_HandleTypeDef *)pdev->pClassDataCmsit[ClassId];
  HIDInEpAdd = USBD_CoreGetEPAdd(pdev, USBD_EP_IN, USBD_EP_TYPE_INTR, ClassId);
#else
uint8_t USBD_HID_SendReport(USBD_HandleTypeDef *pdev, uint8_t *report, uint16_t len)
{
  USBD_HID_HandleTypeDef *hhid = (USBD_HID_HandleTypeDef *)pdev->pClassDataCmsit[pdev->classId];
#endif

  if (hhid == NULL) return (uint8_t)USBD_FAIL;

  if (pdev->dev_state == USBD_STATE_CONFIGURED)
  {
    if (hhid->state == USBD_HID_IDLE)
    {
      hhid->state = USBD_HID_BUSY;
      (void)USBD_LL_Transmit(pdev, HIDInEpAdd, report, len);
      return (uint8_t)USBD_OK;
    }
    return (uint8_t)USBD_BUSY;   /* caller can detect and retry */
  }

  return (uint8_t)USBD_FAIL;
}

/* -----------------------------------------------------------------------
 * IsReady — non-blocking check before calling SendReport
 * ----------------------------------------------------------------------- */
uint8_t USBD_HID_IsReady(USBD_HandleTypeDef *pdev)
{
  USBD_HID_HandleTypeDef *hhid =
    (USBD_HID_HandleTypeDef *)pdev->pClassDataCmsit[pdev->classId];

  if (hhid == NULL) return 0U;
  if (pdev->dev_state != USBD_STATE_CONFIGURED) return 0U;
  return (hhid->state == USBD_HID_IDLE) ? 1U : 0U;
}

/* -----------------------------------------------------------------------
 * DataIn — endpoint finished transmitting, mark idle
 * ----------------------------------------------------------------------- */
static uint8_t USBD_HID_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
  UNUSED(epnum);
  ((USBD_HID_HandleTypeDef *)pdev->pClassDataCmsit[pdev->classId])->state = USBD_HID_IDLE;
  return (uint8_t)USBD_OK;
}

/* -----------------------------------------------------------------------
 * Config descriptor helpers
 * ----------------------------------------------------------------------- */
#ifndef USE_USBD_COMPOSITE
static uint8_t *USBD_HID_GetFSCfgDesc(uint16_t *length)
{
  HID_PatchReportDescSize();
  USBD_EpDescTypeDef *pEpDesc = USBD_GetEpDesc(USBD_HID_CfgDesc, HID_EPIN_ADDR);
  if (pEpDesc != NULL) pEpDesc->bInterval = HID_FS_BINTERVAL;
  *length = (uint16_t)sizeof(USBD_HID_CfgDesc);
  return USBD_HID_CfgDesc;
}

static uint8_t *USBD_HID_GetHSCfgDesc(uint16_t *length)
{
  HID_PatchReportDescSize();
  USBD_EpDescTypeDef *pEpDesc = USBD_GetEpDesc(USBD_HID_CfgDesc, HID_EPIN_ADDR);
  if (pEpDesc != NULL) pEpDesc->bInterval = HID_HS_BINTERVAL;
  *length = (uint16_t)sizeof(USBD_HID_CfgDesc);
  return USBD_HID_CfgDesc;
}

static uint8_t *USBD_HID_GetOtherSpeedCfgDesc(uint16_t *length)
{
  HID_PatchReportDescSize();
  USBD_EpDescTypeDef *pEpDesc = USBD_GetEpDesc(USBD_HID_CfgDesc, HID_EPIN_ADDR);
  if (pEpDesc != NULL) pEpDesc->bInterval = HID_FS_BINTERVAL;
  *length = (uint16_t)sizeof(USBD_HID_CfgDesc);
  return USBD_HID_CfgDesc;
}

static uint8_t *USBD_HID_GetDeviceQualifierDesc(uint16_t *length)
{
  *length = (uint16_t)sizeof(USBD_HID_DeviceQualifierDesc);
  return USBD_HID_DeviceQualifierDesc;
}
#endif /* USE_USBD_COMPOSITE */

uint32_t USBD_HID_GetPollingInterval(USBD_HandleTypeDef *pdev)
{
  if (pdev->dev_speed == USBD_SPEED_HIGH)
    return (((1U << (HID_HS_BINTERVAL - 1U))) / 8U);
  return HID_FS_BINTERVAL;
}
