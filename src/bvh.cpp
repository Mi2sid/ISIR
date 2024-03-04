#include "bvh.hpp"
#include "geometry/triangle_mesh_geometry.hpp"
#include "utils/chrono.hpp"

namespace RT_ISICG
{
	void BVH::build( std::vector<TriangleMeshGeometry> * p_triangles )
	{
		std::cout << "Building BVH..." << std::endl;
		if ( p_triangles == nullptr || p_triangles->empty() )
		{
			throw std::exception( "BVH::build() error: no triangle provided" );
		}
		_triangles = p_triangles;

		Chrono chr;
		chr.start();

		_root = new BVHNode();
		_buildRec( _root, 0, _triangles->size() - 1, 0 );

		chr.stop();

		std::cout << "[DONE]: " << chr.elapsedTime() << "s" << std::endl;
	}

	bool BVH::intersect( const Ray & p_ray, const float p_tMin, const float p_tMax, HitRecord & p_hitRecord ) const
	{
		return _intersectRec( _root, p_ray, p_tMin, p_tMax, p_hitRecord ); 
	}

	bool BVH::intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		return _intersectAnyRec( _root, p_ray, p_tMin, p_tMax );
	}

	void BVH::_buildRec( BVHNode *			p_node,
						 const unsigned int p_firstTriangleId,
						 const unsigned int p_lastTriangleId,
						 const unsigned int p_depth )
	{
		// Calcul aabb noeud
		p_node->_aabb = AABB();
		for ( int i = p_firstTriangleId; i <= p_lastTriangleId; i++ )
			p_node->_aabb.extend( ( *_triangles )[ i ].getAABB() );

		unsigned int nbTriangles = p_lastTriangleId - p_firstTriangleId + 1 ;

		if ( !(p_depth < _maxDepth && nbTriangles > _maxTrianglesPerLeaf) ) {
			// ALLER VOIR FONCTION STD PARTITION <algorithm>
			unsigned int idPartition = _maxAxisPartition( p_node, p_firstTriangleId, p_lastTriangleId );

			p_node->_left = new BVHNode();
			p_node->_right = new BVHNode();

			_buildRec( p_node->_left, p_firstTriangleId, idPartition, p_depth + 1 );
			_buildRec( p_node->_right, idPartition + 1, p_lastTriangleId, p_depth + 1 );
		}
	}



	unsigned int BVH::_maxAxisPartition( BVHNode *			p_node,
										 const unsigned int p_firstTriangleId,
										 const unsigned int p_lastTriangleId )
	{ 
		size_t axisPartition = p_node->_aabb.largestAxis();
		float  middle		 = p_node->_aabb.centroid()[axisPartition]; 
		for (int i = p_firstTriangleId; i <= p_lastTriangleId; i++) if ( ( *_triangles )[ i ].getAABB().getMin()[ axisPartition ] > middle ) return i;
		return p_lastTriangleId;
	}

	bool BVH::_intersectRec( const BVHNode * p_node,
							 const Ray &	 p_ray,
							 const float	 p_tMin,
							 const float	 p_tMax,
							 HitRecord &	 p_hitRecord ) const
	{
		/// TODO
		return false;
	}
	bool BVH::_intersectAnyRec( const BVHNode * p_node,
								const Ray &		p_ray,
								const float		p_tMin,
								const float		p_tMax ) const
	{
		/// TODO
		return false;
	}
} // namespace RT_ISICG
