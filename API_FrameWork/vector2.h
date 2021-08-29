#pragma once
struct vector2
{
	float x, y;

public:
	vector2()
		: x(0.f), y(0.f)
	{}
	vector2(float _x, float _y)
		: x(_x), y(_y)
	{}
	vector2(const POINT& _pt)
		: x((float)_pt.x)
		, y((float)_pt.y)
	{}

	//이동
	vector2 moveX(float dis) {
		x += dis;
		return *this;
	}
	vector2 moveY(float dis) {
		y += dis;
		return *this;
	}

	//+
	vector2 operator+(const vector2& rhs) const
	{
		return vector2(x + rhs.x, y + rhs.y);
	}
	vector2 operator+(const float& rhs) const
	{
		return vector2(x + rhs, y + rhs);
	}
	void operator+=(const vector2& rhs)
	{
		x += rhs.x;
		y += rhs.y;
	}
	void operator+=(const float& rhs)
	{
		x += rhs;
		y += rhs;
	}

	//-
	vector2 operator-(const vector2& rhs) const
	{
		return vector2(x - rhs.x, y - rhs.y);
	}
	vector2 operator-(const float& rhs) const
	{
		return vector2(x - rhs, y - rhs);
	}
	void operator-=(const vector2& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
	}
	void operator-=(const float& rhs)
	{
		x -= rhs;
		y -= rhs;
	}

	//*
	vector2 operator*(const vector2& rhs) const
	{
		return vector2(x * rhs.x, y * rhs.y);
	}
	vector2 operator*(const float& rhs) const
	{
		return vector2(x * rhs, y * rhs);
	}
	void operator*=(const vector2& rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
	}
	void operator*=(const float& rhs)
	{
		x *= rhs;
		y *= rhs;
	}

	///
	vector2 operator/(const vector2& rhs) const
	{
		return vector2(x / rhs.x, y / rhs.y);
	}
	vector2 operator/(const float& rhs) const
	{
		return vector2(x / rhs, y / rhs);
	}
	void operator/=(const vector2& rhs)
	{
		x /= rhs.x;
		y /= rhs.y;
	}
	void operator/=(const float& rhs)
	{
		x /= rhs;
		y /= rhs;
	}

	//비교
	bool operator==(const vector2& rhs) const
	{
		return x == rhs.x && y == rhs.y;
	}
	bool operator!=(const vector2& rhs) const
	{
		return x != rhs.x || y != rhs.y;
	}

	//대입
	void operator=(const vector2& rhs)
	{
		x = rhs.x;
		y = rhs.y;
	}
	void operator=(const POINT& rhs)
	{
		x = (float)rhs.x;
		y = (float)rhs.y;
	}

	//-()
	vector2& operator-()
	{
		return vector2(-x, -y);
	}

	//거리
	float Distance(const vector2& rhs) const
	{
		float disX = x - rhs.x;
		float disY = y - rhs.y;
		return sqrtf(pow(disX, 2) + pow(disY, 2));
	}

	//길이
	float Length() const
	{
		return sqrtf(x * x + y * y);
	}

	//단위벡터
	//	x = x / size,	 y = y / size
	vector2& Normalize() const {
		float size;
		size = Length();
		if (size == 0) return vector2(0, 0);
		return vector2(x / size, y / size);
	}

	//내적
	//(내적값) = x1*x2 + y1*y2
	float Dot(vector2& rhs) const {
		return abs(x * rhs.x + y * rhs.y);
	}

	//정사영 길이
	float Projection(const vector2& rhs) const {
		vector2 unit = rhs.Normalize();
		return (*this).Dot(unit);
	}

	//사영
	vector2& Project(const vector2& rhs) const {
		vector2 unit = rhs.Normalize();
		return unit * (*this).Dot(unit);
	}

	//각도
	float AngleTo(const vector2& rhs) const {
		return UTIL::getAngle(x, y, rhs.x, rhs.y);
	}

	//벡터사이의 각도(theha)
	float Interval(vector2& rhs) const {
		float cosTheta = this->Dot(rhs) / this->Length() * rhs.Length();
		if (cosTheta < 0) cosTheta *= -1;
		return acosf(cosTheta);
	}

	//(매개변수가) 나의 어느방향에 존재하는가
	FOWARD whichFoward(vector2& rhs) const {
		float incrementX = rhs.x - x;
		float incrementY = rhs.y - y;

		if (incrementX == 0 && incrementY == 0)
		{
			return FOWARD::NONE;
		}
		if (incrementX > 0 && incrementY == 0)
		{
			return FOWARD::RIGHT;
		}
		if (incrementX < 0 && incrementY == 0)
		{
			return FOWARD::LEFT;
		}
		if (incrementX == 0 && incrementY > 0)
		{
			return FOWARD::DOWN;
		}
		if (incrementX == 0 && incrementY < 0)
		{
			return FOWARD::UP;
		}
		if (incrementX > 0 && incrementY > 0)
		{
			return FOWARD::RIGHTDOWN;
		}
		if (incrementX > 0 && incrementY < 0)
		{
			return FOWARD::RIGHTUP;
		}
		if (incrementX < 0 && incrementY > 0)
		{
			return FOWARD::LEFTDOWN;
		}
		if (incrementX < 0 && incrementY < 0)
		{
			return FOWARD::LEFTUP;
		}
	}
}typedef Vec2;