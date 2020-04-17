#pragma once

#ifndef GFRAMEBUFFERCOMMONS_H
#define GFRAMEBUFFERCOMMONS_H

#include <QOpenGLFunctions>

namespace GeometryEngine
{
	namespace GeometryBuffer
	{
		namespace GFramebufferCommons
		{
			enum class G_COLOR_ATTACHMENTS
			{
				NONE = GL_NONE,
				ATTACHMENT_0 = GL_COLOR_ATTACHMENT0,
				ATTACHMENT_1 = GL_COLOR_ATTACHMENT1,
				ATTACHMENT_2 = GL_COLOR_ATTACHMENT2,
				ATTACHMENT_3 = GL_COLOR_ATTACHMENT3,
				ATTACHMENT_4 = GL_COLOR_ATTACHMENT4,
				ATTACHMENT_5 = GL_COLOR_ATTACHMENT5,
				ATTACHMENT_6 = GL_COLOR_ATTACHMENT6,
				ATTACHMENT_7 = GL_COLOR_ATTACHMENT7,
			};

			enum class G_DEPTH_STENCIL_ATTACHMENTS
			{
				NONE = GL_NONE,
				DEPTH = GL_DEPTH_ATTACHMENT,
				STENCIL = GL_STENCIL_ATTACHMENT,
				DEPTH_STENCIL = GL_DEPTH_STENCIL_ATTACHMENT
			};

			class EnumCasts
			{
			public:
				static bool CastColorAttachment(int attachmentIndex, G_COLOR_ATTACHMENTS* attachment)
				{
					switch (attachmentIndex)
					{
					case -1:
						(*attachment) = G_COLOR_ATTACHMENTS::NONE;
						break;
					case 0:
						(*attachment) = G_COLOR_ATTACHMENTS::ATTACHMENT_0;
						break;
					case 1:
						(*attachment) = G_COLOR_ATTACHMENTS::ATTACHMENT_1;
						break;
					case 2:
						(*attachment) = G_COLOR_ATTACHMENTS::ATTACHMENT_2;
						break;
					case 3:
						(*attachment) = G_COLOR_ATTACHMENTS::ATTACHMENT_3;
						break;
					case 4:
						(*attachment) = G_COLOR_ATTACHMENTS::ATTACHMENT_4;
						break;
					case 5:
						(*attachment) = G_COLOR_ATTACHMENTS::ATTACHMENT_5;
						break;
					case 6:
						(*attachment) = G_COLOR_ATTACHMENTS::ATTACHMENT_6;
						break;
					case 7:
						(*attachment) = G_COLOR_ATTACHMENTS::ATTACHMENT_7;
						break;
					default:
						(*attachment) = G_COLOR_ATTACHMENTS::NONE;
						return false;
					}

					return true;
				}

				static bool CastDepthStencilAttachment(int attachmentIndex, G_DEPTH_STENCIL_ATTACHMENTS* attachment)
				{
					switch (attachmentIndex)
					{
					case 0:
						(*attachment) = G_DEPTH_STENCIL_ATTACHMENTS::NONE;
						break;
					case 1:
						(*attachment) = G_DEPTH_STENCIL_ATTACHMENTS::DEPTH;
						break;
					case 2:
						(*attachment) = G_DEPTH_STENCIL_ATTACHMENTS::STENCIL;
						break;
					case 3:
						(*attachment) = G_DEPTH_STENCIL_ATTACHMENTS::DEPTH_STENCIL;
						break;
					default:
						(*attachment) = G_DEPTH_STENCIL_ATTACHMENTS::NONE;
						return false;
					}
					return true;
				}
			};
		}
	}
}

#endif