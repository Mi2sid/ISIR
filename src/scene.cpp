#include "scene.hpp"
#include "materials/color_material.hpp"
#include "objects/sphere.hpp"
#include "objects/plane.hpp"
#include "objects/triangle_mesh.hpp"
#include "lights/point_light.hpp"
#include "lights/quad_light.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace RT_ISICG
{
	Scene::Scene() { _addMaterial( new ColorMaterial( "default", WHITE ) ); }

	Scene::~Scene()
	{
		for ( const ObjectMapPair & object : _objectMap )
		{
			delete object.second;
		}
		for ( const MaterialMapPair & material : _materialMap )
		{
			delete material.second;
		}
		for ( const BaseLight * light : _lightList )
		{
			delete light;
		}
	}

	void Scene::init()
	{
		switch ( _version )
		{
		case 1: _initScene1(); break;
		case 2: _initScene2(); break;
		case 3: _initScene3(); break;
		case 4: _initScene4(); break;
		case 5:
		default: _initScene5(); break;
		}
	}

	void Scene::_initScene1() {
		// Add Object
		_addObject( new Sphere( "Sphere1", Vec3f( 0.f, 0.f, 3.f ), 1.f ) );
		
		// Add Material
		_addMaterial( new ColorMaterial( "Blue", BLUE ) );
		
		// Attach Material - Object
		_attachMaterialToObject( "Blue", "Sphere1" );

	}

	void Scene::_initScene2() {
		// Blue Sphere
		_addObject( new Sphere( "Sphere1", Vec3f( 0.f, 0.f, 3.f ), 1.f ) );
		_addMaterial( new ColorMaterial( "Blue", BLUE ) );
		_attachMaterialToObject( "Blue", "Sphere1" );

		// Red Plane
		_addObject( new Plane( "Plane1", Vec3f( 0.f, -2.f, 0.f ), Vec3f( 0.f, 1.f, 0.f ) ) );
		_addMaterial( new ColorMaterial( "Red", RED ) );
		_attachMaterialToObject( "Red", "Plane1" );

		_addLight( new PointLight( "Light1", Vec3f( 1.f, 10.f, 1.f ), WHITE, 100.f ) );
	}

	void Scene::_initScene3()
	{
		// Blue Sphere
		_addObject( new Sphere( "Sphere1", Vec3f( 0.f, 0.f, 3.f ), 1.f ) );
		_addMaterial( new ColorMaterial( "Blue", BLUE ) );
		_attachMaterialToObject( "Blue", "Sphere1" );

		// Red Plane
		_addObject( new Plane( "Plane1", Vec3f( 0.f, -2.f, 0.f ), Vec3f( 0.f, 1.f, 0.f ) ) );
		_addMaterial( new ColorMaterial( "Red", RED ) );
		_attachMaterialToObject( "Red", "Plane1" );

		_addLight( new QuadLight(
			"Light1", Vec3f( 1.f, 10.f, 2.f ), Vec3f( -2.f, 0.f, 0.f ), Vec3f( 0.f, 0.f, 2.f ), WHITE, 40.f ) );
	}

	void Scene::_initScene4() { 
		// ================================================================
		// Add materials .
		// ================================================================
		_addMaterial( new ColorMaterial( "RedColor", RED ) );
		_addMaterial( new ColorMaterial( "GreenColor", GREEN ) );
		_addMaterial( new ColorMaterial( "BlueColor", BLUE ) );
		_addMaterial( new ColorMaterial( "GreyColor", GREY ) );
		_addMaterial( new ColorMaterial( "MagentaColor", MAGENTA ) );
		_addMaterial( new ColorMaterial( "YellowColor", YELLOW ) );
		_addMaterial( new ColorMaterial( "CyanColor", CYAN ) );
		// ================================================================
		// Add objects .
		// ================================================================
		// OBJ.
		loadFileTriangleMesh( "Bunny", "data/Bunny.obj" );
		_attachMaterialToObject( "CyanColor", "Bunny_defaultobject" );
		// Pseudo Cornell box made with infinite planes .
		_addObject( new Plane( "PlaneGround", Vec3f( 0.f, -3.f, 0.f ), Vec3f( 0.f, 1.f, 0.f ) ) );
		_attachMaterialToObject( "GreyColor", "PlaneGround" );
		_addObject( new Plane( "PlaneLeft", Vec3f( 5.f, 0.f, 0.f ), Vec3f( -1.f, 0.f, 0.f ) ) );
		_attachMaterialToObject( "RedColor", "PlaneLeft" );
		_addObject( new Plane( "PlaneCeiling", Vec3f( 0.f, 7.f, 0.f ), Vec3f( 0.f, -1.f, 0.f ) ) );
		_attachMaterialToObject( "GreenColor", "PlaneCeiling" );
		_addObject( new Plane( "PlaneRight", Vec3f( -5.f, 0.f, 0.f ), Vec3f( 1.f, 0.f, 0.f ) ) );
		_attachMaterialToObject( "BlueColor", "PlaneRight" );
		_addObject( new Plane( "PlaneFront", Vec3f( 0.f, 0.f, 10.f ), Vec3f( 0.f, 0.f, -1.f ) ) );
		_attachMaterialToObject( "MagentaColor", "PlaneFront" );
		_addObject( new Plane( "PlaneRear", Vec3f( 0.f, 0.f, -10.f ), Vec3f( 0.f, 0.f, 1.f ) ) );
		_attachMaterialToObject( "YellowColor", "PlaneRear" );
		// ================================================================
		// Add lights .
		// ================================================================
		_addLight( new PointLight( "Light1",  Vec3f( 0.f, 3.f, -5.f ), WHITE, 100.f ) );

	}

	void Scene::_initScene5()
	{
		loadFileTriangleMesh( "Conference", "data/conference/conference.obj" );
		_addLight( new QuadLight( "Light1",
								  Vec3f( 900.f, 600.f, -300.f ),
								  Vec3f( -800.f, 0.f, 0.f ),
								  Vec3f( 0.f, 0.f, 300.f ),
								  WHITE,
								  20.f ) );
	}


	void Scene::loadFileTriangleMesh( const std::string & p_name, const std::string & p_path )
	{
		std::cout << "Loading: " << p_path << std::endl;
		Assimp::Importer importer;

		// Read scene and triangulate meshes
		const aiScene * const scene
			= importer.ReadFile( p_path, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_GenUVCoords );

		if ( scene == nullptr ) { throw std::runtime_error( "Fail to load file: " + p_path ); }

		unsigned int cptTriangles = 0;
		unsigned int cptVertices  = 0;

		for ( unsigned int m = 0; m < scene->mNumMeshes; ++m )
		{
			const aiMesh * const mesh = scene->mMeshes[ m ];
			if ( mesh == nullptr ) { throw std::runtime_error( "Fail to load file: " + p_path + ": mesh is null" ); }

			const std::string meshName = p_name + "_" + std::string( mesh->mName.C_Str() );
			std::cout << "-- Load mesh " << m + 1 << "/" << scene->mNumMeshes << ": " << meshName << std::endl;

			cptTriangles += mesh->mNumFaces;
			cptVertices += mesh->mNumVertices;

			const bool hasUV = mesh->HasTextureCoords( 0 );

			MeshTriangle * triMesh = new MeshTriangle( meshName );
			// Vertices before faces otherwise face normals cannot be computed.
			for ( unsigned int v = 0; v < mesh->mNumVertices; ++v )
			{
				triMesh->addVertex( mesh->mVertices[ v ].x, mesh->mVertices[ v ].y, mesh->mVertices[ v ].z );
				triMesh->addNormal( mesh->mNormals[ v ].x, mesh->mNormals[ v ].y, mesh->mNormals[ v ].z );
				if ( hasUV ) triMesh->addUV( mesh->mTextureCoords[ 0 ][ v ].x, mesh->mTextureCoords[ 0 ][ v ].y );
			}
			for ( unsigned int f = 0; f < mesh->mNumFaces; ++f )
			{
				const aiFace & face = mesh->mFaces[ f ];
				triMesh->addTriangle( face.mIndices[ 0 ], face.mIndices[ 1 ], face.mIndices[ 2 ] );
			}


			triMesh->createBVH();
			_addObject( triMesh );

			const aiMaterial * const mtl = scene->mMaterials[ mesh->mMaterialIndex ];
			if ( mtl == nullptr )
			{
				std::cerr << "Material undefined," << meshName << " assigned to default material" << std::endl;
			}
			else
			{
				Vec3f kd = WHITE;
				Vec3f ks = BLACK;
				float s	 = 0.f;

				aiColor3D aiKd;
				if ( mtl->Get( AI_MATKEY_COLOR_DIFFUSE, aiKd ) == AI_SUCCESS ) kd = Vec3f( aiKd.r, aiKd.g, aiKd.b );
				aiColor3D aiKs;
				if ( mtl->Get( AI_MATKEY_COLOR_SPECULAR, aiKs ) == AI_SUCCESS ) ks = Vec3f( aiKs.r, aiKs.g, aiKs.b );
				float aiS = 0.f;
				if ( mtl->Get( AI_MATKEY_SHININESS, aiS ) == AI_SUCCESS ) s = aiS;
				aiString mtlName;
				mtl->Get( AI_MATKEY_NAME, mtlName );

				_addMaterial( new ColorMaterial( std::string( mtlName.C_Str() ), kd ) );
				_attachMaterialToObject( mtlName.C_Str(), meshName );
			}

			std::cout << "-- [DONE] " << triMesh->getNbTriangles() << " triangles, " << triMesh->getNbVertices()
					  << " vertices." << std::endl;
		}
		std::cout << "[DONE] " << scene->mNumMeshes << " meshes, " << cptTriangles << " triangles, " << cptVertices
				  << " vertices." << std::endl;
	}

	bool Scene::intersect( const Ray & p_ray, const float p_tMin, const float p_tMax, HitRecord & p_hitRecord ) const
	{
		float tMax = p_tMax;
		bool  hit  = false;
		for ( const ObjectMapPair & object : _objectMap )
		{
			if ( object.second->intersect( p_ray, p_tMin, tMax, p_hitRecord ) )
			{
				tMax = p_hitRecord._distance; // update tMax to conserve the nearest hit
				hit	 = true;
			}
		}
		return hit;
	}

	bool Scene::intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		for ( const ObjectMapPair & object : _objectMap )
		{
			if ( object.second->intersectAny( p_ray, p_tMin, p_tMax ) ) { return true; }
		}
		return false;
	}

	void Scene::_addObject( BaseObject * p_object )
	{
		const std::string & name = p_object->getName();
		if ( _objectMap.find( name ) != _objectMap.end() )
		{
			std::cout << "[Scene::addObject] Object \'" << name << "\' already exists" << std::endl;
			delete p_object;
		}
		else
		{
			_objectMap[ name ] = p_object;
			_objectMap[ name ]->setMaterial( _materialMap[ "default" ] );
		}
	}

	void Scene::_addMaterial( BaseMaterial * p_material )
	{
		const std::string & name = p_material->getName();
		if ( _materialMap.find( name ) != _materialMap.end() )
		{
			std::cout << "[Scene::_addMaterial] Material \'" << name << "\' already exists" << std::endl;
			delete p_material;
		}
		else
		{
			std::cout << "Material \'" << name << "\' added." << std::endl;
			_materialMap[ name ] = p_material;
		}
	}

	void Scene::_addLight( BaseLight * p_light ) { _lightList.emplace_back( p_light ); }

	void Scene::_attachMaterialToObject( const std::string & p_materialName, const std::string & p_objectName )
	{
		if ( _objectMap.find( p_objectName ) == _objectMap.end() )
		{
			std::cout << "[Scene::attachMaterialToObject] Object \'" << p_objectName << "\' does not exist"
					  << std::endl;
		}
		else if ( _materialMap.find( p_materialName ) == _materialMap.end() )
		{
			std::cout << "[Scene::attachMaterialToObject] Material \'" << p_materialName << "\' does not exist, "
					  << "object \'" << p_objectName << "\' keeps its material \'"
					  << _objectMap[ p_objectName ]->getMaterial()->getName() << "\'" << std::endl;
		}
		else
		{
			_objectMap[ p_objectName ]->setMaterial( _materialMap[ p_materialName ] );
		}
	}

} // namespace RT_ISICG
