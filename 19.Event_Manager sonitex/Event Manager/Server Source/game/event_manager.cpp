#include "stdafx.h"
#include "event.h"
#include "event_manager.h"
#include "text_file_loader.h"
#include "locale_service.h"
#include "quest.h"
#include "questmanager.h"
#include "priv_manager.h"
#include "sectree_manager.h"
#include "start_position.h"
#include "char.h"
#include "regen.h"
#include "config.h"
#include "desc_client.h"
#include "desc_manager.h"
#include "utils.h"
#include "buffer_manager.h"
#include "p2p.h"

CEventManager::CEventManager()
{
	m_mapEventName.clear();
	m_mapEvent.clear();
	m_mapEventStartQueue.clear();

	BuildEventNameMap();

	m_bReload = false;
}

CEventManager::~CEventManager()
{
}

EVENTINFO(warp_all_to_village_event_info)
{
	DWORD dwWarpMapIndex;

	warp_all_to_village_event_info()
		: dwWarpMapIndex(0)
	{
	}
};

struct FWarpAllToVillage
{
	FWarpAllToVillage() {};
	void operator()(LPENTITY ent)
	{
		if (ent->IsType(ENTITY_CHARACTER))
		{
			LPCHARACTER ch = (LPCHARACTER)ent;
			if (ch->IsPC())
			{
				BYTE bEmpire = ch->GetEmpire();
				if (bEmpire == 0)
				{
					sys_err("Unkonwn Empire %s %d ", ch->GetName(), ch->GetPlayerID());
					return;
				}
		
				ch->WarpSet(g_start_position[bEmpire][0], g_start_position[bEmpire][1]);
			}
		}
	}
};

EVENTFUNC(warp_all_to_village_event)
{
	warp_all_to_village_event_info* info = dynamic_cast<warp_all_to_village_event_info*>(event->info);

	if (!info)
	{
		sys_err("warp_all_to_village_event> <Factor> Null pointer");
		return 0;
	}

	LPSECTREE_MAP pSecMap = SECTREE_MANAGER::instance().GetMap(info->dwWarpMapIndex);

	if (pSecMap)
	{
		FWarpAllToVillage f;
		pSecMap->for_each(f);
	}

	return 0;
}

struct FKillSectree
{
	void operator () (LPENTITY ent)
	{
		if (ent->IsType(ENTITY_CHARACTER))
		{
			LPCHARACTER ch = (LPCHARACTER)ent;
			if (ch->IsMonster() || ch->IsStone())
				ch->Dead();
		}
	}
};

EVENTINFO(dynamic_spawn_cycle_event_info)
{
	DWORD dwWarpMapIndex;
	std::string strRegenPath;
	std::string strRegenPath2;
	BYTE step;

	dynamic_spawn_cycle_event_info()
		: dwWarpMapIndex(0),
		strRegenPath(""),
		strRegenPath2(""),
		step(0)
	{
	}
};

EVENTFUNC(dynamic_spawn_cycle_event)
{
	dynamic_spawn_cycle_event_info* info = dynamic_cast<dynamic_spawn_cycle_event_info*>(event->info);

	if (!info)
	{
		sys_err("dynamic_spawn_cycle_event <Factor> Null pointer to event");
		return 0;
	}

	if (info->dwWarpMapIndex == 0 || info->strRegenPath == "" || info->strRegenPath2 == "")
	{
		sys_err("dynamic_spawn_cycle_event: Null pointer to event info");
		return 0;
	}

	LPSECTREE_MAP pSecMap = SECTREE_MANAGER::instance().GetMap(info->dwWarpMapIndex);

	if (pSecMap)
	{
		info->step++;

		/*
			Clear each entity on the map on the beggining of 1st and 6th round.
		*/
		if (info->step == CEventManager::EEvent::CLEAR_ENTITY_STAGE1_ROUND ||
			info->step == CEventManager::EEvent::CLEAR_ENTITY_STAGE2_ROUND)
		{
			FKillSectree f;
			pSecMap->for_each(f);
		}

		/*
			Round 1~5 -> Stage 1
			Round 6~10 -> Stage 2
			Exit after round 10
		*/
		if (info->step <= CEventManager::EEvent::DYNAMIC_CYCLE_STAGE1)
		{
			regen_load_in_file(info->strRegenPath.c_str(), info->dwWarpMapIndex, pSecMap->m_setting.iBaseX, pSecMap->m_setting.iBaseY);
		}
		else if (info->step >= CEventManager::EEvent::DYNAMIC_CYCLE_STAGE2 &&
			info->step <= CEventManager::EEvent::DYNAMIC_CYCLE_STAGE2)
		{
			regen_load_in_file(info->strRegenPath2.c_str(), info->dwWarpMapIndex, pSecMap->m_setting.iBaseX, pSecMap->m_setting.iBaseY);
		}
		else if (info->step > CEventManager::EEvent::DYNAMIC_CYCLE_STAGE2)
			return 0;
	}

	return PASSES_PER_SEC(5 * 60);
}

EVENTINFO(static_spawn_cycle_event_info)
{
	DWORD dwWarpMapIndex;
	std::string strRegenPath;
	BYTE step;

	static_spawn_cycle_event_info()
		: dwWarpMapIndex(0),
		strRegenPath(""),
		step(0)
	{
	}
};

EVENTFUNC(static_spawn_cycle_event)
{
	static_spawn_cycle_event_info* info = dynamic_cast<static_spawn_cycle_event_info*>(event->info);

	if (!info)
	{
		sys_err("dynamic_spawn_cycle_event <Factor> Null pointer to event");
		return 0;
	}

	if (info->dwWarpMapIndex == 0 || info->strRegenPath == "")
	{
		sys_err("dynamic_spawn_cycle_event: Null pointer to event info");
		return 0;
	}

	LPSECTREE_MAP pSecMap = SECTREE_MANAGER::instance().GetMap(info->dwWarpMapIndex);

	if (pSecMap)
	{
		info->step++;

		/*
			Clear each entity on the map on the beggining of 1st.
			Do not clear map during Tanaka Event.
		*/
		if (info->step == CEventManager::EEvent::CLEAR_ENTITY_STAGE1_ROUND &&
			info->dwWarpMapIndex != CEventManager::EEvent::TANAKA_EVENT_MAP)
		{
			FKillSectree f;
			pSecMap->for_each(f);
		}

		/*
			Round 1~3 -> Stage 1
			Exit after round 3
		*/
		if (info->step <= CEventManager::EEvent::STATIC_CYCLE_MAX_ROUND)
		{
			regen_load_in_file(info->strRegenPath.c_str(), info->dwWarpMapIndex, pSecMap->m_setting.iBaseX, pSecMap->m_setting.iBaseY);
		}
		else
			return 0;
	}

	return PASSES_PER_SEC(8 * 60); 
}

EVENTINFO(queue_event_info)
{
	CEventManager* eventManager;
	TEventTable table;
	BYTE	bQueue;

	queue_event_info() :
		eventManager(nullptr), bQueue(CEventManager::QUEUE_TYPE_NONE)
	{
		memset(&table, 0, sizeof(table));
	}
};

EVENTFUNC(queue_event_process)
{
	queue_event_info* info = dynamic_cast<queue_event_info*>(event->info);

	if (!info)
	{
		sys_err("queue_event_process> <Factor> Null pointer");
		return 0;
	}

	CEventManager* eventMgr = info->eventManager;

	if (info->bQueue == CEventManager::QUEUE_TYPE_START)
	{
		// Start the event and remove it from start-queue
		eventMgr->SetEventState(&info->table, true);
		eventMgr->Dequeue(CEventManager::QUEUE_TYPE_START, &info->table);
	}
	else if(info->bQueue == CEventManager::QUEUE_TYPE_END)
	{
		// End the event and remove it from end-queue
		eventMgr->SetEventState(&info->table, false);
		eventMgr->Dequeue(CEventManager::QUEUE_TYPE_END, &info->table);
	}

	return 0;
}

/*
	Function will be called at the initialization of events.
	Each game event will be added to start-event queue and 
	end-event queue (dependant on bQueue value).
*/
void CEventManager::Enqueue(BYTE bQueue, TEventTable* table)
{
	queue_event_info* info = AllocEventInfo<queue_event_info>();
	info->eventManager = this;
	info->table = *table;

	if (bQueue == QUEUE_TYPE_START)
	{
		info->bQueue = QUEUE_TYPE_START;
		long startTime = table->startTime - get_global_time();

		if (startTime <= 0)
			SetEventState(table, true);
		else
			m_mapEventStartQueue.insert(EventQueue::value_type(table->dwID, event_create(queue_event_process, info, PASSES_PER_SEC(startTime))));
	}
	else if(bQueue == QUEUE_TYPE_END)
	{
		info->bQueue = QUEUE_TYPE_END;
		long endTime = table->endTime - get_global_time();

		m_mapEventEndQueue.insert(EventQueue::value_type(table->dwID, event_create(queue_event_process, info, PASSES_PER_SEC(endTime))));
	}
}

/*
	Events are dequeued from their respective map on start/end.
	At the end of an event, a packet is sent to database to update
	event completion status. Database then sents a packet back
	 to update information on every game core.
*/
void CEventManager::Dequeue(BYTE bQueue, TEventTable* table)
{
	if (bQueue == QUEUE_TYPE_START)
	{
		auto queueIt = m_mapEventStartQueue.find(table->dwID);
		if (queueIt != m_mapEventStartQueue.end())
		{
			event_cancel(&queueIt->second);
			m_mapEventStartQueue.erase(queueIt);
		}
	}
	else if(bQueue == QUEUE_TYPE_END)
	{
		auto queueIt = m_mapEventEndQueue.find(table->dwID);
		if (queueIt != m_mapEventEndQueue.end())
		{
			event_cancel(&queueIt->second);
			m_mapEventEndQueue.erase(queueIt);
		}

		auto eventIt = m_mapEvent.find(table->dwID);
		if (eventIt != m_mapEvent.end())
		{
			db_clientdesc->DBPacket(HEADER_GD_UPDATE_EVENT_STATUS, 0, &eventIt->second.dwID, sizeof(DWORD));
		}
	}
}

void CEventManager::CancelActiveEvents()
{
	for (auto &kv : m_mapEvent)
	{
		DWORD dwIndex = kv.first;

		auto itStart = m_mapEventStartQueue.find(dwIndex);
		bool bStartFound = (itStart != m_mapEventStartQueue.end());

		auto itEnd = m_mapEventEndQueue.find(dwIndex);
		bool bEndFound = (itEnd != m_mapEventEndQueue.end());

		/* 
			If an event is only present in the end queue, it means it has already started.
			Event must be ended to reset the game flag for future start.
		*/
		if (!bStartFound && bEndFound)
		{
			int iEvent = GetEvent(kv.second.szType);

			SetEventState(&kv.second, false);
		}
	}

	// Cancel and remove previous events incase we are reloading
	for (auto kv : m_mapEventStartQueue)
		event_cancel(&kv.second);

	for (auto kv : m_mapEventEndQueue)
		event_cancel(&kv.second);

	m_mapEvent.clear();
	m_mapEventStartQueue.clear();
	m_mapEventEndQueue.clear();
}
void CEventManager::Initialize(TEventTable* pTab, int iSize, bool bReload /* = false */)
{
	if (bReload)
	{
		SetReloadMode(true);
		CancelActiveEvents();
	}

	for (int i = 0; i < iSize; ++i, ++pTab)
	{
		if (!GetEvent(pTab->szType))
		{
			sys_err("Skipping event table id %d, unknown event type %s", pTab->dwID, pTab->szType);
			continue;
		}

		TEventTable t;
		t.dwID = pTab->dwID;
		strlcpy(t.szType, pTab->szType, sizeof(t.szType));
		t.startTime = pTab->startTime;
		t.endTime = pTab->endTime;
		t.iValue0 = pTab->iValue0;
		t.iValue1 = pTab->iValue1;
		t.bCompleted = pTab->bCompleted;

		m_mapEvent.emplace(std::make_pair(t.dwID, t));

		// Event Queues are initialized on only one specific game channel
		if (g_bChannel == EVENT_CHANNEL && !t.bCompleted)
		{
			// Add event to the start queue
			Enqueue(QUEUE_TYPE_START, &t);

			// Add event to the end queue
			Enqueue(QUEUE_TYPE_END, &t);
		}
	}

	// Send a packet to reload event board on client-side incase it was a re-load
	if ((g_bChannel == EVENT_CHANNEL) && bReload)
		ReloadEvent();

	SetReloadMode(false);
}

void CEventManager::UpdateEventStatus(DWORD dwID)
{
	auto eventIt = m_mapEvent.find(dwID);

	if (eventIt != m_mapEvent.end())
		eventIt->second.bCompleted = true;
}

int CEventManager::GetEvent(const std::string strEventName)
{
	if (m_mapEventName.find(strEventName) != m_mapEventName.end())
		return m_mapEventName[strEventName];

	return EVENT_TYPE_NONE;
}

std::string CEventManager::GetEventString(int iEvent)
{
	for (auto kv : m_mapEventName)
	{
		if (kv.second == iEvent)
			return kv.first;
	}

	return "EVENT_TYPE_NONE";
}

void CEventManager::SendEventInfo(const LPCHARACTER ch, int iMonth)
{
	TEMP_BUFFER buf;

	for (const auto& kv : m_mapEvent)
	{
		const time_t startTime = kv.second.startTime;
		const struct tm* tStart = localtime(&startTime);

		// Send current month's events and events from previous months which are still active
		if (((tStart->tm_mon < iMonth) && !kv.second.bCompleted) || (tStart->tm_mon == iMonth))
		{
			TPacketEventData eventData;
			eventData.dwID = kv.second.dwID;
			eventData.bType = GetEvent(kv.second.szType);
			eventData.startTime = kv.second.startTime;
			eventData.endTime = kv.second.endTime;
			eventData.iValue0 = kv.second.iValue0;
			eventData.iValue1 = kv.second.iValue1;
			eventData.bCompleted = kv.second.bCompleted;

			buf.write(&eventData, sizeof(TPacketEventData));
		}
	}

	TPacketGCEventInfo p;
	p.bHeader = HEADER_GC_EVENT_INFO;
	p.iSize = sizeof(TPacketGCEventInfo) + buf.size();

	ch->GetDesc()->Packet(&p, sizeof(TPacketGCEventInfo));

	if(buf.size())
		ch->GetDesc()->Packet(buf.read_peek(), buf.size());
}

void CEventManager::BuildEventNameMap()
{
	m_mapEventName["EVENT_TYPE_NONE"] = EVENT_TYPE_NONE;
	m_mapEventName["EVENT_TYPE_EXPERIENCE"] = EVENT_TYPE_EXPERIENCE;
	m_mapEventName["EVENT_TYPE_ITEM_DROP"] = EVENT_TYPE_ITEM_DROP;
	m_mapEventName["EVENT_TYPE_BOSS"] = EVENT_TYPE_BOSS;
	m_mapEventName["EVENT_TYPE_METIN"] = EVENT_TYPE_METIN;
	m_mapEventName["EVENT_TYPE_MINING"] = EVENT_TYPE_MINING;
	m_mapEventName["EVENT_TYPE_GOLD_FROG"] = EVENT_TYPE_GOLD_FROG;
	m_mapEventName["EVENT_TYPE_MOONLIGHT"] = EVENT_TYPE_MOONLIGHT;
	m_mapEventName["EVENT_TYPE_HEXEGONAL_CHEST"] = EVENT_TYPE_HEXEGONAL_CHEST;
	m_mapEventName["EVENT_TYPE_FISHING"] = EVENT_TYPE_FISHING;
	m_mapEventName["EVENT_TYPE_HIDE_AND_SEEK"] = EVENT_TYPE_HIDE_AND_SEEK;
	m_mapEventName["EVENT_TYPE_OX"] = EVENT_TYPE_OX;
	m_mapEventName["EVENT_TYPE_TANAKA"] = EVENT_TYPE_TANAKA;
}

bool CEventManager::GetEventState(int iEventType)
{
	switch (iEventType)
	{
		case EVENT_TYPE_EXPERIENCE:
			return quest::CQuestManager::instance().GetEventFlag("exp_event");
			break;
		case EVENT_TYPE_ITEM_DROP:
			return quest::CQuestManager::instance().GetEventFlag("item_drop_event");
			break;
		case EVENT_TYPE_BOSS:
			return quest::CQuestManager::instance().GetEventFlag("boss_event");
			break;
		case EVENT_TYPE_METIN:
			return quest::CQuestManager::instance().GetEventFlag("metin_event");
			break;
		case EVENT_TYPE_MINING:
			return quest::CQuestManager::instance().GetEventFlag("mining_event");
			break;
		case EVENT_TYPE_GOLD_FROG:
			return quest::CQuestManager::instance().GetEventFlag("golden_frog_event");
			break;
		case EVENT_TYPE_MOONLIGHT:
			return quest::CQuestManager::instance().GetEventFlag("moon_drop");
			break;
		case EVENT_TYPE_HEXEGONAL_CHEST:
			return quest::CQuestManager::instance().GetEventFlag("hexegonal_drop");
			break;
		case EVENT_TYPE_FISHING:
			return quest::CQuestManager::instance().GetEventFlag("fish_event");
			break;
		case EVENT_TYPE_HIDE_AND_SEEK:
			return quest::CQuestManager::instance().GetEventFlag("hide_seek_event");
			break;
		case EVENT_TYPE_OX:
			return quest::CQuestManager::instance().GetEventFlag("oxevent_status");
			break;
		case EVENT_TYPE_TANAKA:
			return quest::CQuestManager::instance().GetEventFlag("tanaka_event");
			break;
		default:
			return false;
			break;
	}
}

void CEventManager::SetEventState(TEventTable* table, bool bState)
{
	int iEvent = GetEvent(table->szType);

	/* 
		Return if the event state is already set, ignore
		if it is not a reload -> game boot
	*/
	if ((GetEventState(iEvent) == bState) && !GetReloadMode())
		return;

	bool ret = false;

	switch (iEvent)
	{
		case EVENT_TYPE_EXPERIENCE:
			ret = SetExperienceEvent(bState, table);
			break;
		case EVENT_TYPE_ITEM_DROP:
			ret = SetItemDropEvent(bState, table);
			break;
		case EVENT_TYPE_BOSS:
			ret = SetBossEvent(bState);
			break;
		case EVENT_TYPE_METIN:
			ret = SetMetinEvent(bState);
			break;
		case EVENT_TYPE_MINING:
			ret = SetMiningEvent(bState);
			break;
		case EVENT_TYPE_GOLD_FROG:
			ret = SetGoldFrogEvent(bState);
			break;
		case EVENT_TYPE_MOONLIGHT:
			ret = SetMoonlightEvent(bState);
			break;
		case EVENT_TYPE_HEXEGONAL_CHEST:
			ret = SetHexegonalEvent(bState);
			break;
		case EVENT_TYPE_FISHING:
			ret = SetFishingEvent(bState);
			break;
		case EVENT_TYPE_HIDE_AND_SEEK:
			ret = SetHideAndSeekEvent(bState);
			break;
		case EVENT_TYPE_OX:
			ret = SetOXEvent(bState);
			break;
		case EVENT_TYPE_TANAKA:
			ret = SetTanakaEvent(bState, table);
			break;
	}

	if (!ret)
		sys_err("Event %s could not started/stopped, state %d", table->szType, bState);
}

/*
	When reloading, game requires instant information about
	game flags. Waiting for it to retrieve from database is not an
	option so we are updating this core's game flag directly while
	also sending a packet to update db.
*/
void CEventManager::UpdateGameFlag(const char* c_szFlag, bool bState)
{
	quest::CQuestManager::instance().SetEventFlag(c_szFlag, bState);

	TPacketSetEventFlag p;
	strlcpy(p.szFlagName, c_szFlag, sizeof(p.szFlagName));
	p.lValue = long(bState);
	db_clientdesc->DBPacket(HEADER_GD_EVENT_NOTIFICATION, 0, &p, sizeof(TPacketSetEventFlag));
}

bool CEventManager::SetExperienceEvent(bool bActive, TEventTable* table)
{
	if (bActive && (table->iValue0 == 0))
	{
		sys_err("CEventManager::SetExperienceEvent: rate is set to null.");
		return false;
	}

	UpdateGameFlag("exp_event", bActive);

	if (bActive)
	{
		long time = table->endTime - table->startTime;
		if (!time)
		{
			sys_err("CEventManager::SetExperienceEvent: time is set to null.");
			return false;
		}

		BroadcastNotice("Experience Boost Event has started. Enjoy the leveling.");
		CPrivManager::instance().RequestGiveEmpirePriv(ALL_EMPIRES, PRIV_EXP_PCT, table->iValue0, time);
	}
	else
	{
		BroadcastNotice("Experience Boost Event has ended.");
	}

	return true;
}

bool CEventManager::SetItemDropEvent(bool bActive, TEventTable* table)
{
	if (bActive && (table->iValue0 == 0))
	{
		sys_err("CEventManager::SetItemDropEvent: rate is set to null.");
		return false;
	}

	UpdateGameFlag("item_drop_event", bActive);

	if (bActive)
	{
		long time = table->endTime - table->startTime;
		if (!time)
		{
			sys_err("CEventManager::SetExperienceEvent: time is set to null.");
			return false;
		}

		BroadcastNotice("Item-Drop Boost Event has started. Enjoy the grinding.");
		CPrivManager::instance().RequestGiveEmpirePriv(ALL_EMPIRES, PRIV_ITEM_DROP, table->iValue0, time);
	}
	else
		BroadcastNotice("Item-Drop Boost Event has ended.");

	return true;
}

bool CEventManager::SetBossEvent(bool bActive)
{
	UpdateGameFlag("boss_event", bActive);

	if (bActive)
	{
		LPSECTREE_MAP pSecMap = SECTREE_MANAGER::instance().GetMap(EVENT_MAP_INDEX);

		if (!pSecMap)
			return false;

		dynamic_spawn_cycle_event_info * info = AllocEventInfo<dynamic_spawn_cycle_event_info>();
		info->dwWarpMapIndex = EVENT_MAP_INDEX;
		info->strRegenPath = "data/event/boss_event_regen_type_1.txt";
		info->strRegenPath2 = "data/event/boss_event_regen_type_2.txt";

		event_create(dynamic_spawn_cycle_event, info, PASSES_PER_SEC(1));

		BroadcastNotice("Boss Hunt Event has started. Open the Quest Scroll to join.");
	}
	else
	{
		BroadcastNotice("Boss Hunt Event has ended. Thank you for participating.");

		LPSECTREE_MAP pSecMap = SECTREE_MANAGER::instance().GetMap(EVENT_MAP_INDEX);

		if (!pSecMap)
			return false;

		warp_all_to_village_event_info* info = AllocEventInfo<warp_all_to_village_event_info>();

		info->dwWarpMapIndex = EVENT_MAP_INDEX;
		event_create(warp_all_to_village_event, info, PASSES_PER_SEC(60));

		SendNoticeMap(LC_TEXT("Everyone will be teleported into the city shortly."), EVENT_MAP_INDEX, false);

		FKillSectree f;
		pSecMap->for_each(f);
	}
	return true;
}

bool CEventManager::SetMetinEvent(bool bActive)
{
	UpdateGameFlag("metin_event", bActive);

	if (bActive)
	{
		LPSECTREE_MAP pSecMap = SECTREE_MANAGER::instance().GetMap(EVENT_MAP_INDEX);

		if (!pSecMap)
			return false;

		dynamic_spawn_cycle_event_info * info = AllocEventInfo<dynamic_spawn_cycle_event_info>();
		info->dwWarpMapIndex = EVENT_MAP_INDEX;
		info->strRegenPath = "data/event/metin_event_regen_type_1.txt";
		info->strRegenPath2 = "data/event/metin_event_regen_type_2.txt";

		event_create(dynamic_spawn_cycle_event, info, PASSES_PER_SEC(1));

		BroadcastNotice("Metin Shower Event has started. Open the Quest Scroll to join.");
	}
	else
	{
		BroadcastNotice("Metin Shower Event has ended. Thank you for participating.");
		LPSECTREE_MAP pSecMap = SECTREE_MANAGER::instance().GetMap(EVENT_MAP_INDEX);

		if (!pSecMap)
			return false;

		warp_all_to_village_event_info* info = AllocEventInfo<warp_all_to_village_event_info>();

		info->dwWarpMapIndex = EVENT_MAP_INDEX;
		event_create(warp_all_to_village_event, info, PASSES_PER_SEC(15));

		SendNoticeMap(LC_TEXT("Everyone will be teleported into the city shortly."), EVENT_MAP_INDEX, false);

		FKillSectree f;
		pSecMap->for_each(f);
	}

	return true;
}

bool CEventManager::SetMiningEvent(bool bActive)
{
	UpdateGameFlag("mining_event", bActive);

	if (bActive)
	{

		LPSECTREE_MAP pSecMap = SECTREE_MANAGER::instance().GetMap(EVENT_MAP_INDEX);

		if (!pSecMap)
			return false;

		static_spawn_cycle_event_info * info = AllocEventInfo<static_spawn_cycle_event_info>();
		info->dwWarpMapIndex = EVENT_MAP_INDEX;
		info->strRegenPath = "data/event/mining_event_regen_type_0.txt";

		event_create(static_spawn_cycle_event, info, PASSES_PER_SEC(1));

		BroadcastNotice("Boss Hunt Event has started. Open the Quest Scroll to join.");

	}
	else
	{
		BroadcastNotice("Boss Hunt Event has ended. Thank you for participating.");
		LPSECTREE_MAP pSecMap = SECTREE_MANAGER::instance().GetMap(EVENT_MAP_INDEX);

		if (!pSecMap)
			return false;

		warp_all_to_village_event_info* info = AllocEventInfo<warp_all_to_village_event_info>();

		info->dwWarpMapIndex = EVENT_MAP_INDEX;
		event_create(warp_all_to_village_event, info, PASSES_PER_SEC(15));

		SendNoticeMap(LC_TEXT("Everyone will be teleported into the city shortly."), EVENT_MAP_INDEX, false);

		FKillSectree f;
		pSecMap->for_each(f);
	}

	return true;
}

bool CEventManager::SetGoldFrogEvent(bool bActive)
{
	UpdateGameFlag("golden_frog_event", bActive);

	if (bActive)
	{
		LPSECTREE_MAP pSecMap = SECTREE_MANAGER::instance().GetMap(EVENT_MAP_INDEX);

		if (!pSecMap)
			return false;

		static_spawn_cycle_event_info * info = AllocEventInfo<static_spawn_cycle_event_info>();
		info->dwWarpMapIndex = EVENT_MAP_INDEX;
		info->strRegenPath = "data/event/golden_frog_event_regen_type_0.txt";

		event_create(static_spawn_cycle_event, info, PASSES_PER_SEC(1));
		BroadcastNotice("Golden Frog Event has started. Open the Quest Scroll to join.");

	}
	else
	{
		BroadcastNotice("Golden Frog Event has ended.Thank you for participating.");

		LPSECTREE_MAP pSecMap = SECTREE_MANAGER::instance().GetMap(EVENT_MAP_INDEX);

		if (!pSecMap)
			return false;

		warp_all_to_village_event_info* info = AllocEventInfo<warp_all_to_village_event_info>();

		info->dwWarpMapIndex = EVENT_MAP_INDEX;
		event_create(warp_all_to_village_event, info, PASSES_PER_SEC(15));

		SendNoticeMap(LC_TEXT("Everyone will be teleported into the city shortly."), EVENT_MAP_INDEX, false);

		FKillSectree f;
		pSecMap->for_each(f);
	}

	return true;
}

bool CEventManager::SetMoonlightEvent(bool bActive)
{
	UpdateGameFlag("moon_drop", bActive);

	if (bActive)
	{
		BroadcastNotice("Moonlight Chest Event has started! Defeat enemies and grab your rewards!");
	}
	else
	{
		BroadcastNotice("Moonlight Chest Event has ended!");
	}

	return true;
}

bool CEventManager::SetHexegonalEvent(bool bActive)
{
	UpdateGameFlag("hexegonal_drop", bActive);

	if (bActive)
	{
		BroadcastNotice("Hexegonal Box Event has started! Defeat enemies and grab your rewards!");
	}
	else
	{
		BroadcastNotice("Hexegonal Box Event has ended!");
	}

	return true;
}

bool CEventManager::SetFishingEvent(bool bActive)
{
	UpdateGameFlag("fish_event", bActive);

	if (bActive)
	{
		BroadcastNotice("Fishing Event has started. Bring Fishbones to the Fisherman and receive your reward.");
	}
	else
	{
		BroadcastNotice("Hide and Seek Event has ended. Thank you for participating!");
	}

	return true;
}

bool CEventManager::SetHideAndSeekEvent(bool bActive)
{
	UpdateGameFlag("hide_seek_event", bActive);

	if (bActive)
	{
		BroadcastNotice("Hide and Seek Event has started. Find the Team Member by following given hints.");
	}
	else
	{
		BroadcastNotice("Fishing Event has ended.");
	}

	return true;
}

bool CEventManager::SetOXEvent(bool bActive)
{
	//UpdateGameFlag("oxevent_status", bActive);

	if (bActive)
	{
		BroadcastNotice("OX Event will be starting soon, prepare yourselves.");
	}
	else
	{
		BroadcastNotice("OX Event has ended. Thank you for participating!");
	}

	return true;
}

bool CEventManager::SetTanakaEvent(bool bActive, TEventTable* table)
{
	if (!map_allow_find(TANAKA_EVENT_MAP))
	{
		if (g_bChannel == EVENT_CHANNEL)
		{
			TPacketGGEvent packet;
			packet.bHeader = HEADER_GG_EVENT;
			packet.table = *table;
			packet.bState = bActive;

			P2P_MANAGER::instance().Send(&packet, sizeof(packet));
		}

		return true;
	}

	UpdateGameFlag("tanaka_event", bActive);

	if (bActive)
	{

		LPSECTREE_MAP pSecMap = SECTREE_MANAGER::instance().GetMap(TANAKA_EVENT_MAP);

		if (!pSecMap)
			return false;

		static_spawn_cycle_event_info * info = AllocEventInfo<static_spawn_cycle_event_info>();
		info->dwWarpMapIndex = TANAKA_EVENT_MAP;
		info->strRegenPath = "data/event/tanaka_event_regen_type_0.txt";

		event_create(static_spawn_cycle_event, info, PASSES_PER_SEC(1));

		BroadcastNotice("Pirate Tanaka Event has started. Tanakas took over the desert, hunt them down!");

	}
	else
	{
		BroadcastNotice("Pirate Tanaka Event has ended. Thank you for participating!");
	}

	return true;
}