/**
 * @file lv_demo_keypad_encoder.h
 *
 */

#ifndef LV_DEMO_KEYPAD_ENCODER_H
#define LV_DEMO_KEYPAD_ENCODER_H

#ifdef __cplusplus
extern "C" {
#endif

#define LV_USE_DEMO_KEYPAD_AND_ENCODER 1
#define    LV_EX_KEYBOARD 1
/*********************
 *      INCLUDES
 *********************/

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void lv_demo_keypad_encoder(void);

bool keyboard_read(lv_indev_drv_t * drv, lv_indev_data_t*data);

/**********************
 *      MACROS
 **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_DEMO_KEYPAD_ENCODER_H*/
