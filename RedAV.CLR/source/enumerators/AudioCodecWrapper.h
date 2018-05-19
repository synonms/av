#pragma once

namespace RedAV
{
	namespace CLR
	{
		namespace Enumerators
		{
			public enum class AudioCodecWrapper : int
			{
				Unknown = 0,
				PCM_Signed16BitLittleEndian = 65536,
				PCM_Signed16BitBigEndian = 65537,
				PCM_Unsigned16BitLittleEndian = 65538,	// Note: Not supported in WAVE format
				PCM_Unsigned16BitBigEndian = 65539,		// Note: Not supported in WAVE format
				PCM_MuLaw = 65542,
				PCM_ALaw = 65543,
				AdaptivePCM_Ima_SdlMotionJpeg = 69637,
				AdaptivePCM_G726 = 69643,
				AdaptivePCM_G722 = 69660,
				AdaptivePCM_Ima_Oki = 71681,
				AdaptivePCM_G726LittleEndian = 71684,
				AmrNarrowBand = 73728,					// Requires OpenCore AMR libraries
				AmrWideBand = 73729,					// Requires OpenCore AMR libraries
				Mp2 = 86016,
				Mp3 = 86017,
				Aac = 86018,
				Vorbis = 86021,
				Flac = 86028,
				Gsm = 86034,
				Cook_G722_1 = 86036,					// Equivalent to G.722.1
				Gsm_Ms = 86046,
				Speex = 86051,
				G723_1 = 86068,
				G729 = 86069,
				InternetLowBitRate = 86075,
				Opus = 86076
			};
		}
	}
}