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
//		redav::encoding::Decoder decoder(redav::enumerators::Codec::Mp3);
//		decoder.Open();

//		redav::encoding::Encoder encoder(redav::enumerators::Codec::PCM_Signed16BitLittleEndian);
//		encoder.Open();

		redav::muxing::Demuxer demuxer;
		demuxer.Open("C:\\Users\\nsherriff\\Documents\\sine_220Hz_120s.ogg");

		std::cout << "Demuxer opened" << std::endl;

		for(auto stream : demuxer.GetStreams())
		{
			std::cout << redav::enumerators::MediaTypeMapper::ToString(stream.second.GetMediaType()) << " stream found: Audio Codec " << redav::enumerators::CodecTypeMapper::ToString(stream.second.GetAudioCodec()) << std::endl;
		}
  
		redav::muxing::Muxer muxer;
		muxer.Open("C:\\Users\\nsherriff\\Documents\\sine_220Hz_120s.wav", redav::enumerators::CodecType::PCM_Signed16BitLittleEndian, true);
  
		std::cout << "Muxer opened" << std::endl;

		auto frames = 0;

		demuxer.DecodePackets([&frames](redav::media::Frame* frame)
		{
		//		encoder.EncodeFrame(frame, [&muxer](redav::media::Packet* packet)
		//		{
		//			muxer.WritePacket(packet);
		//		});

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

	std::cout << "Enter a character to go away..." << std::endl;
	char c;
	std::cin >> c;

	return 0;
}

