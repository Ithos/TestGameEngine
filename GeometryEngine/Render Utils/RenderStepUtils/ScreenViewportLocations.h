#pragma once
#ifndef SCREENVIEWPORTLOCATION_H
#define SCREENVIEWPORTLOCATION_H

#include <map>
#include <qvector4d.h>
#include "../Items/GraphicItems/CameraUtils/CameraCommons.h"

namespace GeometryEngine
{
	namespace GeometryRenderStep
	{
		/// Class that holds the locations and sizes for multiple screen viewports and their associated camera targets. 
		/// QVector4D values must be in the [0;1] range, they indicate screen size percentages x,y -> viewport screen position z,w -> viewport width and height
		class ScreenViewportLocation
		{
		public:
			/// Constructor
			/// \param viewportNumber Number of viewports in the screen 
			ScreenViewportLocation(int viewportNumber);
			/// Constructor
			/// \param viewportMap Map of viewport sizes expressed as (position, size) in widow size percentages, values will be clamped to the [0,1] range 
			ScreenViewportLocation(const std::map<GeometryWorldItem::GeometryCamera::CameraTargets, QVector4D>& viewportMap);
			/// Copy constructor
			/// \param ref Reference to the object to be copied
			ScreenViewportLocation(const ScreenViewportLocation& ref) { copy(ref); }
			/// Destructor
			virtual ~ScreenViewportLocation();
			/// Returns the size of the selected viewport
			/// \param target Key of the selected viewport
			/// \return size of the selected viewport if it exists.If the viewport doesnt exist QVector4D(-1, -1, -1, -1) is return ed instead.
			const QVector4D& GetViewportSize(const GeometryWorldItem::GeometryCamera::CameraTargets& target);
			/// Helper method that transforms the size vector to a Quad position vector that contains the position for the viewport verteces in window coordinates
			/// \param screenSize Size of the screen as (position, size)
			/// \param viewportSize Size of the viewport as (position, size) expressed as percentages of the screen size
			/// \return Vertex positions of the viewport
			static QVector4D GetQuadVertexPositions(const QVector4D& screenSize, const QVector4D& viewportSize);
			/// Checks if the viewport key exists
			/// \param target Selected viewport key
			/// \return true if the map contains the key, false otherwise
			bool ContainsViewport(const GeometryWorldItem::GeometryCamera::CameraTargets& target);
			/// Adds a new viewport size to the map
			/// \param target New viewport key
			/// \param viewportSize Size of the new viewport
			/// \return true if the map didn't contain the viewport key false otherwise
			bool AddViewport(GeometryWorldItem::GeometryCamera::CameraTargets target, QVector4D viewportSize);
			/// Modifies the size of an existing viewport
			/// \param target Selected viewport key
			/// \param viewportSize New size of the viewport
			/// \return true if the map contains the viewport key false otherwise
			bool ModifyViewport(GeometryWorldItem::GeometryCamera::CameraTargets target, QVector4D viewportSize);
			/// Removes an existing viewport from the map
			/// \param target Selected viewport key
			/// \return true if the map contained the viewport key false otherwise
			bool RemoveViewport(const GeometryWorldItem::GeometryCamera::CameraTargets& target);
			/// Factory method. Returns a copy of this object.
			/// \return A copy of this object.
			virtual ScreenViewportLocation* Clone() const { return new ScreenViewportLocation(*this); }
		protected:
			std::map<GeometryWorldItem::GeometryCamera::CameraTargets, QVector4D> mViewportSize;
			QVector4D mErrorValues;
			/// Clamps vector values to the [0,1] range
			/// \return Vector with values clamped to the [0,1] range
			QVector4D& clampValues(QVector4D& vector);
			/// Clears the map an then fills it with the values passed as parameters
			/// \param viewportMap New values for the viewport map
			void fillViewportMap(const std::map<GeometryWorldItem::GeometryCamera::CameraTargets, QVector4D>& viewportMap);
			/// Clears the map passed as a parameters, then generates the sizes for the selected number of viewports and populates the map with them. 
			/// This method will try to adjust the viewports as close as posible to a square matrix, aditional viewports will be added as aditional columns at certain rows from bottom to top.
			/// All viewport sizes will be adjusted so the full screen is filled.
			/// \param viewportNumber Number of viewports in the screen
			/// \param viewportMap Viewport map to be cleared and filled usually this object viewport map
			void generateViewportSizes(int viewportNumber, std::map<GeometryWorldItem::GeometryCamera::CameraTargets, QVector4D>& viewportMap);
			/// Copies the data from a FinalPass into this object
			/// \param ref FinalPass to be copied
			virtual void copy(const ScreenViewportLocation& ref);
		};
	}
}

#endif // !SCREENVIEWPORTLOCATION_H
