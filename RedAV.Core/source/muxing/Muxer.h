#pragma once

#include <memory>
#include <string>

#include <encoding/CodecParameters.h>
#include <encoding/Encoder.h>
#include <media/Frame.h>
#include <utilities/Resampler.h>

namespace redav
{
	namespace muxing
	{
		class Muxer
		{
		public:
			Muxer();
			~Muxer();

			void Close();
			const encoding::Encoder& GetAudioEncoder() const;
			bool IsValid() const;
			void Open(const std::string& filePath, encoding::CodecParameters* audioParameters, encoding::CodecParameters* videoParameters);
			void WriteAudioFrame(const media::Frame& frame, bool isFinished = false, utilities::Resampler* resampler = nullptr);

		private:
			class Implementation;
			std::unique_ptr<Implementation> implementation;
		};
	}
}