#include <iostream>
#include <fstream>
#include <functional>

#include <enumerators/CodecType.h>
#include <muxing/Demuxer.h>
#include <encoding/Decoder.h>
#include <muxing/Muxer.h>
#include <encoding/Encoder.h>

int main()
{
	try 
	{
		redav::muxing::Demuxer demuxer;
		demuxer.Open("C:\\Temp\\Gus.mp3");

		std::cout << "Demuxer opened" << std::endl;

		// TODO - Get rid of this shite
#define AV_CH_FRONT_LEFT             0x00000001
#define AV_CH_FRONT_RIGHT            0x00000002
#define AV_CH_LAYOUT_STEREO (AV_CH_FRONT_LEFT|AV_CH_FRONT_RIGHT)

		auto audioParameters = demuxer.GetAudioDecoder().GetAudioParameters()
			.SetMediaType(redav::enumerators::MediaType::Audio)
			.SetCodecType(redav::enumerators::CodecType::PCM_Signed16BitLittleEndian)
			.SetSampleRate(44100)
			.SetBitRate(16 * 44100)
			.SetAudioChannelLayout(AV_CH_LAYOUT_STEREO)
			.SetChannelCount(2)
			.SetSampleFormat(redav::enumerators::SampleFormat::Signed16Bit)
			.SetTimeBase({1, 44100});
		

		redav::muxing::Muxer muxer;
		muxer.Open("C:\\Temp\\Gus.wav", &audioParameters, nullptr);
  
		std::cout << "Muxer opened" << std::endl;

		auto frames = 0;

		demuxer.DecodePackets(redav::enumerators::MediaType::Audio, [&frames, &muxer](redav::media::Frame* frame)
		{
			muxer.WriteFrame(frame);

			frames++;
		});
		
		std::cout << frames << " frames processed - cleaning up..." << std::endl;
  
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

