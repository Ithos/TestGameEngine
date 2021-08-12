#pragma once
#ifndef GEOMETRYCAMERACOMMONS_H
#define GEOMETRYCAMERACOMMONS_H

namespace GeometryEngine
{
	namespace GeometryWorldItem
	{
		namespace GeometryCamera
		{
			/// All posible camera render targets
			/// NONE -> The camera will not be rendered to any final location, all other intermediary calculations and renders will happen
			/// TEXTURE -> The camera will be rendered to a texture if a fitting material exists
			/// CAMX -> The camera will be rendered to the screen at the CAMX viewport location if it exists
			enum CameraTargets
			{
				NONE,
				TEXTURE,
				CAM1,
				CAM2,
				CAM3,
				CAM4,
				CAM5,
				CAM6,
				CAM7,
				CAM8,
				CAM9,
				CAM10
			};
		}
	}
}

#endif // !GEOMETRYCAMERACOMMONS_H
