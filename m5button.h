/**
 * m5button.h
 * 
 * (C) 2019 - Pablo Bacho <pablo@pablobacho.com>
 * This code is licensed under the MIT License.
 */

#ifndef _M5BUTTON_H_
#define _M5BUTTON_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "esp_system.h"
#include "esp_log.h"
#include "esp_event_loop.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "m5stickc.h"

#define M5BUTTON_BUTTON_A_GPIO      GPIO_NUM_37

ESP_EVENT_DECLARE_BASE(M5BUTTON_EVENT_BASE); /*!< BASE event of this module */

/**
 * List of possible events this module can trigger
 */
typedef enum {
    M5BUTTON_BUTTON_A_CLICK = 0,        /*!< Normal button press */
    M5BUTTON_BUTTON_A_HOLD,             /*!< Button hold */
    M5BUTTON_EVENT_MAX
} m5_button_event_id_t;

/*!< event_group flag bits */
#define M5BUTTON_BUTTON_A_PUSH_BIT      0b00000001
#define M5BUTTON_BUTTON_A_POP_BIT       0b00000010

/*!< Time constants */
#define M5BUTTON_DEBOUNCE_TIME   10
#define M5BUTTON_HOLD_TIME       2000

#define M5BUTTON_TASK_STACK_DEPTH   2048
#if defined(CONFIG_SUPPORT_STATIC_ALLOCATION)
extern StaticEventGroup_t m5button_event_group_buffer;
extern StaticTask_t m5button_task_buffer;
extern StackType_t m5button_task_stack[M5BUTTON_TASK_STACK_DEPTH];
#endif

/*!< event_group */
extern EventGroupHandle_t m5button_event_group;

/**
 * @brief   Button interrupt service routine
 */
void IRAM_ATTR m5button_buttonA_isr_handler(void* arg);

/**
 * @brief   Initialize buttons
 * 
 *          Create event_group, initialized GPIO pin and register the ISR.
 * 
 * @return  ESP_OK success
 *          ESP_FAIL failed
 */
esp_err_t m5button_init();

/**
 * @brief   Generates button events
 * 
 *          This task generates button events such as press and release. It includes a debouncing mechanism.
 *          First, the task will wait for a key push or pop. When detected, it will "propose" a candidate event to be sent
 *          and configure a timer of 100ms. If the timer expires the candidate event is posted to the default loop. If
 *          button state changes before the timeout, it will propose the new state of button as candidate for event and
 *          restart timer again.
 * 
 * @param   pvParameter unused
 */
void m5button_task(void * pvParameter);

#ifdef __cplusplus
}
#endif

#endif // _M5BUTTON_H_