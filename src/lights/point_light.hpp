#ifndef __RT_ISICG_POINT_LIGHT__
#define __RT_ISICG_POINT_LIGHT__

#include "base_light.hpp"

namespace RT_ISICG
{
	struct PointLight : public BaseLight
	{
	  public:
		PointLight( const std::string & p_name, const Vec3f & p_position, const Vec3f & p_color, const float p_power )
			: BaseLight( p_name, p_color, p_power ), _position( p_position )
		{
		}

		virtual LightSample sample( const Vec3f & p_point ) const override;

	  private:
		Vec3f _position = VEC3F_ZERO; // normalized direction toward light source
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_POINT_LIGHT__
