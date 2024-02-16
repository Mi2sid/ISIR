#include "aabb.hpp"

namespace RT_ISICG
{
	bool AABB::intersect( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		Vec3f o = p_ray.getOrigin();
		Vec3f d = p_ray.getDirection();
		
		float tMin = p_tMin; 
		float tMax = p_tMax;

		if ( d.x != 0.f )
		{
			float txMin = ( _min.x - o.x ) / d.x;
			float txMax = ( _max.x - o.x ) / d.x;

			if ( txMin > txMax ) std::swap( txMin, txMax );

			tMin = glm::max( tMin, txMin );
			tMax = glm::min( tMax, txMax );
		}

		if ( d.y != 0.f )
		{
			float tyMin = ( _min.y - o.y ) / d.y;
			float tyMax = ( _max.y - o.y ) / d.y;

			if ( tyMin > tyMax ) std::swap( tyMin, tyMax );

			tMin = glm::max( tMin, tyMin );
			tMax = glm::min( tMax, tyMax );
		}
		
		if ( d.z != 0.f )
		{
			float tzMin = ( _min.z - o.z ) / d.z;
			float tzMax = ( _max.z - o.z ) / d.z;

			if ( tzMin > tzMax ) std::swap( tzMin, tzMax );

			tMin = glm::max( tMin, tzMin );
			tMax = glm::min( tMax, tzMax );
		}

		if ( tMin > tMax ) return false;
		return true;

	}
} // namespace RT_ISICG
