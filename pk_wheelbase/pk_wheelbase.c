#include "pk_wheelbase.h"

Wheelbase wheelbase;

void wheelbase_init()
{
	wheelbase.motor[0].x = 0.25;
	wheelbase.motor[0].y = -0.95;
	wheelbase.motor[0].r = -1;
	wheelbase.motor[1].x = 0.25;
	wheelbase.motor[1].y = 0.95;
	wheelbase.motor[1].r = -1;
	wheelbase.motor[2].x = -1;
	wheelbase.motor[2].y = 0;
	wheelbase.motor[2].r = -2;
	wheelbase.multiplierX = 1.5;
	wheelbase.multiplierY = 1.9;
	wheelbase.multiplierR = 4;
	wheelbase.xComponent_acc = 0;
	wheelbase.yComponent_acc = 0;
	wheelbase.rotation_acc = 0;
	wheelbase.tick = 0;
}

void addComponent(int magnitude, int bearing, int rotation)
{
	int xComponent = magnitude * int_sin(bearing * 10) / 10000;
	int yComponent = magnitude * int_cos(bearing * 10) / 10000;
	wheelbase.xComponent_acc += xComponent / wheelbase.multiplierX;
	wheelbase.yComponent_acc += yComponent / wheelbase.multiplierY;
	wheelbase.rotation_acc += rotation / wheelbase.multiplierR;
}

void parseWheelbase()
{
	double accel_correction = 1;
	double speed_correction = 1;
	double accel = (get_full_ticks() - wheelbase.tick) * ACCEL;
	double max_delta = 0;
	double max_speed = 0;
	for (int i = 0; i != WHEELBASE_MOTOR_COUNT; i++)
	{
		wheelbase.motor[i].old = wheelbase.motor[i].output;
		wheelbase.motor[i].output = wheelbase.xComponent_acc * wheelbase.motor[i].x + wheelbase.yComponent_acc * wheelbase.motor[i].y + wheelbase.rotation_acc * wheelbase.motor[i].r;
		if (Abs(wheelbase.motor[i].output - wheelbase.motor[i].old) > max_delta)
		{
			max_delta = Abs(wheelbase.motor[i].output - wheelbase.motor[i].old);
		}
		if (Abs(wheelbase.motor[i].output) > max_speed)
		{
			max_speed = Abs(wheelbase.motor[i].output);
		}
	}
	if (max_delta > accel)
	{
		accel_correction = accel / max_delta;
	}
	if (max_speed * accel_correction > MAX_MOTOR_OUTPUT)
	{
		speed_correction = MAX_MOTOR_OUTPUT / (max_speed * accel_correction);
	}
	for (int i = 0; i != WHEELBASE_MOTOR_COUNT; i++)
	{
		wheelbase.motor[i].output = wheelbase.motor[i].old + (wheelbase.motor[i].output - wheelbase.motor[i].old) * accel_correction * speed_correction;
	}
	wheelbase.xComponent_acc = 0;
	wheelbase.yComponent_acc = 0;
	wheelbase.rotation_acc = 0;
	wheelbase.tick = get_full_ticks();
}

void sendMotorCommands()
{
	motor_set_vel(MOTOR1, (int)(wheelbase.motor[0].output), CLOSE_LOOP);
	motor_set_vel(MOTOR2, (int)(wheelbase.motor[1].output), CLOSE_LOOP);
	motor_set_vel(MOTOR3, (int)(wheelbase.motor[2].output), CLOSE_LOOP);
}
