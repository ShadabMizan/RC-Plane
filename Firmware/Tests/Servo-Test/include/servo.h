#pragma once
#include "esp_log.h"
#include "driver/mcpwm_prelude.h"

#define SERVO_GPIO              26

#define PWM_FREQUENCY_HZ        50
#define PWM_RESOLUTION_HZ       1000000     // 1us per tick
#define PWM_PERIOD_TICKS        20000       // 20 ms

#define FORWARD_ROTATION_TICKS  500
#define NO_ROTATION_TICKS       1500
#define REVERSE_ROTATION_TICKS  2500

void servo_pwm_setup(mcpwm_timer_handle_t *timer, mcpwm_oper_handle_t *oper, mcpwm_cmpr_handle_t *cmpr, mcpwm_gen_handle_t *gen);

