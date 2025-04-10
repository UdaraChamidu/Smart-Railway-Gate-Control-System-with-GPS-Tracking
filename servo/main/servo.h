#ifndef SERVO_H_
#define SERVO_H_

#include <stdint.h>

// Define the GPIO pin where the servo is connected
#define SERVO_PIN 13

// Servo angle range (0 to 180 degrees)
#define SERVO_MIN_ANGLE 0
#define SERVO_MAX_ANGLE 180

// Function prototypes
void servo_init(void);
void servo_set_angle(uint8_t angle);

#endif /* SERVO_H_ */
