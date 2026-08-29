
#ifndef CFG_TUSB_CONFIG_H_
#define CFG_TUSB_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "tusb_option.h"

#define BOARD_TUD_RHPORT        0
#define CFG_TUSB_MCU             OPT_MCU_RP2040
#define CFG_TUSB_RHPORT0_MODE    (OPT_MODE_DEVICE)

#define CFG_TUD_ENDPOINT0_SIZE   64

#define CFG_TUD_CDC              1

#define CFG_TUD_HID              1

#define CFG_TUD_MSC              0
#define CFG_TUD_MIDI             0
#define CFG_TUD_VENDOR           0
#define CFG_TUD_AUDIO            0
#define CFG_TUD_VIDEO            0
#define CFG_TUD_BTH              0

#define CFG_TUD_CDC_RX_BUFSIZE   256
#define CFG_TUD_CDC_TX_BUFSIZE   256

#define CFG_TUD_HID_EP_BUFSIZE   64


#define CFG_TUSB_MEM_SECTION

#define CFG_TUSB_MEM_ALIGN       __attribute__((aligned(4)))


#define CFG_TUSB_DEBUG           0

#ifdef __cplusplus
}
#endif

#endif 