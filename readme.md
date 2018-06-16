# synonms AV #

synonms AV is a simple, minimal set of abstraction layers for the audio and video conversion and streaming library FFmpeg (https://www.ffmpeg.org/).  While it is an incredibly powerful and useful piece of software, FFmpeg's gnarly C API and confusing documentation can prove to be difficult barriers to overcome, particularly for those inexperienced with audio/video encoding and users of modern OOP languages.

The goal of synonms AV is essentially a learning exercise to try and unravel the basic operations of FFmpeg - encoding/decoding and muxing/demuxing.  It will provide lean, minimal APIs for C++ and C# via CLR.  It currently supports audio only, though I am currently working on basic video support too.  I may also look to implement an adapter layer for Qt.  It will not be an extensive implementation or provide any of the advanced features of FFmpeg.  Simplicity is a key driver.  It is not intended to replace FFmpeg in any way or to be used in production solutions.

Prebuilt binaries of FFmpeg 4.0 are included in this repo for convenience.  They were obtained from https://ffmpeg.zeranoe.com/builds/ and no modifications have been made to them.  Licensing information for FFmpeg is available at https://www.ffmpeg.org/legal.html.

Feel free to use synonms AV for any purpose as long it's not illegal, unethical or damaging to any individuals or entities.


## AV Core ##

AV Core is the base layer that sits on top of FFmpeg.  It adds an object oriented C++ layer, encapsulating related functionality into simple classes.

For example, the bare essentials for setting up a codec for decoding MP3 data with FFmpeg's API can look like this:

```
AVCodec* codec;
AVCodecContext* c = NULL;
codec = avcodec_find_decoder(AV_CODEC_ID_MP3)
c = avcodec_alloc_context3(codec);
avcodec_open2(c, codec, NULL);

// Do stuff...

avcodec_free_context(&codecContext);
```

With synonms AV, you can achieve the same thing with:

```
Codec codec;
codec.Initialise(Codec::Purpose::Decoder, CodecType::Mp3);
codec.Open();

// Do stuff...
```

Objects are generally freed in object destructors so you don't have to faff about releasing memory.


## AV CLR ##

AV CLR sits above Core and provides CLR objects for use in .NET.  Every class in Core has an equivalent CLR class, although most of them are stub classes with little purpose currently.  Classes which have the 'Wrapper' suffix are simple carriers for instances of Core classes and are not intended to be instantiated directly by the user.  For example, Clr::Media::PacketWrapper is intended to be instantiated with a pointer to an instance of core::media::Packet and is generally just used to pass data between other classes.  Classes without the 'Wrapper' suffix are intended to be instantiated by the user, for example Demuxer and Muxer.


## Example CPP/CS ##

These projects demonstrate how to transcode an audio file from one format to another (MP3 to WAV tested) using either C++ and the AV Core API or C# and the AV CLR API.  The process is as follows:

    * Create a Demuxer to open and decode an existing audio file
	* Create a Muxer to encode and output the new audio file
	* Create a Resampler to convert the audio from the existing format into the new format (not encoding)
	* Use the Demuxer to decode the existing audio
	* For each decoded packet, resample and add them to the Muxer's audio buffer
	* Write the filled buffer to the output file - this process encodes the data
	* Close the Muxer and Demuxer to finalise and close the files

