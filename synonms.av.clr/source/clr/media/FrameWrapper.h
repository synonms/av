#pragma once

#include <media\Frame.h>

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
					FrameWrapper(redav::media::Frame* frame);
					FrameWrapper(redav::media::Frame&& frame);

					redav::media::Frame* GetFrame();

				private:
					redav::media::Frame* _frame;
				};
			}
		}
	}
}
