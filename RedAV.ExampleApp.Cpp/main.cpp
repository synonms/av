#include <iostream>
#include <fstream>
#include <functional>

#include <enumerators/AudioCodec.h>
#include <muxing/Demuxer.h>
#include <encoding/Decoder.h>
#include <muxing/Muxer.h>
#include <encoding/Encoder.h>

int main()
{
	try 
	{
		redav::encoding::Decoder decoder(redav::enumerators::AudioCodec::Opus);
		decoder.Open();

		redav::encoding::Encoder encoder(redav::enumerators::AudioCodec::PCM_Signed16BitLittleEndian);
		encoder.Open();

		redav::muxing::Demuxer demuxer;
		demuxer.Open("C:\\Users\\nsherriff\\Documents\\sine_220Hz_120s.ogg");

		std::cout << "Demuxer opened" << std::endl;

		for(auto stream : demuxer.GetStreams())
		{
			std::cout << redav::enumerators::MediaTypeMapper::ToString(stream.GetMediaType()) << " stream found: Audio Codec " << redav::enumerators::AudioCodecMapper::ToString(stream.GetAudioCodec()) << std::endl;
		}
  
		redav::muxing::Muxer muxer;
		muxer.Open("C:\\Users\\nsherriff\\Documents\\sine_220Hz_120s.wav", redav::enumerators::AudioCodec::PCM_Signed16BitLittleEndian, true);
  
		std::cout << "Muxer opened" << std::endl;
  
		demuxer.ForEachPacket([&decoder, &encoder, &muxer](redav::media::Packet* packet)
		{
			decoder.DecodePacket(packet, [&encoder, &muxer](redav::media::Frame* frame)
			{
				encoder.EncodeFrame(frame, [&muxer](redav::media::Packet* packet)
				{
					muxer.WritePacket(packet);
				});
			});
		});
  
		std::cout << "Packets processed - cleaning up..." << std::endl;
  
		muxer.Close();
		demuxer.Close();

		std::cout << "Finished" << std::endl;
	}
	catch(const std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}

	std::cout << "Enter a character to go away..." << std::endl;
	char c;
	std::cin >> c;

	return 0;
}

