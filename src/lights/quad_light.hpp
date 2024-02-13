#ifndef __RT_ISICG_QUAD_LIGHT__
#define __RT_ISICG_QUAD_LIGHT__

#include "base_light.hpp"
#include "utils/random.hpp"

namespace RT_ISICG
{
	struct QuadLight : public BaseLight
	{
	  public:
		QuadLight( const std::string & p_name,
				   const Vec3f &	   p_position,
				   const Vec3f &	   p_u,
				   const Vec3f &       p_v,
				   const Vec3f &       p_color,
				   const float	       p_power )
			: BaseLight( p_name, p_color, p_power ), _position( p_position ), _u( p_u ), _v( p_v )
		{
			_n = glm::normalize( glm::cross( _u, _v ) );
			_area = glm::length( _u ) * glm::length( _v );
			_isSurface = true;
		}

		virtual LightSample sample( const Vec3f & p_point ) const override;

	  private:
		Vec3f _position = VEC3F_ZERO; // normalized direction toward light source
		Vec3f _u		= VEC3F_ZERO;
		Vec3f _v		= VEC3F_ZERO;
		Vec3f _n		= VEC3F_ZERO;
		float _area		= 0.f;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_QUAS_LIGHT__
