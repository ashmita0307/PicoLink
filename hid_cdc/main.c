#include <stdio.h>
#include <string.h>

#include "bsp/board_api.h"
#include "tusb.h"
#include "usb_descriptors.h"

enum
{
    BLINK_NOT_MOUNTED = 250,
    BLINK_MOUNTED     = 1000,
    BLINK_SUSPENDED   = 2500,
};

static uint32_t blink_interval_ms = BLINK_NOT_MOUNTED;

static const uint8_t picolink[] =
{
    HID_KEY_P,
    HID_KEY_I,
    HID_KEY_C,
    HID_KEY_O,
    HID_KEY_L,
    HID_KEY_I,
    HID_KEY_N,
    HID_KEY_K
};

static uint8_t picolink_index = 0;

static bool button_was_pressed = false;
static bool key_pressed = false;

void led_blinking_task(void);
void hid_task(void);

int main(void)
{
    board_init();

    const tusb_rhport_init_t rh_init =
    {
        .role  = TUSB_ROLE_DEVICE,
        .speed = TUSB_SPEED_FULL
    };

    TU_ASSERT(tud_rhport_init(BOARD_TUD_RHPORT, &rh_init));

    board_init_after_tusb();

    while (1)
    {
        tud_task();

      
        if (tud_cdc_available())
        {
            char buf[64];

            uint32_t count = tud_cdc_read(buf, sizeof(buf));

            if (count > 0)
            {
                tud_cdc_write(buf, count);
                tud_cdc_write_flush();
            }
        }

        hid_task();
        led_blinking_task();
    }

    return 0;
}


void tud_mount_cb(void)
{
    blink_interval_ms = BLINK_MOUNTED;
}

void tud_umount_cb(void)
{
    blink_interval_ms = BLINK_NOT_MOUNTED;
}

void tud_suspend_cb(bool remote_wakeup_en)
{
    (void) remote_wakeup_en;

    blink_interval_ms = BLINK_SUSPENDED;
}

void tud_resume_cb(void)
{
    blink_interval_ms =
        tud_mounted()
        ? BLINK_MOUNTED
        : BLINK_NOT_MOUNTED;
}



void tud_cdc_rx_cb(uint8_t itf)
{
    (void) itf;
}



void hid_task(void)
{
    const uint32_t interval_ms = 10;

    static uint32_t start_ms = 0;

    if (board_millis() - start_ms < interval_ms)
    {
        return;
    }

    start_ms += interval_ms;

    bool button_pressed = board_button_read();



    if (button_pressed && !button_was_pressed)
    {

        if (tud_hid_ready() && !key_pressed)
        {
            uint8_t keycode[6] = {0};

            keycode[0] = picolink[picolink_index];

            tud_hid_keyboard_report(
                REPORT_ID_KEYBOARD,
                0,
                keycode
            );

            key_pressed = true;
        }
    }

    button_was_pressed = button_pressed;
}


void tud_hid_report_complete_cb(
    uint8_t instance,
    uint8_t const* report,
    uint16_t len
)
{
    (void) instance;
    (void) report;
    (void) len;


    if (key_pressed)
    {
        tud_hid_keyboard_report(
            REPORT_ID_KEYBOARD,
            0,
            NULL
        );

        key_pressed = false;

   
        picolink_index++;

        if (picolink_index >= sizeof(picolink))
        {
            picolink_index = 0;
        }
    }
}

uint16_t tud_hid_get_report_cb(
    uint8_t instance,
    uint8_t report_id,
    hid_report_type_t report_type,
    uint8_t* buffer,
    uint16_t reqlen
)
{
    (void) instance;
    (void) report_id;
    (void) report_type;
    (void) buffer;
    (void) reqlen;

    return 0;
}

void tud_hid_set_report_cb(
    uint8_t instance,
    uint8_t report_id,
    hid_report_type_t report_type,
    uint8_t const* buffer,
    uint16_t bufsize
)
{
    (void) instance;

    if (report_type == HID_REPORT_TYPE_OUTPUT)
    {
        if (report_id == REPORT_ID_KEYBOARD)
        {
            if (bufsize < 1)
            {
                return;
            }

            uint8_t const kbd_leds = buffer[0];

            if (kbd_leds & KEYBOARD_LED_CAPSLOCK)
            {
                blink_interval_ms = 0;
                board_led_write(true);
            }
            else
            {
                board_led_write(false);
                blink_interval_ms = BLINK_MOUNTED;
            }
        }
    }
}

void led_blinking_task(void)
{
    static uint32_t start_ms = 0;
    static bool led_state = false;

    if (!blink_interval_ms)
    {
        return;
    }
    if (board_millis() - start_ms < blink_interval_ms)
    {
        return;
    }
    start_ms += blink_interval_ms;
    board_led_write(led_state);
    led_state = !led_state;
}