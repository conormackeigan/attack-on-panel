#include "LogSpiralParticle.h"
#include "EntityManager.h"

#define entity_manager EntityManager::Instance()

const float LogSpiralParticle::a = .25f;//.35f;
const float LogSpiralParticle::b = .55f;//.36f;

LogSpiralParticle::LogSpiralParticle() : IPooled(TextureManager::TEXTURE_NAME::COMBO_CHAIN_SPIRAL)
{
	drawOrder = 1001;
}


LogSpiralParticle::~LogSpiralParticle()
{
}

void LogSpiralParticle::Set(Vector2f origin, float starting_angle)
{
	this->origin = origin;
	this->starting_angle = starting_angle;

	// Drawn one frame before being updated, so set correct position for first frame
	float rads = DEG_TO_RAD(angle);

	float x = a * cos(rads) * pow(e, b*rads);
	float y = a * sin(rads) * pow(e, b*rads);

	Vector2f point = Vector2f(x, y);
	point.Rotate(starting_angle);

	position = origin + point;
}

void LogSpiralParticle::Reset()
{
	angle = 720.f;
}

void LogSpiralParticle::Update()
{
	angle -= speed;
	if (angle < 230)
		Recycle();

	float rads = DEG_TO_RAD(angle);

	float x = a * cos(rads) * pow(e, b*rads);
	float y = a * sin(rads) * pow(e, b*rads);

	Vector2f point = Vector2f(x, y);
	point.Rotate(starting_angle);

	position = origin + point;
}
