
#include <string.h>
#include <stdio.h>
#include "bsp/board_api.h"
#include "tusb.h"
#include "usb_descriptors.h"

#define _PID_MAP(itf, n)  ( (CFG_TUD_##itf) << (n) )

#define USB_VID   0xCafe
#define USB_PID   (0x4000 | \
                   _PID_MAP(CDC,  0) | \
                   _PID_MAP(MSC,  1) | \
                   _PID_MAP(HID,  2) | \
                   _PID_MAP(MIDI, 3) | \
                   _PID_MAP(VENDOR, 4))

#define USB_BCD   0x0200

tusb_desc_device_t const desc_device =
{
    .bLength            = sizeof(tusb_desc_device_t),
    .bDescriptorType    = TUSB_DESC_DEVICE,
    .bcdUSB             = USB_BCD,
    .bDeviceClass       = 0x00,
    .bDeviceSubClass    = 0x00,
    .bDeviceProtocol    = 0x00,

    .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,

    .idVendor           = USB_VID,
    .idProduct          = USB_PID,
    .bcdDevice          = 0x0100,

    .iManufacturer      = 0x01,
    .iProduct           = 0x02,
    .iSerialNumber      = 0x03,

    .bNumConfigurations = 0x01
};

uint8_t const * tud_descriptor_device_cb(void)
{
    return (uint8_t const *) &desc_device;
}

uint8_t const desc_hid_report[] =
{
    TUD_HID_REPORT_DESC_KEYBOARD(
        HID_REPORT_ID(REPORT_ID_KEYBOARD)
    ),

    TUD_HID_REPORT_DESC_MOUSE(
        HID_REPORT_ID(REPORT_ID_MOUSE)
    ),

    TUD_HID_REPORT_DESC_CONSUMER(
        HID_REPORT_ID(REPORT_ID_CONSUMER_CONTROL)
    ),

    TUD_HID_REPORT_DESC_GAMEPAD(
        HID_REPORT_ID(REPORT_ID_GAMEPAD)
    )
};

uint8_t const * tud_hid_descriptor_report_cb(uint8_t instance)
{
    (void) instance;

    return desc_hid_report;
}

enum
{
    ITF_NUM_CDC = 0,
    ITF_NUM_CDC_DATA,
    ITF_NUM_HID,

   
    ITF_NUM_TOTAL
};


#define EPNUM_CDC_NOTIF   0x81
#define EPNUM_CDC_OUT     0x02
#define EPNUM_CDC_IN      0x82

#define EPNUM_HID         0x83

#define CONFIG_TOTAL_LEN  \
    (TUD_CONFIG_DESC_LEN + \
     TUD_CDC_DESC_LEN +   \
     TUD_HID_DESC_LEN)

uint8_t const desc_configuration[] =
{
    
    TUD_CONFIG_DESCRIPTOR(
        1,
        ITF_NUM_TOTAL,
        0,
        CONFIG_TOTAL_LEN,
        0x80,
        100
    ),

    TUD_CDC_DESCRIPTOR(
        ITF_NUM_CDC,
        4,                      
        EPNUM_CDC_NOTIF,
        8,                     
        EPNUM_CDC_OUT,
        EPNUM_CDC_IN,
        64                     
    ),

    TUD_HID_DESCRIPTOR(
        ITF_NUM_HID,
        0,                   
        HID_ITF_PROTOCOL_NONE,
        sizeof(desc_hid_report),
        EPNUM_HID,
        CFG_TUD_HID_EP_BUFSIZE,
        5                       
    )
};

uint8_t const * tud_descriptor_configuration_cb(uint8_t index)
{
    (void) index;

    return desc_configuration;
}

enum
{
    STRID_LANGID = 0,
    STRID_MANUFACTURER,
    STRID_PRODUCT,
    STRID_SERIAL,
    STRID_CDC
};

char const *string_desc_arr[] =
{
    // Language ID: English
    (const char[]) { 0x09, 0x04 },

    "PicoLink",
    "PicoLink CDC HID",
    NULL,
    "PicoLink CDC"
};

static uint16_t _desc_str[32 + 1];

uint16_t const * tud_descriptor_string_cb(
    uint8_t index,
    uint16_t langid
)
{
    (void) langid;

    uint16_t chr_count;

    switch (index)
    {

        case STRID_LANGID:

            memcpy(
                &_desc_str[1],
                string_desc_arr[STRID_LANGID],
                2
            );

            chr_count = 1;
            break;
        case STRID_SERIAL:

            chr_count = board_usb_get_serial(
                _desc_str + 1,
                32
            );

            break;
        default:
        {
            if (index >=
                sizeof(string_desc_arr) /
                sizeof(string_desc_arr[0]))
            {
                return NULL;
            }

            char const *str = string_desc_arr[index];

            if (str == NULL)
            {
                return NULL;
            }

            size_t char_count = strlen(str);

            size_t max_count =
                sizeof(_desc_str) /
                sizeof(_desc_str[0]) - 1;

            if (char_count > max_count)
            {
                char_count = max_count;
            }
            for (size_t i = 0; i < char_count; i++)
            {
                _desc_str[1 + i] = str[i];
            }

            chr_count = char_count;
        }

        break;
    }

    _desc_str[0] =
        (uint16_t)
        (
            (TUSB_DESC_STRING << 8) |
            (chr_count * 2 + 2)
        );

    return _desc_str;
}