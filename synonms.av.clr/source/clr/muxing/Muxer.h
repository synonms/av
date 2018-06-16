#pragma once

#include <core\muxing\Muxer.h>

#include <clr\encoding\CodecParameters.h>
#include <clr\encoding\EncoderWrapper.h>
#include <clr\media\FrameWrapper.h>

namespace Synonms
{
	namespace Av
	{
		namespace Clr
		{
			namespace Muxing
			{
				public ref class Muxer
				{
				public:
					Muxer();
					~Muxer();
					!Muxer();

					void AddToBuffer(Media::FrameWrapper^ frame);
					void Close();
					Encoding::EncoderWrapper^ GetAudioEncoder();
					Encoding::EncoderWrapper^ GetVideoEncoder();
					void Open(System::String^ filePath, Encoding::CodecParameters^ audioParameters, Encoding::CodecParameters^ videoParameters);
					void WriteBuffer();

				private:
					synonms::av::muxing::Muxer* _muxer;
				};
			}
		}
	}
}
