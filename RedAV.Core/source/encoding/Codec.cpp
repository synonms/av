#include "Codec.h"

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/dict.h>
}

#include <iostream>

using namespace redav::enumerators;
using namespace redav::utilities;

namespace redav
{
	// Private Implementation
	namespace encoding
	{
		class Codec::Implementation
		{
		public:
			Implementation()
			{
			}

			AVCodec* codec{ nullptr };
			AVCodecContext* codecContext{ nullptr };

			void SetUp(Purpose purpose, CodecType codecType)
			{
				std::cout << "Codec: Finding " << CodecTypeMapper::ToString(codecType) << "..." << std::endl;

				codec = purpose == Purpose::Encoder ? avcodec_find_encoder(CodecTypeMapper::ToFfmpeg(codecType)) : avcodec_find_decoder(CodecTypeMapper::ToFfmpeg(codecType));

				if (codec == nullptr) throw std::exception("Codec not found");

				std::cout << "Codec: Found" << std::endl;

				codecContext = avcodec_alloc_context3(codec);

				if (codecContext == nullptr) throw std::exception("Failed to create codec context");

				std::cout << "Codec: Context created" << std::endl;
			}

			void TearDown()
			{
				if (codecContext != nullptr)
				{
					avcodec_free_context(&codecContext);
					codecContext = nullptr;
					codec = nullptr;
				}
			}
		};
	}

	// Structors
	namespace encoding
	{
		Codec::Codec()
		{
			implementation = std::make_unique<Implementation>();
		}

		Codec::~Codec()
		{
			implementation->TearDown();
		}
	}

	// Accessors
	namespace encoding
	{
		uint64_t Codec::GetAudioChannelLayout() const
		{
			return implementation->codecContext->channel_layout;
		}

		int64_t Codec::GetBitRate() const
		{
			return implementation->codecContext->bit_rate;
		}

		int Codec::GetChannelCount() const
		{
			return implementation->codecContext->channels;
		}

		int Codec::GetHeight() const
		{
			return implementation->codecContext->height;
		}

		MediaType Codec::GetMediaType() const
		{
			return MediaTypeMapper::FromFfmpeg(implementation->codecContext->codec_type);
		}

		int Codec::GetNoOfSamples() const
		{
			return (implementation->codecContext->codec->capabilities & AV_CODEC_CAP_VARIABLE_FRAME_SIZE) ? 10000 : implementation->codecContext->frame_size;
		}

		PixelFormat Codec::GetPixelFormat() const
		{
			return PixelFormatMapper::FromFfmpeg(implementation->codecContext->pix_fmt);
		}

		RationalNumber Codec::GetSampleAspectRatio() const
		{
			return { implementation->codecContext->sample_aspect_ratio };
		}

		SampleFormat Codec::GetSampleFormat() const
		{
			return SampleFormatMapper::FromFfmpeg(implementation->codecContext->sample_fmt);
		}

		int Codec::GetSampleRate() const
		{
			return implementation->codecContext->sample_rate;
		}

		RationalNumber Codec::GetTimeBase() const
		{
			return { implementation->codecContext->time_base.num, implementation->codecContext->time_base.den };
		}

		int Codec::GetWidth() const
		{
			return implementation->codecContext->width;
		}
	}

	// Internal
	namespace encoding
	{
		AVCodec* Codec::GetCodec() const
		{
			return implementation->codec;
		}

		AVCodecContext* Codec::GetCodecContext() const
		{
			return implementation->codecContext;
		}
	}

	// Public Methods
	namespace encoding
	{
		void Codec::CopyParameters(CodecParameters* parameters)
		{
			std::cout << "Codec: Copying parameters..." << std::endl;

			if (avcodec_parameters_to_context(implementation->codecContext, parameters->GetAVCodecParameters()) < 0) throw std::exception("Failed to copy parameters to codec context");

			std::cout << "Codec: Parameters copied" << std::endl;
		}

		void Codec::Initialise(Purpose purpose, CodecType codecType)
		{
			std::cout << "Codec: Initialising..." << std::endl;

			implementation->SetUp(purpose, codecType);

			std::cout << "Codec: Initialised" << std::endl;
		}

		bool Codec::IsValid() const
		{
			return implementation->codec != nullptr && implementation->codecContext != nullptr;
		}

		void Codec::Open(Dictionary* options)
		{
			std::cout << "Codec: Opening..." << std::endl;

			auto avDictionary = options == nullptr ? nullptr : options->GetAVDictionary();

			if (avcodec_open2(implementation->codecContext, implementation->codec, &avDictionary) < 0) throw std::exception("Failed to open codec");

			std::cout << "Codec: Opened" << std::endl;
		}
	}

	// Mutators
	namespace encoding
	{
		Codec& Codec::SetAudioChannelLayout(uint64_t value)
		{
			implementation->codecContext->channel_layout = value;
			return *this;
		}

		Codec& Codec::SetBitRate(int64_t value)
		{
			implementation->codecContext->bit_rate = value;
			return *this;
		}

		Codec& Codec::SetChannelCount(int value)
		{
			implementation->codecContext->channels = value;
			return *this;
		}

		Codec& Codec::SetFlag(int flag)
		{
			// TODO - Create enum for flags
			implementation->codecContext->flags |= flag;
			return *this;
		}

		Codec& Codec::SetHeight(int value)
		{
			implementation->codecContext->height = value;
			return *this;
		}

		Codec& Codec::SetPixelFormat(PixelFormat value)
		{
			implementation->codecContext->pix_fmt = PixelFormatMapper::ToFfmpeg(value);
			return *this;
		}

		Codec& Codec::SetSampleAspectRatio(const RationalNumber& value)
		{
			implementation->codecContext->sample_aspect_ratio = value.GetAVRational();
			return *this;
		}

		Codec& Codec::SetSampleFormat(SampleFormat value)
		{
			implementation->codecContext->sample_fmt = SampleFormatMapper::ToFfmpeg(value);
			return *this;
		}

		Codec& Codec::SetSampleRate(int value)
		{
			implementation->codecContext->sample_rate = value;
			return *this;
		}

		Codec& Codec::SetTimeBase(const RationalNumber& value)
		{
			implementation->codecContext->time_base = value.GetAVRational();
			return *this;
		}

		Codec& Codec::SetWidth(int value)
		{
			implementation->codecContext->width = value;
			return *this;
		}
	}
}
