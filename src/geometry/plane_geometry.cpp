#include "plane_geometry.hpp"

namespace RT_ISICG
{
	bool PlaneGeometry::intersect( const Ray & p_ray, float & p_t1 ) const
	{
		
		float den = glm::dot( _normal, p_ray.getDirection() );

		if ( den != 0 )
		{ 
			Vec3f  po = _point - p_ray.getOrigin();
			float t	 = glm::dot( po, _normal ) / den;
			if ( t > 0 ) {
				p_t1 = t;
				return true;
			}
		}

		p_t1 = -1.f;
		return false;
	}

} // namespace RT_ISICG
