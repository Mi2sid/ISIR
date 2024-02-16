#include "triangle_mesh.hpp"

namespace RT_ISICG
{
	bool MeshTriangle::intersect( const Ray & p_ray,
								  const float p_tMin,
								  const float p_tMax,
								  HitRecord & p_hitRecord ) const
	{
		float  tClosest = p_tMax;			 // Hit distance.
		Vec3f  n		= VEC3F_ZERO;
		size_t hitTri	= _triangles.size(); // Hit triangle id.

		if ( !_aabb.intersect( p_ray, p_tMin, p_tMax ) ) return false;


		for ( size_t i = 0; i < _triangles.size(); i++ )
		{
			float t;
			Vec3f tmp_n;
			if ( _triangles[ i ].intersect( p_ray, t, tmp_n ) )
			{
				if ( t >= p_tMin && t <= tClosest )
				{
					n		 = tmp_n;
					tClosest = t;
					hitTri	 = i;
				}
			}
		}
		if ( hitTri != _triangles.size() ) // Intersection found.
		{
			p_hitRecord._point	= p_ray.pointAtT( tClosest );
			p_hitRecord._normal = _interpolation ? n : _triangles[ hitTri ].getFaceNormal();
			p_hitRecord.faceNormal( p_ray.getDirection() );
			p_hitRecord._distance = tClosest;
			p_hitRecord._object	  = this;

			return true;
		}
		return false;
	}

	bool MeshTriangle::intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		if ( !_aabb.intersect( p_ray, p_tMin, p_tMax) ) return false;
		for ( size_t i = 0; i < _triangles.size(); i++ )
		{
			Vec3f n = VEC3F_ZERO;
			float t;
			if ( _triangles[ i ].intersect( p_ray, t, n ) )
			{
				if ( t >= p_tMin && t <= p_tMax ) return true; // No need to search for the nearest.
			}
		}
		return false;
	}
} // namespace RT_ISICG
