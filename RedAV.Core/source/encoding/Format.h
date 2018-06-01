#pragma once

#include <memory>
#include <string>

#include <enumerators/CodecType.h>
#include <media/File.h>

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
			void Initialise();
			bool IsValid() const;
			void Open(const media::File& file);

		public: // Internal
			Format(AVFormatContext* formatContext);
			AVFormatContext* GetAVFormatContext() const;

		private:
			class Implementation;
			std::unique_ptr<Implementation> implementation;
		};
	}
}
