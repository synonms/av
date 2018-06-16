#pragma once

#include <string>

enum AVSampleFormat;

namespace synonms
{
	namespace av
	{
		namespace enumerators
		{
			enum class SampleFormat : int
			{
				None = -1,
				Unsigned8Bit,
				Signed16Bit,
				Signed32Bit,
				Float,
				Double,
				Unsigned8BitPlanar,
				Signed16BitPlanar,
				Signed32BitPlanar,
				FloatPlanar,
				DoublePlanar,
				Signed64Bit,
				Signed64BitPlanar
			};

			class SampleFormatMapper
			{
			public:
				static SampleFormat FromFfmpeg(AVSampleFormat value);
				static AVSampleFormat ToFfmpeg(SampleFormat value);
				static std::string ToString(SampleFormat value);

			private:
				SampleFormatMapper() = default;
			};
		}
	}
}
