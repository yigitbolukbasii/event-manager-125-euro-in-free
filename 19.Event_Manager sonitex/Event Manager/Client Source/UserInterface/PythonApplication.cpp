//Search:
bool LoadLocaleData(const char* localePath)
{
	[...]
	if (!rkNetStream.LoadInsultList(szInsultList))
	{
		Tracenf("CPythonApplication - CPythonNetworkStream::LoadInsultList(%s)", szInsultList);				
	}

	
//Add below:
#ifdef ENABLE_EVENT_MANAGER
	InGameEventManager& rkEventMgr = InGameEventManager::Instance();
	char szRewardList[256];
	snprintf(szRewardList, sizeof(szRewardList), "%s/event_reward_list.txt", localePath);

	if (!rkEventMgr.LoadEventRewardList(szRewardList))
	{
		TraceError("LoadLocaleData - LoadEventRewardList(%s) Error", szRewardList);
		return false;
	}
#endif