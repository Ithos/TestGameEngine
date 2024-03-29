#pragma once

#ifndef GEOMETRYMATERIALPOSTPROCESSSTEPINTERFACE_H
#define GEOMETRYMATERIALPOSTPROCESSSTEPINTERFACE_H

#include <map>

namespace GeometryEngine
{
	namespace GeometryMaterial
	{
		class Material;
	}

	namespace CustomShading
	{
		class CustomShadingInterface;
		class MaterialPostProcessInterface;

		/// All posible post process lists
		enum ShadingLists
		{
			PRE_PROCESS_STEP_LIST,
			SECOND_PRE_PROCESS_STEP_LIST,
			POST_PROCESS_STEP_LIST,
			SECOND_POST_PROCESS_STEP_LIST,
			SHADOW_POST_PROCESS_STEP_LIST,
			SECOND_SHADOW_POST_PROCESS_STEP_LIST
		};

		/// Interface and manager class for material post processes maps
		class MultiShadingInterface
		{
		public:
			/// Constructor
			MultiShadingInterface() : mShadingInterface(nullptr) {}
			/// Copy constructor. Automatiaclly initializes the shading steps after copying them.
			/// \param ref Object to be copied.
			MultiShadingInterface(const MultiShadingInterface& ref);
			/// Destructor
			virtual ~MultiShadingInterface();
			/// Abstract method. Factory method. Creates a copy of this object
			/// \return Pointer to a copy of this object
			virtual MultiShadingInterface* Clone() const { return new MultiShadingInterface((*this)); }
			/// Creates a new CustomPostProcessStepInterface and adds it with the selected key if the key does not exist in the map
			/// \param key Key that the new list will have in the map
			/// \return true if the new list was added false otherwise
			bool AddNewList(ShadingLists key);
			/// Removes list from the map
			/// \param key List key to be removed
			/// \return true if the key was removed false otherwise
			bool RemoveList(ShadingLists key);
			/// Checks if the map contains a list step key
			/// \param key Key to check
			/// \return true if the key exists false otherwise
			bool ContainsList(ShadingLists key) { return mListsMap.find(key) != mListsMap.end(); }
			/// Returns the selected list if exists, returns null otherwise
			/// \param key Key to check
			/// \return true if the key exists false otherwise
			MaterialPostProcessInterface* GetList(ShadingLists key);
			/// Sets the target for every list in this interface
			/// \param target Material to be rendered
			void SetTargetMaterial(GeometryMaterial::Material* target);
			/// Calls init on all the shading steps in the map. This method should be called before trying to use any shading step.
			void InitLists();
			/// Sets the shding interface to be used
			void SetShadingInterface(const CustomShadingInterface* const shadingInterface);
			/// Returns the stablished shading interface
			CustomShadingInterface* GetShadingInterface() { return mShadingInterface; }
			/// Calls init on the shading interface. This method should be called before trying to use any shading step.
			void InitShadingInterface();

		protected:
			std::map<ShadingLists, MaterialPostProcessInterface*> mListsMap;
			CustomShadingInterface* mShadingInterface;

			/// Copies the data of a CustomPostProcessStepInterface object to the current object
			/// \param ref Material to be copied
			virtual void copy(const MultiShadingInterface& ref);
		};
	}
}

#endif // !GEOMETRYMATERIALPOSTPROCESSSTEPINTERFACE_H