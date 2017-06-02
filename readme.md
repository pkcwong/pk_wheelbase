# pk_wheelbase

release date - 2017/06/02

Log:
- default support for 3-wheel wheelbase
- added support for max speed clamp and acceleration
- added support for add components
- added support for one-time parse and send motor commands

---

## Brief

Helper functions for wheelbase motor output management.<br>
Note, this is NOT a library. Function implementations may have to be modified to work with systems.

## Dependencies

- approx_math.h
- can_motor.h
- ticks.h

## Method Summary

Method Declaration | Arguments | Method Description
--- | --- | ---
void wheelbase_init | void | initialize components for motors
void addComponent | int magnitude, int bearing, int rotation | adds a vector component to wheelbase
void parseWheelbase | void | parse components to motor output values
void sendMotorCommands | void | send CAN messages to motors

## Variables and Constants

Variables | Description
--- | ---
\#define WHEELBASE_MOTOR_COUNT | number of motors on wheelbase
\#define MAX_MOTOR_OUTPUT | maximum output of one motor
\#define ACCEL | maximum output increment or decrement per tick for one motor

## Method Details

### void wheelbase_init

This method initializes the contribution vector for each motor on wheelbase. To be called on wheelbase start up. Initialization as follows.

First, set up the environment variables.
```C
/* file: pk_wheelbase.h */
#define WHEELBASE_MOTOR_COUNT 3	// number of motors
#define MAX_MOTOR_OUTPUT 147	// max output
#define ACCEL 0.1	// 0.1 increment per tick
```

Next, set up the wheelbase Initialization.
```C
/* file: pk_wheelbase.c */
void wheelbase_init()
{
	wheelbase.motor[0].x = 0.25;	// x component, Cartesian coordinates, local
	wheelbase.motor[0].y = -0.95;	// y component, Cartesian coordinates, local
	wheelbase.motor[0].r = -1;	// rotation torque, clockwise is positive
	wheelbase.motor[1].x = 0.25;
	wheelbase.motor[1].y = 0.95;
	wheelbase.motor[1].r = -1;
	wheelbase.motor[2].x = -1;
	wheelbase.motor[2].y = 0;
	wheelbase.motor[2].r = -2;
	wheelbase.multiplierX = 1.5;	// sum of abs(wheelbase.motor[].x)
	wheelbase.multiplierY = 1.9;	// sum of abs(wheelbase.motor[].y)
	wheelbase.multiplierR = 4;	// sum of abs(wheelbase.motor[].r)
	wheelbase.xComponent_acc = 0;
	wheelbase.yComponent_acc = 0;
	wheelbase.rotation_acc = 0;
	wheelbase.tick = 0;
}
```
### void addComponent

This method adds a vector to wheelbase. Parameters are arbitrary with no specific scaling.

### void parseWheelbase

This method parses the components to motor values. Speed clamps and acceleration clamps are applied if necessary. To be called in every cycle before ```sendMotorCommands```.

### void sendMotorCommands

This method sends motor commands to respective motors. To be called in every cycle after ```parseWheelbase```.
