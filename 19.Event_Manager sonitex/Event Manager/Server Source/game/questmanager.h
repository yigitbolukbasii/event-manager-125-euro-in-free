//Search:
			void		Unmount(unsigned int pc);

//Add below:
#ifdef __EVENT_MANAGER__
			void		EventBegin(unsigned int pc);
			void		EventEnd(unsigned int pc);
#endif


//Search:
			void		ItemInformer(unsigned int pc, unsigned int vnum);

//Add below:
			void		RequestEventQuest(const string& quest_name, unsigned int pc);