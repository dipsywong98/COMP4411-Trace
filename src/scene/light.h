#ifndef LIGHT_H_
#define LIGHT_H_

#include "SceneElement.h"

class Light : public SceneElement
{
public:
	virtual vec3f shadowAttenuation(const vec3f& p, std::stack<Geometry*>& intersections) const = 0;
	virtual double distanceAttenuation(const vec3f& p) const = 0;
	virtual vec3f getColor(const vec3f& p) const = 0;
	virtual vec3f getDirection(const vec3f& p) const = 0;

protected:
	Light(Scene* scene, const vec3f& col);

	vec3f color;
};

class DirectionalLight : public Light
{
public:
	DirectionalLight(Scene* scene, const vec3f& orien, const vec3f& color);

	vec3f shadowAttenuation(const vec3f& p, std::stack<Geometry*>& intersections) const override;
	double distanceAttenuation(const vec3f& p) const override;
	vec3f getColor(const vec3f& p) const override;
	vec3f getDirection(const vec3f& p) const override;

protected:
	vec3f orientation;
};

class PointLight : public Light
{
public:
	PointLight(Scene* scene, const vec3f& pos, const vec3f& color);

	vec3f shadowAttenuation(const vec3f& p, std::stack<Geometry*>& intersections) const override;
	double distanceAttenuation(const vec3f& p) const override;
	vec3f getColor(const vec3f& p) const override;
	vec3f getDirection(const vec3f& p) const override;

protected:
	vec3f position;
};

#endif // LIGHT_H_
