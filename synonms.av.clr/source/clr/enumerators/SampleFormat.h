#pragma once

namespace Synonms
{
	namespace Av
	{
		namespace Clr
		{
			namespace Enumerators
			{
				public enum class SampleFormat : int
				{
					None = -1,
					Unsigned8Bit,
					Signed16Bit,
					Signed32Bit,
					Float,
					Double,
					Unsigned8BitPlanar,
					Signed16BitPlanar,
					Signed32BitPlanar,
					FloatPlanar,
					DoublePlanar,
					Signed64Bit,
					Signed64BitPlanar
				};
			}
		}
	}
}