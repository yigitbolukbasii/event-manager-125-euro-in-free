//Search:
	CAniImageBox::CAniImageBox(PyObject * ppyObject)
		:	CWindow(ppyObject),
			m_bycurDelay(0),
			m_byDelay(4),
			m_bycurIndex(0)
			
//Replace with:
	CAniImageBox::CAniImageBox(PyObject * ppyObject)
		:	CWindow(ppyObject),
			m_bycurDelay(0),
			m_byDelay(4),
			m_bycurIndex(0),
			m_fScaleX(1),
			m_fScaleY(1)
			
			
//Search:
	void CAniImageBox::AppendImage(const char * c_szFileName)
	{
		CResource * pResource = CResourceManager::Instance().GetResourcePointer(c_szFileName);
		if (!pResource->IsType(CGraphicImage::Type()))
			return;

		CGraphicExpandedImageInstance * pImageInstance = CGraphicExpandedImageInstance::New();

		pImageInstance->SetImagePointer(static_cast<CGraphicImage*>(pResource));
		if (pImageInstance->IsEmpty())
		{
			CGraphicExpandedImageInstance::Delete(pImageInstance);
			return;
		}
		
//Add below:
		pImageInstance->SetScale(m_fScaleX, m_fScaleY);
		
		
//Search:
	void CAniImageBox::SetRenderingMode(int iMode)
	{
		FSetRenderingMode setRenderingMode;
		setRenderingMode.iMode = iMode;
		for_each(m_ImageVector.begin(), m_ImageVector.end(), setRenderingMode);
	}
	
//Add below:
	struct FSetAniImgScale
	{
		float fScaleX, fScaleY;
		void operator () (CGraphicExpandedImageInstance* pInstance)
		{
			pInstance->SetScale(fScaleX, fScaleY);
		}
	};
	void CAniImageBox::SetAniImgScale(float fScaleX, float fScaleY)
	{
		m_fScaleX = fScaleX;
		m_fScaleY = fScaleY;

		FSetAniImgScale setAniImgScale;
		setAniImgScale.fScaleX = m_fScaleX;
		setAniImgScale.fScaleY = m_fScaleY;

		for_each(m_ImageVector.begin(), m_ImageVector.end(), setAniImgScale);
	}