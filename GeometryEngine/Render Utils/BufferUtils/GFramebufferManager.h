#pragma once

#ifndef G_FRAMEBUFFERMANAGER
#define G_FRAMEBUFFERMANAGER

#include <unordered_set>

namespace GeometryEngine
{
	namespace GeometryBuffer
	{
		class GFramebufferObject;
		/// Singleton that manages which framebuffer is attached and detaches all that aren't
		class GFramebufferManager
		{
		public:
			/// Returns the pointer to the singleton. It builds the object if the singleton hasn't been built before.
			static GFramebufferManager* GetInstance();
			/// Destroys the pointer to the singleton.
			static void Release();
			/// Adds a FBO to the manager. Does nothing if the FBO was already included. It isn't necessary to call this method outside the manager or the FBO itself.
			/// param fbo Pointer to the framebuffer to add
			void AddFramebuffer(GFramebufferObject* fbo);
			/// Deletes a framebuffer and removes it from the manager. The framebuffer standard destructor also removes it from the manager for safety.
			/// param fbo Pointer to the fbo to be removed
			void DeleteFramebuffer(GFramebufferObject* fbo);
			/// Sets a new active framebuffer and sets all other framebuffers as detached.
			/// param fbo Pointer to the framebuffer to be set as active
			void SetFramebufferActive(GFramebufferObject* fbo);
			/// Factory method. Creates a new framebuffer
			/// return pointer to a new framebuffer
			GFramebufferObject* GetNewFramebuffer();
		private:
			GFramebufferManager();
			virtual ~GFramebufferManager();

			/// Deinitializes the object.
			void close();
			/// Builds the manager
			void init();

			static GFramebufferManager* mpInstance;
			std::unordered_set<GFramebufferObject*> mFramebufferList;
			GFramebufferObject* mpActiveFramebuffer;
		};
	}
}


#endif