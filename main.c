#include <stdio.h>
#include "pico/stdlib.h"
#include "tusb.h"

void hid_init(void);
void hid_task(void);

int main() {
    tusb_init();
    hid_init();

    while (1) {
        tud_task();
        hid_task();
    }
}


void hid_init(void) {
    for (int gpio = 2; gpio < 14; gpio++) {
        gpio_init(gpio);
        gpio_set_dir(gpio, GPIO_IN);
        gpio_pull_up(gpio);
    }
}

#if defined NEOGEO
static uint8_t report[] = {
    0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0x00, 0x00,
    0x00
};

void hid_task(void) {
    uint32_t gpios = ~gpio_get_all();
    uint8_t stick = (gpios & 0x3C00) >> 10;

    report[0] = 
        ((gpios & (1 << 2)) ? (1 << 1) : 0) |
        ((gpios & (1 << 5)) ? (1 << 0) : 0) |
        ((gpios & (1 << 3)) ? (1 << 4) : 0) |
        ((gpios & (1 << 6)) ? (1 << 3) : 0);

    report[1] =
        ((gpios & (1 << 8)) ? (1 << 3) : 0) |
        ((gpios & (1 << 9)) ? (1 << 2) : 0);

    switch (stick) {
        case 0x01: // UP
            report[2] = 0x00;
            break;
        case 0x09: // UP + RIGHT
            report[2] = 0x01;
            break;
        case 0x08: // RIGHT
            report[2] = 0x02;
            break;
        case 0x0a: // DOWN + RIGHT
            report[2] = 0x03;
            break;
        case 0x02: // DOWN
            report[2] = 0x04;
            break;
        case 0x06: // DOWN + LEFT
            report[2] = 0x05;
            break;
        case 0x04: // LEFT
            report[2] = 0x06;
            break;
        case 0x05: // UP + LEFT
            report[2] = 0x07;
            break;
        default:
            report[2] = 0x0f;
            break;
    }

    if (tud_hid_ready()) {
        tud_hid_report(0x00, report, sizeof(report));
    }
}
#endif

#if (defined ASTRO_CITY) || (defined EGRET_II)
static uint8_t report[] = {
    0x01, 0x7f, 0x7f, 0x7f,
    0x7f, 0x0f, 0x00, 0x00
};

void hid_task(void) {
    uint32_t gpios = ~gpio_get_all();
    uint8_t stick = (gpios & 0x3C00) >> 10;
    uint8_t controls = 0x00;

    report[3] =
        (stick & 0x04) ? 0x00 :
        (stick & 0x08) ? 0xff :
        0x7f;

    report[4] =
        (stick & 0x01) ? 0x00 :
        (stick & 0x02) ? 0xff :
        0x7f;

    report[5] =
#if defined ASTRO_CITY
        ((gpios & (1 << 2)) ? 0x4f : 0x00) |
        ((gpios & (1 << 3)) ? 0x2f : 0x00) |
        ((gpios & (1 << 6)) ? 0x1f : 0x00) |
        ((gpios & (1 << 5)) ? 0x8f : 0x00) |
        0x0f;
#elif defined EGRET_II
        (gpios & (1 << 3)) ? (1 << 6) : 0x00 |
        (gpios & (1 << 4)) ? (1 << 5) : 0x00 |
        (gpios & (1 << 6)) ? (1 << 7) : 0x00 |
        (gpios & (1 << 7)) ? (1 << 4) : 0x00;
#endif

    report[6] =
#if defined ASTRO_CITY
        ((gpios & (1 << 4)) ? 0x02 : 0x00) |
        ((gpios & (1 << 7)) ? 0x01 : 0x00) |
        ((gpios & (1 << 8)) ? 0x10 : 0x00) |
        ((gpios & (1 << 9)) ? 0x20 : 0x00) |
        0x00;
#elif defined EGRET_II
        (gpios & (1 << 2)) ? (1 << 0) : 0x00 |
        (gpios & (1 << 5)) ? (1 << 4) : 0x00 |
        (gpios & (1 << 8)) ? (1 << 2) : 0x00 |
        (gpios & (1 << 9)) ? (1 << 3) : 0x00;
#endif
        
#if defined EGRET_II
    // Home button
    report[6] = (gpios & (1 << 8) && (gpios & (1 << 9))) ? (1 << 5) : report[6];
#endif

    if (tud_hid_ready()) {
        tud_hid_report(0x00, report, sizeof(report));
    }
}
#endif

uint16_t tud_hid_get_report_cb(
        uint8_t instance,
        uint8_t report_id,
        hid_report_type_t report_type,
        uint8_t *buffer,
        uint16_t reqlen) {
    // TODO Not implemented
    return 0;
}

void tud_hid_set_report_cb(
        uint8_t instance,
        uint8_t report_id,
        hid_report_type_t report_type,
        uint8_t const *buffer,
        uint16_t bufsize) {
    // TODO Not implemented
    return;
}
