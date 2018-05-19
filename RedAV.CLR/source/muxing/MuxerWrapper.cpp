#include "MuxerWrapper.h"

#include <utilities\TypeHelper.h>

using namespace RedAV::CLR::Enumerators;
using namespace RedAV::CLR::Media;
using namespace RedAV::CLR::Muxing;
using namespace RedAV::CLR::Utilities;

MuxerWrapper::MuxerWrapper()
{
	muxer_ = new redav::muxing::Muxer();
}

MuxerWrapper::~MuxerWrapper()
{
	this->!MuxerWrapper();
}

MuxerWrapper::!MuxerWrapper()
{
	delete muxer_;
}

void MuxerWrapper::Close()
{
	muxer_->Close();
}

void MuxerWrapper::Open(System::String^ filePath, AudioCodecWrapper audioCodecType, bool isInterleaved)
{
	muxer_->Open(TypeHelper::ToNativeString(filePath), static_cast<redav::enumerators::AudioCodec>(audioCodecType), isInterleaved);
}

void MuxerWrapper::WritePacket(PacketWrapper^ packet)
{
	muxer_->WritePacket(packet->GetPacket());
}
