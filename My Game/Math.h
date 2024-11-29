#pragma once
#include <algorithm>
#include <cmath>
#include <random>
namespace Math
{
	static float SmoothDamp(float current, float target, float* currentVelocity, float smoothTime, float maxSpeed, float deltaTime);
	template <typename T>
	T clamp(T value, T low, T high) {
		return (std::max)(low, (std::min)(value, high));
	}

	template <typename T>
	T lerp(T start, T end, float progress) {
		progress = clamp(progress, 0.f, 1.f);

		return start + (progress * (end - start));
	}

	static int RandomizePhase(int numPhases, float deltaTime);
};


//	smoother camera movement.
//	FROM UNITY 6 DOCUMENTATION: https://docs.unity3d.com/ScriptReference/Mathf.SmoothDamp.html
//		smooth damp "gradually moves the current value towards a target value,
//	  over a specified time and at a specified velocity. this method smoothes
//	  the current value towards a target value with a spring-damper like algorithm.

//	ALGORITHM SOURCE: https://discussions.unity.com/t/formula-behind-smoothdamp/6483
static float Math::SmoothDamp(float current, float target, float* currentVelocity, float smoothTime, float maxSpeed, float deltaTime) {
	smoothTime = (std::max)(0.0001f, smoothTime);
	float num = 2.f / smoothTime;
	float num2 = num * deltaTime;
	float num3 = 1.f / (1.f + num2 + 0.48f * num2 * num2 + 0.235f * num2 * num2 * num2);
	float num4 = current - target;
	float num5 = target;
	float num6 = maxSpeed * smoothTime;
	num4 = clamp(num4, -num6, num6);
	target = current - num4;

	float num7 = (*currentVelocity + num * num4) * deltaTime;
	*currentVelocity = (*currentVelocity - num * num7) * num3;
	float num8 = target + (num4 + num7) * num3;
	if (num5 - current > 0.f == num8 > num5)
	{
		num8 = num5;
		*currentVelocity = (num8 - num5) / deltaTime;
	}
	return num8;
}

static int Math::RandomizePhase(int numPhases, float deltaTime) {

	int remainder = RAND_MAX % (numPhases + 1);
	int x;

	/*if (((int)timer % 2 == 0)) {
		phase = rand() % numPhases;
		timer = 0.0f;
	}*/
	do {
		x = rand();
	} while (x >= RAND_MAX - remainder);

	return x % (numPhases + 1);
}