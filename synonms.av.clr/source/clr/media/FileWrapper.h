#pragma once

#include <media\File.h>

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
					FileWrapper(redav::media::File* file);

					redav::media::File* GetCoreFile();

				private:
					redav::media::File* _file;
				};
			}
		}
	}
}
