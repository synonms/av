#include <iostream>
#include <fstream>
#include <functional>

#include <core\enumerators\CodecType.h>
#include <core\muxing\Demuxer.h>
#include <core\encoding\Decoder.h>
#include <core\muxing\Muxer.h>
#include <core\encoding\Encoder.h>
#include <core\utilities\Resampler.h>

int main()
{
	try 
	{
		synonms::av::muxing::Demuxer demuxer;
		demuxer.Open("C:\\Temp\\Gus.mp3");

		std::cout << "Demuxer opened" << std::endl;

		// TODO - Get rid of this shite
#define AV_CH_FRONT_LEFT             0x00000001
#define AV_CH_FRONT_RIGHT            0x00000002
#define AV_CH_LAYOUT_STEREO (AV_CH_FRONT_LEFT|AV_CH_FRONT_RIGHT)

		auto audioParameters = demuxer.GetAudioDecoder().GetAudioParameters()
			.SetMediaType(synonms::av::enumerators::MediaType::Audio)
			.SetCodecType(synonms::av::enumerators::CodecType::PCM_Signed16BitLittleEndian)
			.SetSampleRate(44100)
			.SetBitRate(16 * 44100 * 2)
			.SetAudioChannelLayout(AV_CH_LAYOUT_STEREO)
			.SetChannelCount(2)
			.SetSampleFormat(synonms::av::enumerators::SampleFormat::Signed16Bit)
			.SetTimeBase({1, 44100});
		

		synonms::av::muxing::Muxer muxer;
		muxer.Open("C:\\Temp\\Gus.wav", &audioParameters, nullptr);
  
		std::cout << "Muxer opened" << std::endl;

		synonms::av::utilities::Resampler resampler;
		resampler.Initialise(demuxer.GetAudioDecoder().GetCodec(), muxer.GetAudioEncoder().GetCodec());

		auto frames = 0;

		demuxer.DecodePackets(synonms::av::enumerators::MediaType::Audio, [&frames, &muxer, &resampler](const synonms::av::media::Frame& frame)
		{
			switch (frame.GetMediaType())
			{
			case synonms::av::enumerators::MediaType::Audio:
				auto resampledFrame = resampler.Convert(frame);
				muxer.AddToBuffer(resampledFrame);

				if (resampler.Flush(resampledFrame) > 0) muxer.AddToBuffer(resampledFrame);

				frames++;

				std::cout << "\r" << frames << " frames decoded";
			}
		});

		std::cout << "\r" << frames << " audio frames decoded - muxing..." << std::endl;

		muxer.WriteBuffer();

		std::cout << "Mux complete - cleaning up..." << std::endl;
  
		muxer.Close();
		demuxer.Close();

		std::cout << "Finished" << std::endl;
	}
	catch(const std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}

	std::cout << "Press a key to go away..." << std::endl;

	std::cin.get();

	return 0;
}

