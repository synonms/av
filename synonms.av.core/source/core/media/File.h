#pragma once

#include <memory>
#include <string>

struct AVIOContext;

namespace synonms
{
	namespace av
	{
		namespace media
		{
			class File
			{
			public:
				File();
				~File();

				const std::string& GetFilePath() const;
				void Close();
				void Open(const std::string& filePath);

			public: // Internal
				AVIOContext * GetAVIOContext() const;

			private:
				class Implementation;
				std::unique_ptr<Implementation> implementation;
			};
		}
	}
}
