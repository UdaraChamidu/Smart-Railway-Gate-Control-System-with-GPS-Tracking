#include "servo.h"
#include "driver/mcpwm.h"
#include "esp_log.h"

static const char *TAG = "SERVO";

// MCPWM configuration (used for PWM signal generation)
#define PWM_FREQUENCY 50 // 50Hz frequency for servo control (standard for most servos)
#define PWM_UNIT MCPWM_UNIT_0
#define PWM_TIMER MCPWM_TIMER_0
#define PWM_GEN 0
#define PWM_CHANNEL MCPWM0A

/**
 * @brief Initialize the servo control pin and configure MCPWM for PWM output.
 */
void servo_init(void) {
    // Initialize the MCPWM module for PWM generation
    mcpwm_gpio_init(PWM_UNIT, PWM_GEN, SERVO_PIN);

    mcpwm_config_t pwm_config = {
        .frequency = PWM_FREQUENCY, // Set frequency to 50Hz
        .cmpr_a = 0,                // Initial duty cycle for servo (0% at start)
        .cmpr_b = 0,
        .duty_mode = MCPWM_DUTY_MODE_0,
    };
    // Initialize the MCPWM timer with the configuration
    ESP_ERROR_CHECK(mcpwm_init(PWM_UNIT, PWM_TIMER, &pwm_config));
}

/**
 * @brief Set the angle of the servo motor.
 * @param angle Angle to move the servo to (0 to 180 degrees).
 */
void servo_set_angle(uint8_t angle) {
    // Ensure angle is within valid range
    if (angle < SERVO_MIN_ANGLE || angle > SERVO_MAX_ANGLE) {
        ESP_LOGW(TAG, "Angle must be between %d and %d", SERVO_MIN_ANGLE, SERVO_MAX_ANGLE);
        return;
    }

    // Calculate the corresponding duty cycle for the given angle (0-100%).
    // Standard servos use a duty cycle range of ~5% to ~10% to move from 0 to 180 degrees.
    uint32_t duty_cycle = (uint32_t)(1000 + (angle * 1000) / 180);  // Duty cycle in microseconds

    // Set the duty cycle on the PWM channel (output signal)
    mcpwm_set_duty(PWM_UNIT, PWM_TIMER, PWM_CHANNEL, duty_cycle);
    ESP_LOGI(TAG, "Servo angle set to: %d degrees", angle);
}
