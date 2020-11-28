#pragma once

#ifndef GEOMETRYLIGHTFUNCTIONALITIES_H
#define GEOMETRYLIGHTFUNCTIONALITIES_H

#include <map>

#include <qmatrix4x4.h>
#include <qvector3d.h>
#include <QOpenGLBuffer>

#include <ConfigurationManager.h>
#include <ShaderManager.h>

namespace GeometryEngine
{
	namespace GeometryWorldItem
	{
		namespace GeometryLight
		{
			class Light;
		}
	}

	class BuffersInfo;

	/// Namespace for classes that define specific light components, functionalities and shading
	namespace LightUtils
	{
		class LightShadingComponent;
		class LightRenderComponent;

		/// All posible shading steps to override
		enum LightShading
		{
			STENCIL_TESTING,
			DEFAULT_SHADOWMAP
		};

		/// All posible render techniques
		enum LightRender
		{
			BOUNDING_GEOMETRY
		};

		/// String constants thet reference shader keys
		struct LightFuntionalitiesConstants
		{
			static const std::string POSITION_VERTEX_SHADER;
			static const std::string NULL_FRAGMENT_SHADER;
			static const std::string SHADOW_MAP_FRAGMENT_SHADER;
		};


		/// Class that manages light components and acts as an interface for them
		class LightComponentManager
		{
		public:
			/// Constructor
			LightComponentManager() : mpShaderManager(nullptr), mpConfInstance(nullptr) {}

			/// Copy constructor. Automatiaclly initializes the shading steps after copying them.
			/// \param ref Object to be copied.
			LightComponentManager(const LightComponentManager& ref);
			/// Destructor
			virtual ~LightComponentManager();

			/// Adds a new LightShadingComponent T on the selected ShadingComponent slot
			/// \param component LightShading key where the LightShadingComponent object will be added
			template <typename T > LightShadingComponent* AddNewLightShadingComponent(LightShading component) { return new T(this, component); }
			/// Adds a new LightShadingComponent on the selected ShadingComponent slot
			/// \param value LightShadingComponent to be added to the manager
			/// \param key LightShading key where the LightShadingComponent object will be added
			bool AddNewLightShadingComponent(LightShadingComponent* value, LightShading key);
			/// Removes a LightShadingComponent from the manager
			/// \param key LightShading key to be removed
			/// Returns false if the LightShading key is not in the manager true otherwise
			bool RemoveLightShadingComponent(LightShading key);
			/// Checks if the manager contains a LightShading key
			/// \param key Key to check
			bool ContainsLightShadingComponent(LightShading key) { return mShadingMap.find(key) != mShadingMap.end(); }
			/// Calls init on the selected shading step. Steps should be initialited before trying to use them.
			bool InitLightShadingComponent(LightShading key);

			/// Adds a new LightRenderComponent T on the selected LightRender slot
			/// \param component LightRender key where the LightRenderComponent object will be added
			template <typename T > LightRenderComponent* AddNewLightRenderComponent(LightRender component) { return new T(this, component); }
			/// Adds a new LightRenderComponent on the selected LightRender slot
			/// \param value LightRenderComponent to be added to the manager
			/// \param key LightRender key where the LightRenderComponent object will be added
			bool AddNewLightRenderComponent(LightRenderComponent* value, LightRender key);
			/// Removes a LightRenderComponent from the manager
			/// \param key LightRender key to be removed
			/// Returns false if the LightRender key is not in the manager true otherwise
			bool RemoveLightRenderComponent(LightRender key);
			/// Checks if the manager contains a LightRender key
			/// \param key LightRender key to check
			bool ContainsLightRenderComponent(LightRender key) { return mRenderMap.find(key) != mRenderMap.end(); }


			/// Sets the target for every shading step in this interface
			/// \param target Light to be set as target
			void SetTargetLight(GeometryWorldItem::GeometryLight::Light* target);
			/// Sets the target for the selected function
			/// \param target Light to be set as target
			/// \param key Component key to be modified
			/// Returns false if the function is not in the manager true otherwise
			bool SetTargetLight(GeometryWorldItem::GeometryLight::Light* target, LightShading key);
			/// Sets the target for the selected technique
			/// \param target Light to be set as target
			/// \param key Component key to be modified
			/// Returns false if the key is not in the manager true otherwise
			bool SetTargetLight(GeometryWorldItem::GeometryLight::Light* target, LightRender key);


			/// Execute a light shading component
			/// \param component Component key to be executed
			/// \param vertexBuf Pointer to the vertex buffer
			/// \param indexBuffer Pointer to the index buffer
			/// \param projectionMatrix Projection matrix to be used 
			/// \param viewMatrix ViewMatrix to be used
			/// \param modelMatrix ModelMatrix of the object to be drawn or identity if it's not necessary
			/// \param totalVertexNum Total amount of vertices
			/// \param totalIndexNum Total amount of indices
			virtual bool ApplyLightShading(LightShading component, QOpenGLBuffer* vertexBuf, QOpenGLBuffer* indexBuf, const QMatrix4x4& projectionMatrix, const QMatrix4x4& viewMatrix, 
				const QMatrix4x4& modelMatrix, unsigned int totalVertexNum, unsigned int totalIndexNum); 

			/// Execute a light shading component			
			/// \param component Component key to be executed
			/// \param projectionMatrix Camera projection matrix
			/// \param viewMatrix Camera view matrix
			/// \param gBuffInfo Texture info from the camera buffers
			/// \param viewPos Position of the camera
			virtual bool ApplyLightRender(LightRender component, const QMatrix4x4& projectionMatrix, const QMatrix4x4& viewMatrix, const BuffersInfo& buffersInfo, const QVector3D& viewPos);

			/// Calls init on all the functionalities in the map. This method should be called before trying to use any functionality.
			void InitFunctions();

			/// Abstract method. Factory method. Creates a copy of this object
			/// \return Pointer to a copy of this object
			virtual LightComponentManager* Clone() const { return new LightComponentManager((*this)); }

		protected:
			ShaderFiles::ShaderManager* mpShaderManager;
			Configuration::ConfigurationManager* mpConfInstance;

			std::map<LightShading, LightShadingComponent*> mShadingMap;
			std::map<LightRender, LightRenderComponent*> mRenderMap;

			/// Initializes managers and shaders
			virtual void getManagers();
			/// Copies the data of a Material object to the current object
			/// \param ref Material to be copied
			virtual void copy(const LightComponentManager& ref);
		};
	}
}
#endif