//Add:
#ifdef __EVENT_MANAGER__
#include "event_manager.h"
#endif

//Add:
#ifdef __EVENT_MANAGER__
	int _start_event(lua_State* L)
	{
		if (false == lua_isnumber(L, 1))
		{
			lua_pushnumber(L, 0);
			return 1;
		}

		int eventIdx = lua_tonumber(L, 1);

		int value0 = 0;
		int value1 = 0;

		if(lua_isnumber(L, 2))
			value0 = lua_tonumber(L, 2);

		if (lua_isnumber(L, 3))
			value1 = lua_tonumber(L, 3);

		TEventTable table;
		memset(&table, 0, sizeof(table));

		std::string eventName = CEventManager::Instance().GetEventString(eventIdx);
		strlcpy(table.szType, eventName.c_str(), sizeof(table.szType));
		table.iValue0 = value0;
		table.iValue1 = value1;


		CEventManager::Instance().SetEventState(&table, true);
		
		return 0;
	}

	int _end_event(lua_State* L)
	{
		if (false == lua_isnumber(L, 1))
		{
			lua_pushnumber(L, 0);
			return 1;
		}

		int eventIdx = lua_tonumber(L, 1);

		TEventTable table;
		memset(&table, 0, sizeof(table));

		std::string eventName = CEventManager::Instance().GetEventString(eventIdx);
		strlcpy(table.szType, eventName.c_str(), sizeof(table.szType));

		CEventManager::Instance().SetEventState(&table, false);
		
		return 0;
	}
#endif


//Search:
			{	"warp_all_in_area_to_area",		_warp_all_in_area_to_area		},

//Add below:
#ifdef __EVENT_MANAGER__
			{	"start_event",					_start_event					},
			{	"end_event",					_end_event						},
#endif