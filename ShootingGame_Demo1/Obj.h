#pragma once
#include <vector>

typedef struct _tagRectangle {
	float l, t, r, b;
} RECTANGLE ,*PRECTANGLE;

class BULLET {
private:
	std::vector<RECTANGLE> bullet;
	float bullet_speed;
	int bullet_demage;
public:
	void FireBullet(float l, float t, float r, float b, float fSpeed = 1000, int bullet_demage = 50)
	{
		bullet.reserve(10);
		bullet.push_back({ l, t, r, b });
		bullet_speed = fSpeed;
		this->bullet_demage = bullet_demage;
	}

	float GetBulletSpeed() { return bullet_speed; }
	int GetBulletDemage() { return bullet_demage; }

	std::vector<RECTANGLE>::iterator EraseBullet(std::vector<RECTANGLE>::iterator itr) { return bullet.erase(itr); }
	std::vector<RECTANGLE>::iterator GetBegin() { return bullet.begin(); }
	std::vector<RECTANGLE>::iterator GetEnd() { return bullet.end(); }
	void MoveY(float value) 
	{ 
		for (auto& const elem : bullet)
		{
			elem.t += value;
			elem.b += value;
		}
	}
};

class Obj {
private:
	RECTANGLE rc;
	float fspeed;
public:
	Obj(float, float, float, float, float);
	float GetL();
	float GetT();
	float GetR();
	float GetB();
	void SetL(float value) { rc.l = value; }
	void SetT(float value) { rc.t = value; }
	void SetR(float value) { rc.r = value; }
	void SetB(float value) { rc.b = value; }
	void MoveX(float value) { rc.l += value; rc.r += value; }
	void MoveY(float value) { rc.t += value; rc.b += value; }
	float GetSpeed() { return fspeed; }
};

class Player : public Obj {
private:
public:
	BULLET p_bullet;

	Player(float l, float t, float r, float b, float speed = 500)
		: Obj(l, t, r, b, speed) {}
};

class Monster : public Obj {
private:
	int HP;
public:
	BULLET p_bullet;
	Monster(float l, float t, float r, float b, float speed = 500, int hp = 100)
	:Obj(l, t, r, b, speed)
	, HP(hp)
	{}

	void Attacked(int demage) { HP += demage; }
	int GetHP() { return HP; }

};