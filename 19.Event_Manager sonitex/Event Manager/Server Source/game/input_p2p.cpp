//Add:
#ifdef __EVENT_MANAGER__
#include "event_manager.h"
#endif


//Add:
#ifdef __EVENT_MANAGER__
void CInputP2P::Event(const char* c_pData)
{
	TPacketGGEvent* p = (TPacketGGEvent*)c_pData;
	CEventManager::Instance().SetEventState(&p->table, p->bState);
}
#endif

//Search:
		case HEADER_GG_CHECK_AWAKENESS:
			IamAwake(d, c_pData);
			break;
			
//Add below:
#ifdef __EVENT_MANAGER__
		case HEADER_GG_EVENT_RELOAD:
			BroadcastEventReload();
			break;
		case HEADER_GG_EVENT:
			Event(c_pData);
			break;
#endif