#include "direct_lighting_integrator.hpp"

namespace RT_ISICG
{
	Vec3f DirectLightingIntegrator::Li( const Scene & p_scene,
								 const Ray &   p_ray,
								 const float   p_tMin,
								 const float   p_tMax ) const
	{
		HitRecord hitRecord;
		if ( p_scene.intersect( p_ray, p_tMin, p_tMax, hitRecord ) )
		{
			Vec3f Li = VEC3F_ZERO;
			for ( BaseLight *light : p_scene.getLights() ) {
				LightSample	lightSample = light->sample( hitRecord._point );
				float cosTheta = glm::max( glm::dot( lightSample._direction, hitRecord._normal ), 0.f );
				Li += hitRecord._object->getMaterial()->getFlatColor() * lightSample._radiance * cosTheta;
			}
			
			return Li;
		}
		else { return _backgroundColor; }
	}
} // namespace RT_ISICG
