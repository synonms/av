#pragma once

#include <core\media\File.h>

namespace Synonms
{
	namespace Av
	{
		namespace Clr
		{
			namespace Media
			{
				public ref class FileWrapper
				{
				internal:
					FileWrapper(synonms::av::media::File* file);

					synonms::av::media::File* GetCoreFile();

				private:
					synonms::av::media::File* _file;
				};
			}
		}
	}
}
