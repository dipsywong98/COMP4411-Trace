//
// material.h
//
// The Material class: a description of the physical properties of a surface
// that are used to determine how that surface interacts with light.

#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "../vecmath/vecmath.h"

class Scene;
class Ray;
class ISect;


class Material
{
public:
	virtual ~Material() = default;

	Material()
        : ke( vec3f( 0.0, 0.0, 0.0 ) )
        , ka( vec3f( 0.0, 0.0, 0.0 ) )
        , ks( vec3f( 0.0, 0.0, 0.0 ) )
        , kd( vec3f( 0.0, 0.0, 0.0 ) )
        , kr( vec3f( 0.0, 0.0, 0.0 ) )
        , kt( vec3f( 0.0, 0.0, 0.0 ) )
        , shininess( 0.0 ) 
		, index(1.0) {
		id = cnt++;
	}

    Material( const vec3f& e, const vec3f& a, const vec3f& s, 
              const vec3f& d, const vec3f& r, const vec3f& t, double sh, double in)
		: ke(e), ka(a), ks(s), kd(d), kr(r), kt(t), shininess(sh), index(in) {
		id = cnt++;
	}

	static Material getAir()
	{
		return Material({ 0,0,0 }, { 0,0,0 }, { 0,0,0 }, { 0,0,0 }, { 0,0,0 }, { 1,1,1 }, 0, 1);
	}

	virtual vec3f shade( Scene *scene, const Ray& r, const ISect& i ) const;

    vec3f ke;                    // emissive
    vec3f ka;                    // ambient
    vec3f ks;                    // specular
    vec3f kd;                    // diffuse
    vec3f kr;                    // reflective
    vec3f kt;                    // transmissive
    
    double shininess;
    double index;               // index of refraction

    
                                // material with zero coeffs for everything
                                // as opposed to the "default" material which is
                                // a pleasant blue.
    static const Material ZERO;

    Material &
    operator+=( const Material &m )
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

    friend Material operator*( double d, Material m );
	int id;
	static int cnt;
};

inline Material
operator*(const double d, Material m )
{
    m.ke *= d;
    m.ka *= d;
    m.ks *= d;
    m.kd *= d;
    m.kr *= d;
    m.kt *= d;
    m.index *= d;
    m.shininess *= d;
    return m;
}
// extern Material THE_DEFAULT_MATERIAL;

#endif // MATERIAL_H_
