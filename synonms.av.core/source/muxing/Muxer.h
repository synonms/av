#pragma once

#include <memory>
#include <string>

#include <encoding/CodecParameters.h>
#include <encoding/Encoder.h>
#include <media/Frame.h>

namespace redav
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