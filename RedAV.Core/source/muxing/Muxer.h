#pragma once

#include <memory>
#include <string>

#include <encoding\CodecParameters.h>
#include <media\Frame.h>

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
			bool IsValid() const;
			void Open(const std::string& filePath, encoding::CodecParameters* audioParameters, encoding::CodecParameters* videoParameters);
			void WriteFrame(media::Frame* frame);

		private:
			class Implementation;
			std::unique_ptr<Implementation> implementation;
		};
	}
}