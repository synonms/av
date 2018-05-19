#pragma once

#include <string>

enum AVMediaType;

namespace redav
{	
	namespace enumerators
	{
		enum class MediaType : int
		{
			Unknown = -1,
			Video = 0,
			Audio = 1,
		};

		class MediaTypeMapper
		{
		public:
			static MediaType FromFfmpeg(AVMediaType value);
			static AVMediaType ToFfmpeg(MediaType value);
			static std::string ToString(MediaType type);

		private:
			MediaTypeMapper() = default;
		};
	}
}
