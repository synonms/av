#pragma once

#include <memory>
#include <string>

#include <enumerators\CodecType.h>

struct AVFormatContext;

namespace redav
{
	namespace encoding
	{
		class Format
		{
		public:
			Format();
			~Format();

			enumerators::CodecType GetAudioCodecType() const;
			enumerators::CodecType GetVideoCodecType() const;
			bool HasGlobalHeader() const;
			void Initialise(const std::string& filePath);
			bool IsValid() const;

		public: // Internal
			Format(AVFormatContext* formatContext);
			AVFormatContext* GetAVFormatContext() const;

		private:
			class Implementation;
			std::unique_ptr<Implementation> implementation;
		};
	}
}
