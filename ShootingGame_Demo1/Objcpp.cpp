#pragma once
#include "Obj.h"

Obj::Obj(float l, float t, float r, float b, float speed = 0) 
:rc{l,t,r,b}
,fspeed(speed){}

float Obj::GetL() { return rc.l; }
float Obj::GetT() { return rc.t; }
float Obj::GetR() { return rc.r; }
float Obj::GetB() { return rc.b; }