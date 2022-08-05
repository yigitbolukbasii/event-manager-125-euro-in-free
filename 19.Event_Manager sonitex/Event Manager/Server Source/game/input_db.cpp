//Add:
#ifdef __EVENT_MANAGER__
#include "event_manager.h"
#endif


//Search:
void CInputDB::Boot(const char* data)
{
	[...]
	sys_log(0, "sizeof(TObject) = %d", sizeof(building::TObject));
	
//Add below:
#ifdef __EVENT_MANAGER__
	sys_log(0, "sizeof(TEventTable) = %d", sizeof(TEventTable));
#endif


//Search:
		/*
		 * OBJECT 
		 */
		if (decode_2bytes(data) != sizeof(TObject))
		{
			sys_err("object table size error");
			thecore_shutdown();
			return;
		}
		data += 2;

		size = decode_2bytes(data);
		data += 2;

		TObject * kObj = (TObject *) data;
		data += size * sizeof(TObject);

		for (WORD i = 0; i < size; ++i, ++kObj)
			CManager::instance().LoadObject(kObj, true);
	}
	
//Add below:
#ifdef __EVENT_MANAGER__
	/*
	* EVENT
	*/

	if (decode_2bytes(data) != sizeof(TEventTable))
	{
		sys_err("event table size error");
		thecore_shutdown();
		return;
	}
	data += 2;

	size = decode_2bytes(data);
	data += 2;
	sys_log(0, "BOOT: EVENT: %d", size);

	if (size)
	{
		CEventManager::instance().Initialize((TEventTable*)data, size);
		data += size * sizeof(TEventTable);
	}
#endif


//Search:
void CInputDB::ReloadProto(const char * c_pData)
{
	[...]
	/*
	 * MONSTER
	 */
	wSize = decode_2bytes(c_pData);
	c_pData += 2;
	sys_log(0, "RELOAD: MOB: %d", wSize);

	if (wSize)
	{
		CMobManager::instance().Initialize((TMobTable *) c_pData, wSize);
		c_pData += wSize * sizeof(TMobTable);
	}
	
	
//Add below:
#ifdef __EVENT_MANAGER__
	/*
	* EVENT
	*/
	wSize = decode_2bytes(c_pData);
	c_pData += 2;
	sys_log(0, "RELOAD: Event: %d", wSize);

	if (wSize)
	{
		CEventManager::instance().Initialize((TEventTable*)c_pData, wSize, true);
		c_pData += wSize * sizeof(TEventTable);
	}
#endif


//Search:
	case HEADER_DG_RESPOND_CHANNELSTATUS:
		RespondChannelStatus(DESC_MANAGER::instance().FindByHandle(m_dwHandle), c_pData);
		break;
		
//Add below:
#ifdef __EVENT_MANAGER__
	case HEADER_DG_UPDATE_EVENT_STATUS:
		CEventManager::Instance().UpdateEventStatus((DWORD)c_pData);

	case HEADER_DG_EVENT_NOTIFICATION:
		EventNotification(c_pData);
		break;
#endif


//Add:
#ifdef __EVENT_MANAGER__
void CInputDB::EventNotification(const char* c_pData)
{
	TPacketSetEventFlag* p = (TPacketSetEventFlag*)c_pData;
	quest::CQuestManager::instance().SetEventFlag(p->szFlagName, p->lValue);

	if (p->lValue)
		SendEventBeginNotification();
	else
		SendEventEndNotification();
}
#endif