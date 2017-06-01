#ifndef _PK_WHEELBASE
#define _PK_WHEELBASE

#define WHEELBASE_MOTOR_COUNT 3
#define MAX_MOTOR_OUTPUT 147
#define ACCEL 0.1

#include "approx_math.h"
#include "can_motor.h"
#include "ticks.h"

typedef struct Motor_t Motor;
typedef struct Wheelbase_t Wheelbase;

struct Motor_t
{
	double x;
	double y;
	double r;
	double old;
	double output;
};

struct Wheelbase_t
{
	Motor motor[WHEELBASE_MOTOR_COUNT];
	double multiplierX;
	double multiplierY;
	double multiplierR;
	int xComponent_acc;
	int yComponent_acc;
	int rotation_acc;
	int tick;
};

void wheelbase_init(void);

void addComponent(int magnitude, int bearing, int rotation);
void parseWheelbase(void);
void sendMotorCommands(void);

#endif

