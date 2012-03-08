// interface.cpp

#define STRICT
#define D3D_OVERLOADS

#include <windows.h>
#include <stdio.h>
#include <d3d.h>

#include "struct.h"
#include "D3DEngine.h"
#include "math3d.h"
#include "event.h"
#include "misc.h"
#include "iman.h"
#include "control.h"
#include "button.h"
#include "color.h"
#include "check.h"
#include "key.h"
#include "group.h"
#include "image.h"
#include "label.h"
#include "edit.h"
#include "editvalue.h"
#include "scroll.h"
#include "slider.h"
#include "list.h"
#include "shortcut.h"
#include "compass.h"
#include "target.h"
#include "map.h"
#include "window.h"
#include "camera.h"
#include "interface.h"




// Constructeur de l'objet.

CInterface::CInterface(CInstanceManager* iMan)
{
	int		i;

	m_iMan = iMan;
	m_iMan->AddInstance(CLASS_INTERFACE, this);

	m_engine = (CD3DEngine*)m_iMan->SearchInstance(CLASS_ENGINE);
	m_camera = 0;

	for ( i=0 ; i<MAXCONTROL ; i++ )
	{
		m_table[i] = 0;
	}
}

// Destructeur de l'objet.

CInterface::~CInterface()
{
	Flush();
}


// Purge tous les contr�les.

void CInterface::Flush()
{
	int		i;

	for ( i=0 ; i<MAXCONTROL ; i++ )
	{
		if ( m_table[i] != 0 )
		{
			delete m_table[i];
			m_table[i] = 0;
		}
	}
}


// Cr�e un nouveau bouton.

CWindow* CInterface::CreateWindows(FPOINT pos, FPOINT dim, int icon, EventMsg eventMsg)
{
	CWindow*	pc;
	int			i;

	if ( eventMsg == EVENT_NULL )  eventMsg = GetUniqueEventMsg();

	if ( eventMsg == EVENT_WINDOW0 )  {i=0; goto create;}
	if ( eventMsg == EVENT_WINDOW1 )  {i=1; goto create;}
	if ( eventMsg == EVENT_WINDOW2 )  {i=2; goto create;}
	if ( eventMsg == EVENT_WINDOW3 )  {i=3; goto create;}
	if ( eventMsg == EVENT_WINDOW4 )  {i=4; goto create;}
	if ( eventMsg == EVENT_WINDOW5 )  {i=5; goto create;}
	if ( eventMsg == EVENT_WINDOW6 )  {i=6; goto create;}
	if ( eventMsg == EVENT_WINDOW7 )  {i=7; goto create;}
	if ( eventMsg == EVENT_WINDOW8 )  {i=8; goto create;}
	if ( eventMsg == EVENT_WINDOW9 )  {i=9; goto create;}

	if ( eventMsg == EVENT_TOOLTIP )  {i=MAXCONTROL-1; goto create;}

	for ( i=10 ; i<MAXCONTROL-1 ; i++ )
	{
		if ( m_table[i] == 0 )
		{
			create:
			m_table[i] = new CWindow(m_iMan);
			pc = (CWindow*)m_table[i];
			pc->Create(pos, dim, icon, eventMsg);
			return pc;
		}
	}
	return 0;
}

// Cr�e un nouveau bouton.

CButton* CInterface::CreateButton(FPOINT pos, FPOINT dim, int icon, EventMsg eventMsg)
{
	CButton*	pc;
	int			i;

	if ( eventMsg == EVENT_NULL )  eventMsg = GetUniqueEventMsg();

	for ( i=10 ; i<MAXCONTROL ; i++ )
	{
		if ( m_table[i] == 0 )
		{
			m_table[i] = new CButton(m_iMan);
			pc = (CButton*)m_table[i];
			pc->Create(pos, dim, icon, eventMsg);
			return pc;
		}
	}
	return 0;
}

// Cr�e un nouveau bouton.

CColor* CInterface::CreateColor(FPOINT pos, FPOINT dim, int icon, EventMsg eventMsg)
{
	CColor*		pc;
	int			i;

	if ( eventMsg == EVENT_NULL )  eventMsg = GetUniqueEventMsg();

	for ( i=10 ; i<MAXCONTROL ; i++ )
	{
		if ( m_table[i] == 0 )
		{
			m_table[i] = new CColor(m_iMan);
			pc = (CColor*)m_table[i];
			pc->Create(pos, dim, icon, eventMsg);
			return pc;
		}
	}
	return 0;
}

// Cr�e un nouveau bouton.

CCheck* CInterface::CreateCheck(FPOINT pos, FPOINT dim, int icon, EventMsg eventMsg)
{
	CCheck*		pc;
	int			i;

	if ( eventMsg == EVENT_NULL )  eventMsg = GetUniqueEventMsg();

	for ( i=10 ; i<MAXCONTROL ; i++ )
	{
		if ( m_table[i] == 0 )
		{
			m_table[i] = new CCheck(m_iMan);
			pc = (CCheck*)m_table[i];
			pc->Create(pos, dim, icon, eventMsg);
			return pc;
		}
	}
	return 0;
}

// Cr�e un nouveau bouton.

CKey* CInterface::CreateKey(FPOINT pos, FPOINT dim, int icon, EventMsg eventMsg)
{
	CKey*		pc;
	int			i;

	if ( eventMsg == EVENT_NULL )  eventMsg = GetUniqueEventMsg();

	for ( i=10 ; i<MAXCONTROL ; i++ )
	{
		if ( m_table[i] == 0 )
		{
			m_table[i] = new CKey(m_iMan);
			pc = (CKey*)m_table[i];
			pc->Create(pos, dim, icon, eventMsg);
			return pc;
		}
	}
	return 0;
}

// Cr�e un nouveau bouton.

CGroup* CInterface::CreateGroup(FPOINT pos, FPOINT dim, int icon, EventMsg eventMsg)
{
	CGroup*		pc;
	int			i;

	if ( eventMsg == EVENT_NULL )  eventMsg = GetUniqueEventMsg();

	for ( i=10 ; i<MAXCONTROL ; i++ )
	{
		if ( m_table[i] == 0 )
		{
			m_table[i] = new CGroup(m_iMan);
			pc = (CGroup*)m_table[i];
			pc->Create(pos, dim, icon, eventMsg);
			return pc;
		}
	}
	return 0;
}

// Cr�e un nouveau bouton.

CImage* CInterface::CreateImage(FPOINT pos, FPOINT dim, int icon, EventMsg eventMsg)
{
	CImage*		pc;
	int			i;

	if ( eventMsg == EVENT_NULL )  eventMsg = GetUniqueEventMsg();

	for ( i=10 ; i<MAXCONTROL ; i++ )
	{
		if ( m_table[i] == 0 )
		{
			m_table[i] = new CImage(m_iMan);
			pc = (CImage*)m_table[i];
			pc->Create(pos, dim, icon, eventMsg);
			return pc;
		}
	}
	return 0;
}

// Cr�e un nouveau label.

CLabel* CInterface::CreateLabel(FPOINT pos, FPOINT dim, int icon, EventMsg eventMsg,
							 char *name)
{
	CLabel*		pc;
	int			i;

	if ( eventMsg == EVENT_NULL )  eventMsg = GetUniqueEventMsg();

	for ( i=10 ; i<MAXCONTROL ; i++ )
	{
		if ( m_table[i] == 0 )
		{
			m_table[i] = new CLabel(m_iMan);
			pc = (CLabel*)m_table[i];
			pc->Create(pos, dim, icon, eventMsg);
			pc->SetName(name);
			return pc;
		}
	}
	return 0;
}

// Cr�e un nouveau pav� �ditable.

CEdit* CInterface::CreateEdit(FPOINT pos, FPOINT dim, int icon, EventMsg eventMsg)
{
	CEdit*		pc;
	int			i;

	if ( eventMsg == EVENT_NULL )  eventMsg = GetUniqueEventMsg();

	for ( i=10 ; i<MAXCONTROL ; i++ )
	{
		if ( m_table[i] == 0 )
		{
			m_table[i] = new CEdit(m_iMan);
			pc = (CEdit*)m_table[i];
			pc->Create(pos, dim, icon, eventMsg);
			return pc;
		}
	}
	return 0;
}

// Cr�e un nouveau pav� �ditable.

CEditValue* CInterface::CreateEditValue(FPOINT pos, FPOINT dim, int icon, EventMsg eventMsg)
{
	CEditValue*	pc;
	int			i;

	if ( eventMsg == EVENT_NULL )  eventMsg = GetUniqueEventMsg();

	for ( i=10 ; i<MAXCONTROL ; i++ )
	{
		if ( m_table[i] == 0 )
		{
			m_table[i] = new CEditValue(m_iMan);
			pc = (CEditValue*)m_table[i];
			pc->Create(pos, dim, icon, eventMsg);
			return pc;
		}
	}
	return 0;
}

// Cr�e un nouvel ascenseur.

CScroll* CInterface::CreateScroll(FPOINT pos, FPOINT dim, int icon, EventMsg eventMsg)
{
	CScroll*	pc;
	int			i;

	if ( eventMsg == EVENT_NULL )  eventMsg = GetUniqueEventMsg();

	for ( i=10 ; i<MAXCONTROL ; i++ )
	{
		if ( m_table[i] == 0 )
		{
			m_table[i] = new CScroll(m_iMan);
			pc = (CScroll*)m_table[i];
			pc->Create(pos, dim, icon, eventMsg);
			return pc;
		}
	}
	return 0;
}

// Cr�e un nouveau curseur.

CSlider* CInterface::CreateSlider(FPOINT pos, FPOINT dim, int icon, EventMsg eventMsg)
{
	CSlider*	pc;
	int			i;

	if ( eventMsg == EVENT_NULL )  eventMsg = GetUniqueEventMsg();

	for ( i=10 ; i<MAXCONTROL ; i++ )
	{
		if ( m_table[i] == 0 )
		{
			m_table[i] = new CSlider(m_iMan);
			pc = (CSlider*)m_table[i];
			pc->Create(pos, dim, icon, eventMsg);
			return pc;
		}
	}
	return 0;
}

// Cr�e une nouvelle liste.

CList* CInterface::CreateList(FPOINT pos, FPOINT dim, int icon, EventMsg eventMsg,
							float expand)
{
	CList*		pc;
	int			i;

	if ( eventMsg == EVENT_NULL )  eventMsg = GetUniqueEventMsg();

	for ( i=10 ; i<MAXCONTROL ; i++ )
	{
		if ( m_table[i] == 0 )
		{
			m_table[i] = new CList(m_iMan);
			pc = (CList*)m_table[i];
			pc->Create(pos, dim, icon, eventMsg, expand);
			return pc;
		}
	}
	return 0;
}

// Cr�e un nouveau raccourci.

CShortcut* CInterface::CreateShortcut(FPOINT pos, FPOINT dim, int icon, EventMsg eventMsg)
{
	CShortcut*	ps;
	int			i;

	if ( eventMsg == EVENT_NULL )  eventMsg = GetUniqueEventMsg();

	for ( i=10 ; i<MAXCONTROL ; i++ )
	{
		if ( m_table[i] == 0 )
		{
			m_table[i] = new CShortcut(m_iMan);
			ps = (CShortcut*)m_table[i];
			ps->Create(pos, dim, icon, eventMsg);
			return ps;
		}
	}
	return 0;
}

// Cr�e un nouvelle boussole.

CCompass* CInterface::CreateCompass(FPOINT pos, FPOINT dim, int icon, EventMsg eventMsg)
{
	CCompass*	pc;
	int			i;

	if ( eventMsg == EVENT_NULL )  eventMsg = GetUniqueEventMsg();

	for ( i=10 ; i<MAXCONTROL ; i++ )
	{
		if ( m_table[i] == 0 )
		{
			m_table[i] = new CCompass(m_iMan);
			pc = (CCompass*)m_table[i];
			pc->Create(pos, dim, icon, eventMsg);
			return pc;
		}
	}
	return 0;
}

// Cr�e un nouvelle cible.

CTarget* CInterface::CreateTarget(FPOINT pos, FPOINT dim, int icon, EventMsg eventMsg)
{
	CTarget*	pc;
	int			i;

	if ( eventMsg == EVENT_NULL )  eventMsg = GetUniqueEventMsg();

	for ( i=10 ; i<MAXCONTROL ; i++ )
	{
		if ( m_table[i] == 0 )
		{
			m_table[i] = new CTarget(m_iMan);
			pc = (CTarget*)m_table[i];
			pc->Create(pos, dim, icon, eventMsg);
			return pc;
		}
	}
	return 0;
}

// Cr�e une nouvelle carte.

CMap* CInterface::CreateMap(FPOINT pos, FPOINT dim, int icon, EventMsg eventMsg)
{
	CMap*		pm;
	int			i;

	if ( eventMsg == EVENT_NULL )  eventMsg = GetUniqueEventMsg();

	for ( i=10 ; i<MAXCONTROL ; i++ )
	{
		if ( m_table[i] == 0 )
		{
			m_table[i] = new CMap(m_iMan);
			pm = (CMap*)m_table[i];
			pm->Create(pos, dim, icon, eventMsg);
			return pm;
		}
	}
	return 0;
}

// Supprime un contr�le.

BOOL CInterface::DeleteControl(EventMsg eventMsg)
{
	int		i;

	for ( i=0 ; i<MAXCONTROL ; i++ )
	{
		if ( m_table[i] != 0 )
		{
			if ( eventMsg == m_table[i]->RetEventMsg() )
			{
				delete m_table[i];
				m_table[i] = 0;
				return TRUE;
			}
		}
	}
	return FALSE;
}

// Donne un contr�le.

CControl* CInterface::SearchControl(EventMsg eventMsg)
{
	int		i;

	for ( i=0 ; i<MAXCONTROL ; i++ )
	{
		if ( m_table[i] != 0 )
		{
			if ( eventMsg == m_table[i]->RetEventMsg() )
			{
				return m_table[i];
			}
		}
	}
	return 0;
}

// Gestion d'un �v�nement.

BOOL CInterface::EventProcess(const Event &event)
{
	int		i;

	if ( event.event == EVENT_MOUSEMOVE )
	{
		if ( m_camera == 0 )
		{
			m_camera = (CCamera*)m_iMan->SearchInstance(CLASS_CAMERA);
		}
		m_engine->SetMouseType(m_camera->RetMouseDef(event.pos));
	}

	for ( i=MAXCONTROL-1 ; i>=0 ; i-- )
	{
		if ( m_table[i] != 0 &&
			 m_table[i]->TestState(STATE_ENABLE) )
		{
			if ( !m_table[i]->EventProcess(event) )
			{
				return FALSE;
			}
		}
	}

	return TRUE;
}


// Donne le tooltip li� � la fen�tre.

BOOL CInterface::GetTooltip(FPOINT pos, char* name)
{
	int		i;

	for ( i=MAXCONTROL-1 ; i>=0 ; i-- )
	{
		if ( m_table[i] != 0 )
		{
			if ( m_table[i]->GetTooltip(pos, name) )
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}


// Dessine tous les boutons.

void CInterface::Draw()
{
	D3DMATERIAL7	material;
	int				i;

	ZeroMemory( &material, sizeof(D3DMATERIAL7) );
	material.diffuse.r = 1.0f;
	material.diffuse.g = 1.0f;
	material.diffuse.b = 1.0f;
	material.ambient.r = 0.5f;
	material.ambient.g = 0.5f;
	material.ambient.b = 0.5f;
	m_engine->SetMaterial(material);

	for ( i=0 ; i<MAXCONTROL ; i++ )
//?	for ( i=MAXCONTROL-1 ; i>=0 ; i-- )
	{
		if ( m_table[i] != 0 )
		{
			m_table[i]->Draw();
		}
	}
}


