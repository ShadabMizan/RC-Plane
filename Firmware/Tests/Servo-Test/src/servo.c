#include "servo.h"
void servo_pwm_setup(mcpwm_timer_handle_t *timer, mcpwm_oper_handle_t *oper, mcpwm_cmpr_handle_t *cmpr, mcpwm_gen_handle_t *gen) {
    /* 
        Timer Config Notes
        Timer's job is to keep counting up from 0 to a max value. From this counting, the comparator can make determine a PWM Signal.
    */
    mcpwm_timer_config_t timer_config = {
        .group_id = 0,
        .clk_src = MCPWM_TIMER_CLK_SRC_DEFAULT,
        .resolution_hz = PWM_RESOLUTION_HZ,
        .period_ticks = PWM_PERIOD_TICKS,
        .count_mode = MCPWM_TIMER_COUNT_MODE_UP
    };

    ESP_ERROR_CHECK(
        mcpwm_new_timer(&timer_config, timer)
    );

    /*
        Operator Config Notes
        The MCPWM Operator is what encapsulates both the comparator, and generator, as well as other useful functions like dead-time.
    */
    mcpwm_operator_config_t operator_config = {
        .group_id = 0
    };

    ESP_ERROR_CHECK(
        mcpwm_new_operator(&operator_config, oper)
    );

    // Linking the operator to the timer.
    ESP_ERROR_CHECK(
        mcpwm_operator_connect_timer(*oper, *timer)
    );

    /*
        Comparator Config Notes
        The comparator is responsible for comparing the current value on the timer to a predefined value, 
        then determines if the output signal should transition from high to low etc.
    */

    mcpwm_comparator_config_t comparator_config = {
        .flags.update_cmp_on_tez = true             // Comparator updates at the start of each period (time equal zero)
    };

    ESP_ERROR_CHECK(
        mcpwm_new_comparator(*oper, &comparator_config, cmpr)
    );

    ESP_ERROR_CHECK(
        mcpwm_comparator_set_compare_value(*cmpr, FORWARD_ROTATION_TICKS)     // Initial PWM corresponds to no rotation.
    );

    /*
        Generator Config Notes
        Generator is the module that places the PWM signal onto a specified GPIO Pin
    */

    mcpwm_generator_config_t generator_config = {
        .gen_gpio_num = SERVO_GPIO
    };

    ESP_ERROR_CHECK(
        mcpwm_new_generator(*oper, &generator_config, gen)
    );

    // Set generator action on timer event
    mcpwm_gen_timer_event_action_t timer_action_config = {
        .direction = MCPWM_TIMER_DIRECTION_UP,   // Timer is counting up
        .event = MCPWM_TIMER_EVENT_EMPTY,        // Action triggered when timer resets to zero
        .action = MCPWM_GEN_ACTION_HIGH          // Set output to HIGH at the start of the PWM cycle
    };
    ESP_ERROR_CHECK(mcpwm_generator_set_action_on_timer_event(*gen, timer_action_config));

    // Set generator action on comparator event
    mcpwm_gen_compare_event_action_t compare_action_config = {
        .comparator = *cmpr,                     // Specify the comparator to use
        .action = MCPWM_GEN_ACTION_LOW          // Set output to LOW when reaching comparator value
    };
    ESP_ERROR_CHECK(
        mcpwm_generator_set_action_on_compare_event(*gen, compare_action_config)
    );

    ESP_ERROR_CHECK(
        mcpwm_generator_set_action_on_compare_event(*gen, compare_action_config)
    );

    // Start Timer
        ESP_ERROR_CHECK(
        mcpwm_timer_enable(*timer)
    );

    ESP_ERROR_CHECK(
        mcpwm_timer_start_stop(*timer, MCPWM_TIMER_START_NO_STOP)
    );

    // Successful Setup!
    ESP_LOGI(
        "PWM_Setup", "MCPWM configuration complete: GPIO %d, Frequency %d Hz, Duty Cycle %d ms", 
        SERVO_GPIO, PWM_FREQUENCY_HZ, NO_ROTATION_TICKS
    );
}

