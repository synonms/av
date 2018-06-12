#pragma once

#include <memory>

struct AVDictionary;

namespace redav
{
	namespace utilities
	{
		class Dictionary
		{
		public:
			Dictionary();
			Dictionary(const Dictionary& dictionary);
			~Dictionary();
			Dictionary& operator=(const Dictionary& dictionary);

		public: //Internal
			Dictionary(AVDictionary* dictionary);

			AVDictionary* GetAVDictionary() const;

		private:
			class Implementation;
			std::unique_ptr<Implementation> implementation;
		};
	}
}