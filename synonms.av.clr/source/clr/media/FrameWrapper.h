#pragma once

#include <core\media\Frame.h>

#include <clr\enumerators\MediaType.h>

namespace Synonms
{
	namespace Av
	{
		namespace Clr
		{
			namespace Media
			{
				public ref class FrameWrapper
				{
				public:
					Enumerators::MediaType GetMediaType();

				internal:
					FrameWrapper(synonms::av::media::Frame* frame);
					FrameWrapper(synonms::av::media::Frame&& frame);

					synonms::av::media::Frame* GetFrame();

				private:
					synonms::av::media::Frame* _frame;
				};
			}
		}
	}
}
