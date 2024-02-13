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
			Vec3f Li = _directLighting( p_scene, p_tMin, p_tMax, hitRecord );
			return Li;
		}
		else { return _backgroundColor; }
	}

	Vec3f DirectLightingIntegrator::_directLighting( const Scene &	 p_scene,
													 const float	 p_tMin,
													 const float     p_tMax,
													 const HitRecord hitRecord ) const
	{
		Vec3f Li = VEC3F_ZERO;
		for ( BaseLight * light : p_scene.getLights() )
		{
			Vec3f luminusContribution = VEC3F_ZERO;
			for ( int i = 0; i < _nbLightSamples; i++ )
			{
				LightSample lightSample = light->sample( hitRecord._point );
				Ray			ray			= Ray( hitRecord._point, lightSample._direction );
				ray.offset( ray.getDirection() );
				if ( !p_scene.intersectAny( ray, p_tMin, lightSample._distance ) )
				{
					float cosTheta = glm::max( glm::dot( lightSample._direction, hitRecord._normal ), 0.f );
					luminusContribution += hitRecord._object->getMaterial()->getFlatColor() * lightSample._radiance * cosTheta;
				}
				if ( !light->getIsSurface() ) { i += _nbLightSamples; }
			}
			luminusContribution
				= light->getIsSurface() ? luminusContribution / (float) _nbLightSamples : luminusContribution;
			Li += luminusContribution;
		}
		return Li;
	}
} // namespace RT_ISICG
