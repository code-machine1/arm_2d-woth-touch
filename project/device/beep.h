#ifndef _BEEP_H
#define _BEEP_H

#include <stdio.h>
#include "at32f415.h"

/* output */
#define BEEP_PORT             GPIOC
#define BEEP_PIN              GPIO_PINS_4

typedef enum
{
    BEEP_OPEN = 0,
    BEEP_CLOSE,
} BEEP_WORK;


typedef enum
{
    BEEP_OFF = 0,
    BEEP_SHORT,
	BEEP_LONG,
} BEEP_STATUS;

typedef struct
{
	BEEP_WORK beep_work;
    char *name;
    BEEP_STATUS status;      //×´Ì¬
    void (*on)(void);     //´ò¿ª
    void (*off)(void);    //¹Ø±Õ
	
} beep_t;

static void beep_on(void);
static void beep_off(void);
#endif
