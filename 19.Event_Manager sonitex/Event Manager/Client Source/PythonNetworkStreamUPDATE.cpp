//Search:


//Add below:
#ifdef ENABLE_EVENT_MANAGER
			Set(HEADER_GC_EVENT_INFO, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCEventInfo), DYNAMIC_SIZE_PACKET));
			Set(HEADER_GC_EVENT_RELOAD, CNetworkPacketHeaderMap::TPacketType(sizeof(TPacketGCEventReload), STATIC_SIZE_PACKET));
#endif