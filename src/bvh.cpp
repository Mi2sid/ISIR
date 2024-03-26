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
		_buildRec( _root, 0, _triangles->size(), 0 );

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
		for ( int i = p_firstTriangleId; i < p_lastTriangleId; i++ )
			p_node->_aabb.extend( ( *_triangles )[ i ].getAABB() );

		p_node->_firstTriangleId = p_firstTriangleId;
		p_node->_lastTriangleId	 = p_lastTriangleId;

		unsigned int nbTriangles = p_lastTriangleId - p_firstTriangleId ;

		if ( p_depth < _maxDepth && nbTriangles > _maxTrianglesPerLeaf ) {

			// Partitions function
			size_t axisPartition = p_node->_aabb.largestAxis();
			Vec3f middle		 = p_node->_aabb.centroid();
			std::function<bool( TriangleMeshGeometry )> largestAxisSplit
				= [ middle, axisPartition ]( TriangleMeshGeometry triangle ) -> bool
			{
				float a = triangle.getAABB().getMin()[ axisPartition ];
				float b = middle[ axisPartition ];

				return a <= b;
			};
			unsigned int idPartition = std::distance( _triangles->begin(), 
														std::partition(
															_triangles->begin() + p_firstTriangleId,
															_triangles->begin() + p_lastTriangleId - 1,
															largestAxisSplit
														)
													);
			p_node->_left = new BVHNode();
			p_node->_right = new BVHNode();

			_buildRec( p_node->_left, p_firstTriangleId, idPartition, p_depth + 1 );
			_buildRec( p_node->_right, idPartition + 1, p_lastTriangleId, p_depth + 1 );
		}
	}
	/*
	unsigned int BVH::_maxAxisPartition( BVHNode *			p_node,
										 const unsigned int p_firstTriangleId,
										 const unsigned int p_lastTriangleId )
	{ 
		size_t axisPartition = p_node->_aabb.largestAxis();
		float  middle		 = p_node->_aabb.centroid()[axisPartition]; 
		for (int i = p_firstTriangleId; i <= p_lastTriangleId; i++) if ( ( *_triangles )[ i ].getAABB().getMin()[ axisPartition ] > middle ) return i;
		return p_lastTriangleId;
	}*/

	bool BVH::_intersectRec( const BVHNode * p_node,
							 const Ray &	 p_ray,
							 const float	 p_tMin,
							 const float	 p_tMax,
							 HitRecord &	 p_hitRecord ) const
	{
		if ( !p_node->_aabb.intersect( p_ray, p_tMin, p_tMax ) ) { return false; } 

		if ( !p_node->isLeaf() ) {
			if ( p_node->_left != nullptr ) _intersectRec( p_node->_left, p_ray, p_tMin, p_tMax, p_hitRecord );
			if ( p_node->_right != nullptr ) _intersectRec( p_node->_right, p_ray, p_tMin, p_tMax, p_hitRecord );
			return p_hitRecord._object != nullptr;
		}
		for ( size_t i = p_node->_firstTriangleId; i < p_node->_lastTriangleId; i++ )
		{
			float t;
			Vec3f normal;
			if ( _triangles->at( i ).intersect( p_ray, t, normal ) )
			{
				if ( p_tMin <= t && t <= p_tMax )
				{
					if ( p_hitRecord._object != nullptr && t > p_hitRecord._distance )
						continue;
	
					TriangleMeshGeometry triangle = _triangles->at( i );
					p_hitRecord._point		 = p_ray.pointAtT( t );
					p_hitRecord._normal		 = normal;
					p_hitRecord.faceNormal( p_ray.getDirection() );
					p_hitRecord._distance = t;
					p_hitRecord._object	  = (BaseObject *)( triangle.getMesh() );
				}
			}
			return p_hitRecord._object != nullptr;
		}
	}

	bool BVH::_intersectAnyRec( const BVHNode * p_node,
								const Ray &		p_ray,
								const float		p_tMin,
								const float		p_tMax ) const
	{
		if ( !p_node->_aabb.intersect( p_ray, p_tMin, p_tMax ) ) { return false; }

		if ( !p_node->isLeaf() )
		{
			if ( p_node->_left != nullptr ) { 
				if ( _intersectAnyRec( p_node->_left, p_ray, p_tMin, p_tMax ) ) return true;
			}
			if ( p_node->_right != nullptr ) { 
				if( _intersectAnyRec( p_node->_right, p_ray, p_tMin, p_tMax ) ) return true; 
			}
			return false;
		}
		for ( size_t i = p_node->_firstTriangleId; i < p_node->_lastTriangleId; i++ )
		{
			float t;
			Vec3f normal;
			if ( _triangles->at( i ).intersect( p_ray, t, normal ) )
			{
				if ( p_tMin <= t && t <= p_tMax ) return true;
			}
			return false;
		}
	}
} // namespace RT_ISICG
