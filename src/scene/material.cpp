#include <cmath>
#include <algorithm>
#include "ray.h"
#include "material.h"
#include "light.h"

Material::Material(): ke(vec3f(0.0, 0.0, 0.0))
                      , ka(vec3f(0.0, 0.0, 0.0))
                      , ks(vec3f(0.0, 0.0, 0.0))
                      , kd(vec3f(0.0, 0.0, 0.0))
                      , kr(vec3f(0.0, 0.0, 0.0))
                      , kt(vec3f(0.0, 0.0, 0.0))
                      , shininess(0.0)
                      , index(1.0)
{
}

// Apply the phong model to this point on the surface of the object, returning
Material::Material(const vec3f& e, const vec3f& a, const vec3f& s, const vec3f& d, const vec3f& r, const vec3f& t,
                   double sh, double in): ke(e), ka(a), ks(s), kd(d), kr(r), kt(t), shininess(sh), index(in)
{
}

// the color of that point.
vec3f Material::shade( Scene *scene, const Ray& r, const ISect& i ) const
{
	// YOUR CODE HERE

	// For now, this method just returns the diffuse color of the object.
	// This gives a single matte color for every distinct surface in the
	// scene, and that's it.  Simple, but enough to get you started.
	// (It's also inconsistent with the phong model...)

	// Your mission is to fill in this method with the rest of the phong
	// shading model, including the contributions of all the light sources.
    // You will need to call both distanceAttenuation() and shadowAttenuation()
    // somewhere in your code in order to compute shadows and light falloff.

	vec3f V = r.getDirection();
	vec3f N = i.N;
	vec3f P = r.at(i.t);
	vec3f color = ke+prod(ka, scene->ambientLight);

	auto diffuseColor = kd;

	const auto* obj = dynamic_cast<const MaterialSceneObject*>(i.obj);
	if (obj != nullptr)
	{
		// obj is type of MaterialSceneObject
		// do uv mapping
		auto[u, v] = obj->getUV(r, i);
		if (diffuseTexturePtr != nullptr)
		{
			diffuseColor = diffuseTexturePtr->getColorByUV(u, v);
		}
	}

	for (auto l = scene->beginLights(); l != scene->endLights(); ++l) {
		Light *pLight = *l;
		vec3f L = pLight->getDirection(P);
		vec3f intensity = pLight->getColor(P);
		const double distAtte = pLight->distanceAttenuation(P);
		vec3f shadowAtte = pLight->shadowAttenuation(P);

		const double diffuse = std::max(0.0, N.dot(L));

		double specular = 0;
		vec3f R = L - 2 * L.dot(N) * N;
		R = R.normalize();
		const double specAngle = std::max(R.dot(V), 0.0);
		specular = pow(specAngle, shininess * 128);

		vec3f ret = prod(distAtte *(specular * ks + diffuse * diffuseColor), intensity);
		ret = prod(ret, shadowAtte);
		color += ret;
	}


	for (int i = 0; i < 3; i++) color[i] = std::min(color[i], 1.0);
	return color;
}

Material& Material::operator+=(const Material& m)
{
	ke += m.ke;
	ka += m.ka;
	ks += m.ks;
	kd += m.kd;
	kr += m.kr;
	kt += m.kt;
	index += m.index;
	shininess += m.shininess;
	return *this;
}