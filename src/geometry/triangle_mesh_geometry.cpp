#include "triangle_mesh_geometry.hpp"
#include "objects/triangle_mesh.hpp"

namespace RT_ISICG
{
	TriangleMeshGeometry::TriangleMeshGeometry( const unsigned int p_v0,
												const unsigned int p_v1,
												const unsigned int p_v2,
												MeshTriangle *	   p_refMesh )
		: _v0( p_v0 ), _v1( p_v1 ), _v2( p_v2 ), _refMesh( p_refMesh )
	{
		_faceNormal = glm::normalize( glm::cross( _refMesh->_vertices[ p_v1 ] - _refMesh->_vertices[ p_v0 ],
												  _refMesh->_vertices[ p_v2 ] - _refMesh->_vertices[ p_v0 ] ) );
	}

	bool TriangleMeshGeometry::intersect( const Ray & p_ray, float & p_t, Vec3f & p_n ) const
	{
		const Vec3f & o	 = p_ray.getOrigin();
		const Vec3f & d	 = p_ray.getDirection();

		const Vec3f & v0 = _refMesh->_vertices[ _v0 ];
		const Vec3f & v1 = _refMesh->_vertices[ _v1 ];
		const Vec3f & v2 = _refMesh->_vertices[ _v2 ];

		const Vec3f & n0 = _refMesh->_normals[ _v0 ];
		const Vec3f & n1 = _refMesh->_normals[ _v1 ];
		const Vec3f & n2 = _refMesh->_normals[ _v2 ];

		p_t = -1.f;

		constexpr float epsilon = std::numeric_limits<float>::epsilon();

		Vec3f a = v1 - v0;
		Vec3f b = v2 - v0;


		Vec3f v_d = glm::cross( d, b );
		float cos = glm::dot( a, v_d ); // si le cos est proche de 0 alors on a un angle droit

		// test rayon parallèle
		if ( glm::abs( cos ) < epsilon ) return false;

		float inv_cos = 1.f / cos;
		Vec3f v0o	  = o - v0;
		float u		  = inv_cos * glm::dot( v0o, v_d );

		if ( u < 0 || u > 1 ) return false;

		Vec3f v0o_a = glm::cross( v0o, a );
		float v		= inv_cos * dot( d, v0o_a );

		if ( v < 0 || u + v > 1 ) return false;

		p_t = inv_cos * glm::dot( b, v0o_a );

		p_n = ( 1 - u - v ) * n0 + u * n1 + v * n2; 
		if ( p_t > epsilon ) return true;

		p_t = -1.f;
		return false;
	}

} // namespace RT_ISICG
