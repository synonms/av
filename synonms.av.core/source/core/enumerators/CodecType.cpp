#include "CodecType.h"

extern "C"
{
#include <libavcodec/avcodec.h>
}

using namespace synonms::av::enumerators;

CodecType CodecTypeMapper::FromFfmpeg(AVCodecID value)
{
	switch (value)
	{
	case AV_CODEC_ID_H263: return CodecType::Video_H263;
	case AV_CODEC_ID_MPEG4: return CodecType::Video_MPEG4;
	case AV_CODEC_ID_RAWVIDEO: return CodecType::Video_RAW;
	case AV_CODEC_ID_H264: return CodecType::Video_H264;
	case AV_CODEC_ID_VP8: return CodecType::Video_VP8;
	case AV_CODEC_ID_VP9: return CodecType::Video_VP9;

	case AV_CODEC_ID_PCM_S16LE: return CodecType::PCM_Signed16BitLittleEndian;
	case AV_CODEC_ID_PCM_S16BE: return CodecType::PCM_Signed16BitBigEndian;
	case AV_CODEC_ID_PCM_U16LE: return CodecType::PCM_Unsigned16BitLittleEndian;
	case AV_CODEC_ID_PCM_U16BE: return CodecType::PCM_Unsigned16BitBigEndian;
	case AV_CODEC_ID_PCM_MULAW: return CodecType::PCM_MuLaw;
	case AV_CODEC_ID_PCM_ALAW: return CodecType::PCM_ALaw;
	case AV_CODEC_ID_ADPCM_IMA_SMJPEG: return CodecType::AdaptivePCM_Ima_SdlMotionJpeg;
	case AV_CODEC_ID_ADPCM_G726: return CodecType::AdaptivePCM_G726;
	case AV_CODEC_ID_ADPCM_G722: return CodecType::AdaptivePCM_G722;
	case AV_CODEC_ID_ADPCM_IMA_OKI: return CodecType::AdaptivePCM_Ima_Oki;
	case AV_CODEC_ID_ADPCM_G726LE: return CodecType::AdaptivePCM_G726LittleEndian;
	case AV_CODEC_ID_AMR_NB: return CodecType::AmrNarrowBand;
	case AV_CODEC_ID_AMR_WB: return CodecType::AmrWideBand;
	case AV_CODEC_ID_MP2: return CodecType::Mp2;
	case AV_CODEC_ID_MP3: return CodecType::Mp3;
	case AV_CODEC_ID_AAC: return CodecType::Aac;
	case AV_CODEC_ID_VORBIS: return CodecType::Vorbis;
	case AV_CODEC_ID_FLAC: return CodecType::Flac;
	case AV_CODEC_ID_GSM: return CodecType::Gsm;
	case AV_CODEC_ID_COOK: return CodecType::Cook_G722_1;
	case AV_CODEC_ID_GSM_MS: return CodecType::Gsm_Ms;
	case AV_CODEC_ID_SPEEX: return CodecType::Speex;
	case AV_CODEC_ID_G723_1: return CodecType::G723_1;
	case AV_CODEC_ID_G729: return CodecType::G729;
	case AV_CODEC_ID_ILBC: return CodecType::InternetLowBitRate;
	case AV_CODEC_ID_OPUS: return CodecType::Opus;
	default: return CodecType::Unknown;
	}
}

AVCodecID CodecTypeMapper::ToFfmpeg(CodecType value)
{
	switch (value)
	{
	case CodecType::Video_H263: return AV_CODEC_ID_H263;
	case CodecType::Video_MPEG4: return AV_CODEC_ID_MPEG4;
	case CodecType::Video_RAW: return AV_CODEC_ID_RAWVIDEO;
	case CodecType::Video_H264: return AV_CODEC_ID_H264;
	case CodecType::Video_VP8: return AV_CODEC_ID_VP8;
	case CodecType::Video_VP9: return AV_CODEC_ID_VP9;

	case CodecType::PCM_Signed16BitLittleEndian: return AV_CODEC_ID_PCM_S16LE;
	case CodecType::PCM_Signed16BitBigEndian: return AV_CODEC_ID_PCM_S16BE;
	case CodecType::PCM_Unsigned16BitLittleEndian: return AV_CODEC_ID_PCM_U16LE;
	case CodecType::PCM_Unsigned16BitBigEndian: return AV_CODEC_ID_PCM_U16BE;
	case CodecType::PCM_MuLaw: return AV_CODEC_ID_PCM_MULAW;
	case CodecType::PCM_ALaw: return AV_CODEC_ID_PCM_ALAW;
	case CodecType::AdaptivePCM_Ima_SdlMotionJpeg: return AV_CODEC_ID_ADPCM_IMA_SMJPEG;
	case CodecType::AdaptivePCM_G726: return AV_CODEC_ID_ADPCM_G726;
	case CodecType::AdaptivePCM_G722: return AV_CODEC_ID_ADPCM_G722;
	case CodecType::AdaptivePCM_Ima_Oki: return AV_CODEC_ID_ADPCM_IMA_OKI;
	case CodecType::AdaptivePCM_G726LittleEndian: return AV_CODEC_ID_ADPCM_G726LE;
	case CodecType::AmrNarrowBand: return AV_CODEC_ID_AMR_NB;
	case CodecType::AmrWideBand: return AV_CODEC_ID_AMR_WB;
	case CodecType::Mp2: return AV_CODEC_ID_MP2;
	case CodecType::Mp3: return AV_CODEC_ID_MP3;
	case CodecType::Aac: return AV_CODEC_ID_AAC;
	case CodecType::Vorbis: return AV_CODEC_ID_VORBIS;
	case CodecType::Flac: return AV_CODEC_ID_FLAC;
	case CodecType::Gsm: return AV_CODEC_ID_GSM;
	case CodecType::Cook_G722_1: return AV_CODEC_ID_G723_1;
	case CodecType::Gsm_Ms: return AV_CODEC_ID_GSM_MS;
	case CodecType::Speex: return AV_CODEC_ID_SPEEX;
	case CodecType::G723_1: return AV_CODEC_ID_G723_1;
	case CodecType::G729: return AV_CODEC_ID_G729;
	case CodecType::InternetLowBitRate: return AV_CODEC_ID_ILBC;
	case CodecType::Opus: return AV_CODEC_ID_OPUS;
	default: return AV_CODEC_ID_NONE;
	}
}

std::string CodecTypeMapper::ToString(CodecType value)
{
	switch (value)
	{
	case CodecType::Video_H263: return "H.263";
	case CodecType::Video_MPEG4: return "MPEG 4";
	case CodecType::Video_RAW: return "Raw";
	case CodecType::Video_H264: return "H.264";
	case CodecType::Video_VP8: return "VP8";
	case CodecType::Video_VP9: return "VP9";

	case CodecType::PCM_Signed16BitLittleEndian: return "PCM - Signed 16bit Little Endian";
	case CodecType::PCM_Signed16BitBigEndian: return "PCM - Signed 16bit Big Endian";
	case CodecType::PCM_Unsigned16BitLittleEndian: return "PCM - Unsigned 16bit Little Endian";
	case CodecType::PCM_Unsigned16BitBigEndian: return "PCM - Unsigned 16bit Big Endian";
	case CodecType::PCM_MuLaw: return "PCM - MuLaw";
	case CodecType::PCM_ALaw: return "PCM - ALaw";
	case CodecType::AdaptivePCM_Ima_SdlMotionJpeg: return "Adaptive PCM - IMA SDL Motion JPEG";
	case CodecType::AdaptivePCM_G726: return "Adaptive PCM - G.726";
	case CodecType::AdaptivePCM_G722: return "Adaptive PCM - G.722";
	case CodecType::AdaptivePCM_Ima_Oki: return "Adaptive PCM - IMA OKI";
	case CodecType::AdaptivePCM_G726LittleEndian: return "Adaptive PCM - G.726 Little Endian";
	case CodecType::AmrNarrowBand: return "AMR Narrow Band";
	case CodecType::AmrWideBand: return "AMR Wide Band";
	case CodecType::Mp2: return "MPEG-1 Layer 2 (MPEG-2)";
	case CodecType::Mp3: return "MPEG-1 Layer 3 (MP3)";
	case CodecType::Aac: return "Advanced Audio Coding";
	case CodecType::Vorbis: return "Vorbis";
	case CodecType::Flac: return "Free Lossless Audio CodecType";
	case CodecType::Gsm: return "GSM";
	case CodecType::Cook_G722_1: return "Cook (G.722.1)";
	case CodecType::Gsm_Ms: return "GSM MS";
	case CodecType::Speex: return "Speex";
	case CodecType::G723_1: return "G723.1";
	case CodecType::G729: return "G729";
	case CodecType::InternetLowBitRate: return "Internet Low Bitrate";
	case CodecType::Opus: return "Opus";
	default: return "Unknown";
	}
}