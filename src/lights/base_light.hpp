#ifndef __RT_ISICG_BASE_LIGHT__
#define __RT_ISICG_BASE_LIGHT__

#include "light_sample.hpp"

namespace RT_ISICG
{
	class BaseLight
	{
	  public:
		BaseLight( const std::string & p_name, const Vec3f & p_color, const float p_power = 1.f )
			: _name( p_name ), _color( p_color ), _power( p_power )
		{
		}
		virtual ~BaseLight() = default;

		virtual const std::string & getName() const final { return _name; }

		inline const Vec3f & getFlatColor() const { return _color; }

		virtual LightSample sample( const Vec3f & p_point ) const = 0;

	  protected:
		const std::string _name;
		Vec3f _color = WHITE;
		float _power = 1.f;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_BASE_LIGHT__
