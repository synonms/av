#include "AudioCodec.h"

extern "C"
{
#include <libavcodec/avcodec.h>
}

using namespace redav::enumerators;

AudioCodec AudioCodecMapper::FromFfmpeg(AVCodecID value)
{
	switch (value)
	{
	case AV_CODEC_ID_PCM_S16LE: return AudioCodec::PCM_Signed16BitLittleEndian;
	case AV_CODEC_ID_PCM_S16BE: return AudioCodec::PCM_Signed16BitBigEndian;
	case AV_CODEC_ID_PCM_U16LE: return AudioCodec::PCM_Unsigned16BitLittleEndian;
	case AV_CODEC_ID_PCM_U16BE: return AudioCodec::PCM_Unsigned16BitBigEndian;
	case AV_CODEC_ID_PCM_MULAW: return AudioCodec::PCM_MuLaw;
	case AV_CODEC_ID_PCM_ALAW: return AudioCodec::PCM_ALaw;
	case AV_CODEC_ID_ADPCM_IMA_SMJPEG: return AudioCodec::AdaptivePCM_Ima_SdlMotionJpeg;
	case AV_CODEC_ID_ADPCM_G726: return AudioCodec::AdaptivePCM_G726;
	case AV_CODEC_ID_ADPCM_G722: return AudioCodec::AdaptivePCM_G722;
	case AV_CODEC_ID_ADPCM_IMA_OKI: return AudioCodec::AdaptivePCM_Ima_Oki;
	case AV_CODEC_ID_ADPCM_G726LE: return AudioCodec::AdaptivePCM_G726LittleEndian;
	case AV_CODEC_ID_AMR_NB: return AudioCodec::AmrNarrowBand;
	case AV_CODEC_ID_AMR_WB: return AudioCodec::AmrWideBand;
	case AV_CODEC_ID_MP2: return AudioCodec::Mp2;
	case AV_CODEC_ID_MP3: return AudioCodec::Mp3;
	case AV_CODEC_ID_AAC: return AudioCodec::Aac;
	case AV_CODEC_ID_VORBIS: return AudioCodec::Vorbis;
	case AV_CODEC_ID_FLAC: return AudioCodec::Flac;
	case AV_CODEC_ID_GSM: return AudioCodec::Gsm;
	case AV_CODEC_ID_COOK: return AudioCodec::Cook_G722_1;
	case AV_CODEC_ID_GSM_MS: return AudioCodec::Gsm_Ms;
	case AV_CODEC_ID_SPEEX: return AudioCodec::Speex;
	case AV_CODEC_ID_G723_1: return AudioCodec::G723_1;
	case AV_CODEC_ID_G729: return AudioCodec::G729;
	case AV_CODEC_ID_ILBC: return AudioCodec::InternetLowBitRate;
	case AV_CODEC_ID_OPUS: return AudioCodec::Opus;
	default: return AudioCodec::Unknown;
	}
}

AVCodecID AudioCodecMapper::ToFfmpeg(AudioCodec value)
{
	switch (value)
	{
	case AudioCodec::PCM_Signed16BitLittleEndian: return AV_CODEC_ID_PCM_S16LE;
	case AudioCodec::PCM_Signed16BitBigEndian: return AV_CODEC_ID_PCM_S16BE;
	case AudioCodec::PCM_Unsigned16BitLittleEndian: return AV_CODEC_ID_PCM_U16LE;
	case AudioCodec::PCM_Unsigned16BitBigEndian: return AV_CODEC_ID_PCM_U16BE;
	case AudioCodec::PCM_MuLaw: return AV_CODEC_ID_PCM_MULAW;
	case AudioCodec::PCM_ALaw: return AV_CODEC_ID_PCM_ALAW;
	case AudioCodec::AdaptivePCM_Ima_SdlMotionJpeg: return AV_CODEC_ID_ADPCM_IMA_SMJPEG;
	case AudioCodec::AdaptivePCM_G726: return AV_CODEC_ID_ADPCM_G726;
	case AudioCodec::AdaptivePCM_G722: return AV_CODEC_ID_ADPCM_G722;
	case AudioCodec::AdaptivePCM_Ima_Oki: return AV_CODEC_ID_ADPCM_IMA_OKI;
	case AudioCodec::AdaptivePCM_G726LittleEndian: return AV_CODEC_ID_ADPCM_G726LE;
	case AudioCodec::AmrNarrowBand: return AV_CODEC_ID_AMR_NB;
	case AudioCodec::AmrWideBand: return AV_CODEC_ID_AMR_WB;
	case AudioCodec::Mp2: return AV_CODEC_ID_MP2;
	case AudioCodec::Mp3: return AV_CODEC_ID_MP3;
	case AudioCodec::Aac: return AV_CODEC_ID_AAC;
	case AudioCodec::Vorbis: return AV_CODEC_ID_VORBIS;
	case AudioCodec::Flac: return AV_CODEC_ID_FLAC;
	case AudioCodec::Gsm: return AV_CODEC_ID_GSM;
	case AudioCodec::Cook_G722_1: return AV_CODEC_ID_G723_1;
	case AudioCodec::Gsm_Ms: return AV_CODEC_ID_GSM_MS;
	case AudioCodec::Speex: return AV_CODEC_ID_SPEEX;
	case AudioCodec::G723_1: return AV_CODEC_ID_G723_1;
	case AudioCodec::G729: return AV_CODEC_ID_G729;
	case AudioCodec::InternetLowBitRate: return AV_CODEC_ID_ILBC;
	case AudioCodec::Opus: return AV_CODEC_ID_OPUS;
	default: return AV_CODEC_ID_NONE;
	}
}

std::string AudioCodecMapper::ToString(AudioCodec value)
{
	switch (value)
	{
	case AudioCodec::PCM_Signed16BitLittleEndian: return "PCM - Signed 16bit Little Endian";
	case AudioCodec::PCM_Signed16BitBigEndian: return "PCM - Signed 16bit Big Endian";
	case AudioCodec::PCM_Unsigned16BitLittleEndian: return "PCM - Unsigned 16bit Little Endian";
	case AudioCodec::PCM_Unsigned16BitBigEndian: return "PCM - Unsigned 16bit Big Endian";
	case AudioCodec::PCM_MuLaw: return "PCM - MuLaw";
	case AudioCodec::PCM_ALaw: return "PCM - ALaw";
	case AudioCodec::AdaptivePCM_Ima_SdlMotionJpeg: return "Adaptive PCM - IMA SDL Motion JPEG";
	case AudioCodec::AdaptivePCM_G726: return "Adaptive PCM - G.726";
	case AudioCodec::AdaptivePCM_G722: return "Adaptive PCM - G.722";
	case AudioCodec::AdaptivePCM_Ima_Oki: return "Adaptive PCM - IMA OKI";
	case AudioCodec::AdaptivePCM_G726LittleEndian: return "Adaptive PCM - G.726 Little Endian";
	case AudioCodec::AmrNarrowBand: return "AMR Narrow Band";
	case AudioCodec::AmrWideBand: return "AMR Wide Band";
	case AudioCodec::Mp2: return "MPEG-1 Layer 2 (MPEG-2)";
	case AudioCodec::Mp3: return "MPEG-1 Layer 3 (MP3)";
	case AudioCodec::Aac: return "Advanced Audio Coding";
	case AudioCodec::Vorbis: return "Vorbis";
	case AudioCodec::Flac: return "Free Lossless Audio Codec";
	case AudioCodec::Gsm: return "GSM";
	case AudioCodec::Cook_G722_1: return "Cook (G.722.1)";
	case AudioCodec::Gsm_Ms: return "GSM MS";
	case AudioCodec::Speex: return "Speex";
	case AudioCodec::G723_1: return "G723.1";
	case AudioCodec::G729: return "G729";
	case AudioCodec::InternetLowBitRate: return "Internet Low Bitrate";
	case AudioCodec::Opus: return "Opus";
	default: return "Unknown";
	}
}