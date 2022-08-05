"class CAniImageBox : public CWindow"

//Search:
			void SetRenderingMode(int iMode);
			
//Add below:
			void SetAniImgScale(float x, float y);
			
			
//Search:
			std::vector<CGraphicExpandedImageInstance*> m_ImageVector;
			
//Add below:
			float m_fScaleX;
			float m_fScaleY;