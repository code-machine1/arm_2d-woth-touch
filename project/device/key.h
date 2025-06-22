#ifndef _KEY_H
#define _KEY_H

#include <stdio.h>
#include "at32f415.h"

/* input */

#define KEY_HEL1_PORT             GPIOF
#define KEY_HEL1_PIN              GPIO_PINS_6
#define READ_KEY_HEL1             gpio_input_data_bit_read(KEY_HEL1_PORT, KEY_HEL1_PIN)

#define KEY_HEL2_PORT             GPIOA
#define KEY_HEL2_PIN              GPIO_PINS_11
#define READ_KEY_HEL2             gpio_input_data_bit_read(KEY_HEL2_PORT, KEY_HEL2_PIN)

#define KEY_HEL3_PORT             GPIOA
#define KEY_HEL3_PIN              GPIO_PINS_12
#define READ_KEY_HEL3             gpio_input_data_bit_read(KEY_HEL3_PORT, KEY_HEL3_PIN)

#define KEY_HEL4_PORT             GPIOC
#define KEY_HEL4_PIN              GPIO_PINS_8
#define READ_KEY_HEL4             gpio_input_data_bit_read(KEY_HEL4_PORT, KEY_HEL4_PIN)

#define KEY_HEL5_PORT             GPIOC
#define KEY_HEL5_PIN              GPIO_PINS_7
#define READ_KEY_HEL5             gpio_input_data_bit_read(KEY_HEL5_PORT, KEY_HEL5_PIN)

#define KEY_HEL6_PORT             GPIOC
#define KEY_HEL6_PIN              GPIO_PINS_6
#define READ_KEY_HEL6             gpio_input_data_bit_read(KEY_HEL6_PORT, KEY_HEL6_PIN)


#define LONG_PRESS_TIME          10 
#define KEY_CYCLE_TIME           10
#define KEY_NUMBER               6
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    K_RELEASE,
    K_PRESS,
} KEY_VALUE;

typedef enum {
    KS_RELEASE,
	//KS_LONG_RELEASE,
	KS_CHECK,
    KS_PRESS,
	KS_LONG_PRESS,
} KEY_STATUS;

typedef enum {
    KE_PRESS,
    KE_RELEASE,
    KE_LONG_PRESS,
    KE_LONG_RELEASE,
    KE_NONE,
} KEY_EVENT;

typedef struct {
    KEY_STATUS status;
    int count;
	int key_cycle_time;
    KEY_VALUE (*get)(void);
} KEY;

KEY_EVENT key_event_check(KEY *key, int interval);

#ifdef __cplusplus
}
#endif
#endif


