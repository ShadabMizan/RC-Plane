#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "servo.h"

mcpwm_timer_handle_t timer_handle = NULL;
mcpwm_oper_handle_t oper_handle = NULL;
mcpwm_cmpr_handle_t cmpr_handle = NULL;
mcpwm_gen_handle_t gen_handle = NULL;


int pwm_ticks = 1500;
void user_input(void *arg) {
    while (1) {
        scanf("%d", &pwm_ticks);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void pwm_output(void *arg) {
    mcpwm_cmpr_handle_t comparator = *(mcpwm_cmpr_handle_t*)arg;
    while (1) {
        mcpwm_comparator_set_compare_value(comparator, pwm_ticks);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void app_main() {
    // From servo.h
    servo_pwm_setup(&timer_handle, &oper_handle, &cmpr_handle, &gen_handle);

    xTaskCreate(user_input, "Input", 2048, NULL, 1, NULL);
    xTaskCreate(pwm_output, "Output", 2048, &cmpr_handle, 1, NULL);
}