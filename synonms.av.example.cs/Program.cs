using System;
using Synonms.Av.Clr.Enumerators;
using Synonms.Av.Clr.Muxing;
using Synonms.Av.Clr.Utilities;

namespace Synonms.Av.Example.Cs
{
    public class Program
    {
        public static void Main(string[] args)
        {
            try
            {
                var demuxer = new Demuxer();
                demuxer.Open("C:\\Temp\\Gus.mp3");

                Console.WriteLine("Demuxer opened");

                var audioParameters = demuxer.GetAudioDecoder().GetAudioParameters()
                    .SetMediaType(MediaType.Audio)
                    .SetCodecType(CodecType.PCM_Signed16BitLittleEndian)
                    .SetSampleRate(44100)
                    .SetBitRate(16 * 44100 * 2)
                    .SetAudioChannelLayout(0x00000003)
                    .SetChannelCount(2)
                    .SetSampleFormat(SampleFormat.Signed16Bit)
                    .SetTimeBase(new RationalNumber(1, 44100));

                var muxer = new Muxer();
                muxer.Open("C:\\Temp\\Gus.wav", audioParameters, null);

                Console.WriteLine("Muxer opened");

                var resampler = new Resampler();
                resampler.Initialise(demuxer.GetAudioDecoder().GetCodec(), muxer.GetAudioEncoder().GetCodec());

                var frames = 0;

                demuxer.FrameDecodedEvent += (frameWrapper) => 
                {
                    switch (frameWrapper.GetMediaType())
                    {
                        case MediaType.Audio:
                            var resampledFrame = resampler.Convert(frameWrapper);
                            muxer.AddToBuffer(resampledFrame);

                            if (resampler.Flush(resampledFrame) > 0) muxer.AddToBuffer(resampledFrame);

                            frames++;

                            Console.WriteLine($"\r{frames} frames decoded");
                            break;
                        case MediaType.Video:
                            // TODO
                            break;
                        default:
                            break;
                    }
                };

                demuxer.DecodePackets(MediaType.Audio);

                Console.WriteLine($"\r{frames} audio frames decoded - muxing...");

                muxer.WriteBuffer();

                Console.WriteLine("Mux complete - cleaning up...");

                muxer.Close();
                demuxer.Close();

                Console.WriteLine("Finished");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Exception: {ex}");
            }

            Console.WriteLine("Enter a character to go away...");
            Console.ReadKey();
        }
    }
}
