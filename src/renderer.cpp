#include "renderer.hpp"
#include "integrators/ray_cast_integrator.hpp"
#include "utils/console_progress_bar.hpp"
#include "utils/random.hpp"

namespace RT_ISICG
{
	Renderer::Renderer() { _integrator = new RayCastIntegrator(); }

	void Renderer::setIntegrator( const IntegratorType p_integratorType )
	{
		if ( _integrator != nullptr ) { delete _integrator; }

		switch ( p_integratorType )
		{
		case IntegratorType::RAY_CAST:
		default:
		{
			_integrator = new RayCastIntegrator();
			break;
		}
		}
	}

	void Renderer::setBackgroundColor( const Vec3f & p_color )
	{
		if ( _integrator == nullptr ) { std::cout << "[Renderer::setBackgroundColor] Integrator is null" << std::endl; }
		else
		{
			_integrator->setBackgroundColor( p_color );
		}
	}

	float Renderer::renderImage( const Scene & p_scene, const BaseCamera * p_camera, Texture & p_texture )
	{
		const int width	 = p_texture.getWidth();
		const int height = p_texture.getHeight();

		Chrono			   chrono;
		ConsoleProgressBar progressBar;

		progressBar.start( height, 50 );
		chrono.start();

		for ( int j = 0; j < height; j++ )
		{
			for ( int i = 0; i < width; i++ )
			{
				float x = i + 0.5f, y = j + 0.5f;

				Ray ray = p_camera->generateRay( x / (float)( width - 1 ), y / (float)( height - 1 ) );

				float tMin = 0, tMax = 1000;
				Vec3f color = _integrator->Li( p_scene, ray, tMin, tMax );
				p_texture.setPixel( i, j, color );

				/* 
					p_texture.setPixel( i, j, ( ray.getDirection() + 1.f ) * 0.5f );
				*/
				/*
					float r, g, b = 0.f;
					r = i / (float) (width-1); // (MAX - MIN) * PERCENTAGE + MIN
					g = j / (float) (height-1); // (MAX - MIN) * PERCENTAGE + MIN
					
					p_texture.setPixel( i, j, Vec3f( r, g, b ) );
				*/
			
			}
			progressBar.next();
		}

		chrono.stop();
		progressBar.stop();

		return chrono.elapsedTime();
	}
} // namespace RT_ISICG
