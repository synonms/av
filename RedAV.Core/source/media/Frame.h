#pragma once

struct AVFrame;

namespace redav
{
	namespace media
	{
		class Frame
		{
		public:
			Frame(AVFrame* frame);

			AVFrame* GetFrame() const;

		private:
			AVFrame* frame_;
		};
	}
}
