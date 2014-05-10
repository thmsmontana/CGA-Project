#pragma once


class Car 
{
public:
	Car();
	~Car();
	void draw();
};


void drawWheel(float ext_radius, float inn_radius, float width);
void drawWheelCarcass(float wheel_radius, float wheel_gap, float carcass_height);
void drawInterWheelCarcass(float interwheel_l, float carcass_height);
void drawExtremity(float interwheel_w, float interwheel_l, float wheel_width, float overflow, float angle_radius, float carcass_height, float wheel_radius, float wheel_gap, float direction);
void drawFrontHood(float interwheel_l, float interwheel_w, float carcass_height, float wheel_gap, float wheel_width, float wheel_radius, float carcass_angle_radius, float front_overflow);
void drawBackHood(float interwheel_l, float interwheel_w, float carcass_height, float wheel_gap, float wheel_width, float wheel_radius, float carcass_angle_radius, float back_overflow);
void drawTop(float interwheel_l, float interwheel_w, float wheel_width, float wheel_radius, float wheel_gap, float top_height, float top_length_ratio, float top_width_ratio, float top_offset);
void drawHalfOfTheCar(float t);

void setupShipList();

