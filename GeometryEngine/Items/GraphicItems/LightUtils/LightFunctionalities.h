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

	/// Namespace for classes that define specific light functionalities and shading
	namespace LightUtils
	{
		class LightFunction;
		class LightRenderTechnique;

		enum LightFunctions
		{
			STENCIL_TESTING,
			DEFAULT_SHADOWMAP
		};

		enum LightTechniques
		{
			BOUNDING_GEOMETRY
		};

		struct LightFuntionalitiesConstants
		{
			static const std::string POSITION_VERTEX_SHADER;
			static const std::string NULL_FRAGMENT_SHADER;
			static const std::string SHADOW_MAP_FRAGMENT_SHADER;
		};

		class LightFunctionalities
		{
		public:
			LightFunctionalities() : mpShaderManager(nullptr), mpConfInstance(nullptr) {}

			/// Copy constructor. Automatiaclly initializes the shading steps after copying them.
			/// param ref Object to be copied.
			LightFunctionalities(const LightFunctionalities& ref);
			/// Destructor
			virtual ~LightFunctionalities();

			/// Adds a new LightFunction T on the selected function slot
			/// param function Funtion key where the LightFunction object will be added
			template <typename T > LightFunction* AddNewLightFunction(LightFunctions function) { return new T(this, function); }
			/// Adds a new LightFunction on the selected function slot
			/// param value LightFunction to be added to the manager
			/// param key Funtion key where the LightFunction object will be added
			bool AddNewLightFunction(LightFunction* value, LightFunctions key);
			/// Removes a light funtion from the manager
			/// param key Light function key to be removed
			/// returns false if the function is not in the manager true otherwise
			bool RemoveLightFunction(LightFunctions key);
			/// Checks if the manager contains a function key
			/// param key Key to check
			bool ContainsFunction(LightFunctions key) { return mFunctionMap.find(key) != mFunctionMap.end(); }
			/// Calls init on the selected shading step. Steps should be initialited before trying to use them.
			bool InitLightFunction(LightFunctions key);

			/// Adds a new light LightRenderTechnique T on the selected technique slot
			/// param function Technique key where the LightRenderTechnique object will be added
			template <typename T > LightRenderTechnique* AddNewLightTechnique(LightTechniques function) { return new T(this, function); }
			/// Adds a new light LightRenderTechnique on the selected technique slot
			/// param value LightRenderTechnique to be added to the manager
			/// param key Technique key where the LightRenderTechnique object will be added
			bool AddNewLightTechnique(LightRenderTechnique* value, LightTechniques key);
			/// Removes a light technique from the manager
			/// param key Light technique key to be removed
			/// returns false if the technique is not in the manager true otherwise
			bool RemoveLightTechnique(LightTechniques key);
			/// Checks if the manager contains a technique key
			/// param key Key to check
			bool ContainsTechnique(LightTechniques key) { return mTechniqueMap.find(key) != mTechniqueMap.end(); }


			/// Sets the target for every shading step in this interface
			/// param target Light to be set as target
			void SetTargetLight(GeometryWorldItem::GeometryLight::Light* target);
			/// Sets the target for the selected function
			/// param target Light to be set as target
			/// param function Function to be modified
			/// returns false if the function is not in the manager true otherwise
			bool SetTargetLight(GeometryWorldItem::GeometryLight::Light* target, LightFunctions function);
			/// Sets the target for the selected technique
			/// param target Light to be set as target
			/// param technique Technique to be modified
			/// returns false if the function is not in the manager true otherwise
			bool SetTargetLight(GeometryWorldItem::GeometryLight::Light* target, LightTechniques technique);


			/// Apply a light function
			/// param function Function to be applied
			/// param vertexBuf Pointer to the vertex buffer
			/// param indexBuffer Pointer to the index buffer
			/// param projectionMatrix Projection matrix to be used 
			/// param viewMatrix ViewMatrix to be used
			/// param modelMatrix ModelMatrix of the object to be drawn or identity if it's not necessary
			/// param totalVertexNum Total amount of vertices
			/// param totalIndexNum Total amount of indices
			virtual bool ApplyFunction(LightFunctions function, QOpenGLBuffer* vertexBuf, QOpenGLBuffer* indexBuf, const QMatrix4x4& projectionMatrix, const QMatrix4x4& viewMatrix, 
				const QMatrix4x4& modelMatrix, unsigned int totalVertexNum, unsigned int totalIndexNum); 

			/// Apply a light technique. 
			/// param technique Technique to be applied
			/// param projectionMatrix Camera projection matrix
			/// param viewMatrix Camera view matrix
			/// param gBuffInfo Texture info from the camera buffers
			/// param viewPos Position of the camera
			virtual bool ApplyTechnique(LightTechniques technique, const QMatrix4x4& projectionMatrix, const QMatrix4x4& viewMatrix, const BuffersInfo& buffersInfo, const QVector3D& viewPos);

			/// Calls init on all the functionalities in the map. This method should be called before trying to use any functionality.
			void InitFunctions();

			/// Abstract method. Factory method. Creates a copy of this object
			/// return Pointer to a copy of this object
			virtual LightFunctionalities* Clone() const { return new LightFunctionalities((*this)); }

		protected:
			ShaderFiles::ShaderManager* mpShaderManager;
			Configuration::ConfigurationManager* mpConfInstance;

			std::map<LightFunctions, LightFunction*> mFunctionMap;
			std::map<LightTechniques, LightRenderTechnique*> mTechniqueMap;

			/// Initializes managers and shaders
			virtual void getManagers();
			/// Copies the data of a Material object to the current object
			/// param ref Material to be copied
			virtual void copy(const LightFunctionalities& ref);
		};
	}
}
#endif