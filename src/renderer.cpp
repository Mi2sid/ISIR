#include "renderer.hpp"
#include "integrators/ray_cast_integrator.hpp"
#include "integrators/direct_lighting_integrator.hpp"
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
		{
			_integrator = new RayCastIntegrator();
			break;
		}
		case IntegratorType::DIRECT_LIGHTING:
		default:
		{
			_integrator = new DirectLightingIntegrator();
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


		#pragma omp parallel for
		for ( int j = 0; j < height; j++ )
		{
			for ( int i = 0; i < width; i++ )
			{
				float x, y;
				Vec3f color = VEC3F_ZERO;

				if ( _nbPixelSamples > 0 )
				{
					x		= i + 0.5f;
					y		= j + 0.5f;
					Ray ray = p_camera->generateRay( x / (float)( width ), y / (float)( height ) );
					color	= _integrator->Li( p_scene, ray, 0, 10000 );
					
					for ( int k = 1; k < _nbPixelSamples; k++ )
					{
						x = i + randomFloat();
						y = j + randomFloat();

						Ray ray = p_camera->generateRay( x / (float)( width ), y / (float)( height ) );
						color += _integrator->Li( p_scene, ray, 0, 10000 );
					}
					color /= (float)_nbPixelSamples;
				}

				p_texture.setPixel( i, j, glm::clamp(color, Vec3f(0.f), Vec3f(1.f) ) );

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
