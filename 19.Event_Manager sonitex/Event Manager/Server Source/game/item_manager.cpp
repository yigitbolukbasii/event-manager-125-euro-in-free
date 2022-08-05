/*
	This is an example, there are already code segments which control
	drop of the moonlighht chest and hexegonal box (flags 'drop_moon' & '2006_drop')
	in the ITEM_MANAGER::CreateQuestDropItem.
	
	You can change their game flags instead of adding the following!
*/

//Search: 
void ITEM_MANAGER::CreateQuestDropItem(LPCHARACTER pkChr, LPCHARACTER pkKiller, std::vector<LPITEM> & vec_item, int iDeltaPercent, int iRandRange)

//Add at the end of the function:
#ifdef __EVENT_MANAGER__
	if (quest::CQuestManager::instance().GetEventFlag("moon_drop"))
	{
		const DWORD dwVnum = 50011;
		int iPercent = 5;
		if (iPercent <= number(1, 100))
		{
			if ((item = CreateItem(dwVnum, 1, 0, true)))
				vec_item.push_back(item);
		}
			
	}

	if (quest::CQuestManager::instance().GetEventFlag("hexegonal_drop"))
	{
		const DWORD dwVnum = 50037;
		int iPercent = 5;
		if (iPercent <= number(1, 100))
		{
			if ((item = CreateItem(dwVnum, 1, 0, true)))
				vec_item.push_back(item);
		}

	}
#endif