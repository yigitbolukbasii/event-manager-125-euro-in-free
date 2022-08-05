//Add

#ifdef ENABLE_EVENT_MANAGER
	PyModule_AddIntConstant(poModule, "ENABLE_EVENT_MANAGER", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_EVENT_MANAGER", 0);
#endif