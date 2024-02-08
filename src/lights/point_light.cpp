#include "point_light.hpp"

namespace RT_ISICG
{
	LightSample PointLight::sample( const Vec3f & p_point ) const { 
		Vec3f direction = glm::normalize( _position - p_point );
		float distance	= glm::distance ( _position,  p_point );
		return LightSample( direction, distance , _color * _power * (1.f / (distance * distance)) , 1.f ); 
	}
} // namespace RT_ISICG