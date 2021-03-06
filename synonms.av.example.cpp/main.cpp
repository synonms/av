#include <iostream>
#include <fstream>
#include <functional>

#include <core\enumerators\CodecType.h>
#include <core\muxing\Demuxer.h>
#include <core\encoding\Decoder.h>
#include <core\muxing\Muxer.h>
#include <core\encoding\Encoder.h>
#include <core\utilities\Resampler.h>

void TranscodeAudioMp3ToWav()
{
	synonms::av::muxing::Demuxer demuxer;
	demuxer.Open("C:\\temp\\Gus.mp3");

	std::cout << "Demuxer opened" << std::endl;

	// TODO - Get rid of this define rubbish
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
		.SetTimeBase({ 1, 44100 });

	synonms::av::muxing::Muxer muxer;
	muxer.Open("C:\\Temp\\Gus.wav", &audioParameters, nullptr);

	std::cout << "Muxer opened" << std::endl;

	synonms::av::utilities::Resampler audioResampler;
	audioResampler.Initialise(demuxer.GetAudioDecoder().GetCodec(), muxer.GetAudioEncoder().GetCodec());

	std::cout << "Audio resampler initialised" << std::endl;

	auto frames = 0;

	demuxer.DecodePackets(synonms::av::enumerators::MediaType::Audio, [&frames, &muxer, &audioResampler](const synonms::av::media::Frame& frame)
	{
		switch (frame.GetMediaType())
		{
		case synonms::av::enumerators::MediaType::Audio:
			auto resampledFrame = audioResampler.Convert(frame);
			muxer.AddToBuffer(resampledFrame);

			if (audioResampler.Flush(resampledFrame) > 0) muxer.AddToBuffer(resampledFrame);

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

void WIP_TranscodeVideoAviToMp4()
{
	synonms::av::muxing::Demuxer demuxer;
	demuxer.Open("C:\\temp\\test.avi");

	std::cout << "Demuxer opened" << std::endl;

	// TODO - Get rid of this
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
		.SetTimeBase({ 1, 44100 });

	auto videoParameters = demuxer.GetVideoDecoder().GetVideoParameters()
		.SetMediaType(synonms::av::enumerators::MediaType::Video)
		.SetCodecType(synonms::av::enumerators::CodecType::Video_H264)
		.SetWidth(352)
		.SetHeight(288)
		.SetBitRate(400000)
		.SetSampleAspectRatio({ 352, 288 })
		.SetPixelFormat(synonms::av::enumerators::PixelFormat::AV_PIX_FMT_YUV420P)
		.SetTimeBase({ 1, 25 });

	synonms::av::muxing::Muxer muxer;
	muxer.Open("C:\\Temp\\Siren.mp4", &audioParameters, &videoParameters);

	std::cout << "Muxer opened" << std::endl;

	synonms::av::utilities::Resampler audioResampler;
	audioResampler.Initialise(demuxer.GetAudioDecoder().GetCodec(), muxer.GetAudioEncoder().GetCodec());

	std::cout << "Audio resampler initialised" << std::endl;

	synonms::av::utilities::Resampler videoResampler;
	videoResampler.Initialise(demuxer.GetVideoDecoder().GetCodec(), muxer.GetVideoEncoder().GetCodec());

	std::cout << "Video resampler initialised" << std::endl;

	auto frames = 0;

	//demuxer.DecodePackets(synonms::av::enumerators::MediaType::Audio, [&frames, &muxer, &resampler](const synonms::av::media::Frame& frame)
	//{
	//	switch (frame.GetMediaType())
	//	{
	//	case synonms::av::enumerators::MediaType::Audio:
	//		auto resampledFrame = resampler.Convert(frame);
	//		muxer.AddToBuffer(resampledFrame);

	//		if (resampler.Flush(resampledFrame) > 0) muxer.AddToBuffer(resampledFrame);

	//		frames++;

	//		std::cout << "\r" << frames << " frames decoded";
	//	}
	//});

	//std::cout << "\r" << frames << " audio frames decoded - muxing..." << std::endl;

	//muxer.WriteBuffer();

	std::cout << "Mux complete - cleaning up..." << std::endl;

	muxer.Close();
	demuxer.Close();

	std::cout << "Finished" << std::endl;
}

int main()
{
	try 
	{
		TranscodeAudioMp3ToWav();
	}
	catch(const std::exception& ex)
	{
		std::cout << ex.what() << std::endl;
	}

	std::cout << "Press a key to go away..." << std::endl;

	std::cin.get();

	return 0;
}

