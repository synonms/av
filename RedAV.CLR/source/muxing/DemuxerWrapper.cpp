#include "DemuxerWrapper.h"

#include <utilities\TypeHelper.h>

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Runtime::InteropServices;
using namespace RedAV::CLR::Media;
using namespace RedAV::CLR::Muxing;
using namespace RedAV::CLR::Utilities;

DemuxerWrapper::DemuxerWrapper()
{
	demuxer_ = new redav::muxing::Demuxer();
}

DemuxerWrapper::~DemuxerWrapper()
{
	this->!DemuxerWrapper();
}

DemuxerWrapper::!DemuxerWrapper()
{
	delete demuxer_;
}

void DemuxerWrapper::Close()
{
	demuxer_->Close();
}

void DemuxerWrapper::ReadPackets()
{
	typedef void(__stdcall * nativeFunction)(redav::media::Packet*);

	auto packetReadDelegate = gcnew PacketReadDelegate(this, &DemuxerWrapper::OnPacketRead);
	auto delegatePointer = Marshal::GetFunctionPointerForDelegate(packetReadDelegate);
	auto functionPointer = static_cast<nativeFunction>(delegatePointer.ToPointer());

	demuxer_->ForEachPacket(functionPointer);
}

const List<StreamWrapper^>^ DemuxerWrapper::GetStreams()
{
	List<StreamWrapper^>^ streams = gcnew List<StreamWrapper^>();

	for(const auto& stream : demuxer_->GetStreams())
	{
		streams->Add(gcnew StreamWrapper(&stream));
	}

	return streams;
}

void DemuxerWrapper::Open(System::String^ filePath)
{
	demuxer_->Open(TypeHelper::ToNativeString(filePath));
}

void DemuxerWrapper::OnPacketRead(redav::media::Packet* packet)
{
	auto packetWrapper = gcnew PacketWrapper(packet);

	PacketReadEvent(packetWrapper);
}
