using System;
using RedAV.CLR.Enumerators;
using RedAV.CLR.Muxing;

namespace RedAV.ExampleApp.Cs
{
    public class Program
    {
        public static void Main(string[] args)
        {
            try
            {
                var demuxer = new DemuxerWrapper();
                demuxer.Open("C:\\Users\\nsherriff\\Documents\\sine_220Hz_120s.ogg");

                Console.WriteLine("Demuxer opened");

                Console.WriteLine($"{demuxer.GetStreams().Count} streams found");

                var muxer = new MuxerWrapper();
                muxer.Open("C:\\Users\\nsherriff\\Documents\\sine_220Hz_120s.wav", AudioCodecWrapper.PCM_Signed16BitLittleEndian, true);

                Console.WriteLine("Muxer opened");

                demuxer.PacketReadEvent += wrapper => { muxer.WritePacket(wrapper); };

                demuxer.ReadPackets();

                Console.WriteLine("Packets processed - cleaning up...");

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
