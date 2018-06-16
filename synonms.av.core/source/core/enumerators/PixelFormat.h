#pragma once

#include <string>

enum AVPixelFormat;

namespace synonms
{
	namespace av
	{
		namespace enumerators
		{
			enum class PixelFormat : int
			{
				Unknown = -1,
				AV_PIX_FMT_YUV420P
				// TODO
			};

			class PixelFormatMapper
			{
			public:
				static PixelFormat FromFfmpeg(AVPixelFormat value);
				static AVPixelFormat ToFfmpeg(PixelFormat value);
				static std::string ToString(PixelFormat value);

			private:
				PixelFormatMapper() = default;
			};
		}
	}
}
