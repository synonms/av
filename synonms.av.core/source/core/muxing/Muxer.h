#pragma once

#include <memory>
#include <string>

#include <core\encoding\CodecParameters.h>
#include <core\encoding\Encoder.h>
#include <core\media\Frame.h>

namespace synonms
{
	namespace av
	{
		namespace muxing
		{
			class Muxer
			{
			public:
				Muxer();
				~Muxer();

				void AddToBuffer(const media::Frame& frame);
				void Close();
				const encoding::Encoder& GetAudioEncoder() const;
				const encoding::Encoder& GetVideoEncoder() const;
				bool IsValid() const;
				void Open(const std::string& filePath, encoding::CodecParameters* audioParameters, encoding::CodecParameters* videoParameters);
				void WriteBuffer();

			private:
				class Implementation;
				std::unique_ptr<Implementation> implementation;
			};
		}
	}
}