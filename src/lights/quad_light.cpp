#include "quad_light.hpp"

namespace RT_ISICG
{
	LightSample QuadLight::sample( const Vec3f & p_point ) const
	{
		Vec3f randomPos = _position + _u * randomFloat() + _v * randomFloat();
		Vec3f direction = glm::normalize( randomPos - p_point );
		float distance	= glm::distance( randomPos, p_point );

		float pdf = ( 1.f/_area ) * ( ( distance * distance ) / ( glm::dot( _n, direction ) ) ) ;

		Vec3f radiance = ( _color * _power ) / pdf;

		return LightSample( direction, distance, radiance, pdf);
	}
} // namespace RT_ISICG