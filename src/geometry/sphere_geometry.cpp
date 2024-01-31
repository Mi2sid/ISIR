#include "sphere_geometry.hpp"

namespace RT_ISICG
{
	bool SphereGeometry::intersect( const Ray & p_ray, float & p_t1, float & p_t2 ) const
	{

		Vec3f oc	= p_ray.getOrigin() - _center;
		float b		= 2 * glm::dot( oc, p_ray.getDirection() );
		float c		= glm::dot( oc, oc ) - _radius * _radius;
		float delta = b * b - 4 * c;

		if (delta >= 0) {
			float t = ( -b - sqrt( delta ) ) / 2;
			p_t1 =  t;

			float u = ( -b + sqrt( delta ) ) / 2;
			p_t2 = u;

			return true;
		}

		p_t1 = -1.f;
		p_t2 = -1.f;
		return false;
	}

} // namespace RT_ISICG
