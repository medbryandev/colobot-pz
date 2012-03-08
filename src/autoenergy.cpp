// autoenergy.cpp

#define STRICT
#define D3D_OVERLOADS

#include <windows.h>
#include <stdio.h>
#include <d3d.h>

#include "struct.h"
#include "D3DEngine.h"
#include "D3DMath.h"
#include "event.h"
#include "misc.h"
#include "iman.h"
#include "math3d.h"
#include "particule.h"
#include "light.h"
#include "terrain.h"
#include "camera.h"
#include "object.h"
#include "interface.h"
#include "button.h"
#include "gauge.h"
#include "window.h"
#include "displaytext.h"
#include "sound.h"
#include "cmdtoken.h"
#include "auto.h"
#include "autoenergy.h"



#define ENERGY_POWER	 0.4f		// �nergie n�cessaire pour une pile
#define ENERGY_DELAY	12.0f		// dur�e de la transformation




// Constructeur de l'objet.

CAutoEnergy::CAutoEnergy(CInstanceManager* iMan, CObject* object)
						 : CAuto(iMan, object)
{
	CAuto::CAuto(iMan, object);

	m_partiSphere = -1;
	Init();
}

// Destructeur de l'objet.

CAutoEnergy::~CAutoEnergy()
{
	CAuto::~CAuto();
}


// D�truit l'objet.

void CAutoEnergy::DeleteObject(BOOL bAll)
{
	CObject*	fret;

	if ( m_partiSphere != -1 )
	{
		m_particule->DeleteParticule(m_partiSphere);
		m_partiSphere = -1;
	}

	if ( !bAll )
	{
		fret = SearchMetal();
		if ( fret != 0 )
		{
			fret->DeleteObject();  // d�truit le m�tal
			delete fret;
		}

		fret = SearchPower();
		if ( fret != 0 )
		{
			fret->DeleteObject();  // d�truit la pile
			delete fret;
		}
	}

	CAuto::DeleteObject(bAll);
}


// Initialise l'objet.

void CAutoEnergy::Init()
{
	m_time = 0.0f;
	m_timeVirus = 0.0f;
	m_lastUpdateTime = 0.0f;
	m_lastParticule = 0.0f;

	m_phase    = AENP_WAIT;  // attend ...
	m_progress = 0.0f;
	m_speed    = 1.0f/2.0f;

	CAuto::Init();
}


// Gestion d'un �v�nement.

BOOL CAutoEnergy::EventProcess(const Event &event)
{
	CObject*	fret;
	D3DVECTOR	pos, ppos, speed;
	FPOINT		dim, c, p;
	TerrainRes	res;
	float		big;
	BOOL		bGO;

	CAuto::EventProcess(event);

	if ( m_engine->RetPause() )  return TRUE;
	if ( event.event != EVENT_FRAME )  return TRUE;

	m_progress += event.rTime*m_speed;
	m_timeVirus -= event.rTime;

	if ( m_object->RetVirusMode() )  // contamin� par un virus ?
	{
		if ( m_timeVirus <= 0.0f )
		{
			m_timeVirus = 0.1f+Rand()*0.3f;

			if ( m_lastParticule+m_engine->ParticuleAdapt(0.05f) <= m_time )
			{
				m_lastParticule = m_time;
				pos = m_object->RetPosition(0);
				pos.y += 10.0f;
				speed.x = (Rand()-0.5f)*10.0f;
				speed.z = (Rand()-0.5f)*10.0f;
				speed.y = -7.0f;
				dim.x = Rand()*0.5f+0.5f;
				dim.y = dim.x;
				m_particule->CreateParticule(pos, speed, dim, PARTIFIREZ, 1.0f, 0.0f, 0.0f);
			}
		}
		return TRUE;
	}
	
	UpdateInterface(event.rTime);
	EventProgress(event.rTime);

	big = m_object->RetEnergy();

	res = m_terrain->RetResource(m_object->RetPosition(0));
	if ( res == TR_POWER )
	{
		big += event.rTime*0.01f;  // recharge la grosse pile
	}

	if ( m_phase == AENP_WAIT )
	{
		if ( m_progress >= 1.0f )
		{
			bGO = FALSE;
			fret = SearchMetal();  // m�tal � transformer ?
			if ( fret != 0 )
			{
				if ( fret->RetType() == OBJECT_METAL )
				{
					if ( big > ENERGY_POWER )  bGO = TRUE;
				}
				else
				{
					if ( !SearchVehicle() )  bGO = TRUE;
				}
			}

			if ( bGO )
			{
				if ( fret->RetType() == OBJECT_METAL )
				{
					fret->SetLock(TRUE);  // m�tal plus utilisable
					CreatePower();  // cr�e la pile
				}

				SetBusy(TRUE);
				InitProgressTotal(ENERGY_DELAY);
				CAuto::UpdateInterface();

				pos = m_object->RetPosition(0);
				pos.y += 4.0f;
				speed = D3DVECTOR(0.0f, 0.0f, 0.0f);
				dim.x = 3.0f;
				dim.y = dim.x;
				m_partiSphere = m_particule->CreateParticule(pos, speed, dim, PARTISPHERE1, ENERGY_DELAY, 0.0f, 0.0f);

				m_phase    = AENP_CREATE;
				m_progress = 0.0f;
				m_speed    = 1.0f/ENERGY_DELAY;
			}
			else
			{
				if ( rand()%3 == 0 && big > 0.01f )
				{
					m_phase    = AENP_BLITZ;
					m_progress = 0.0f;
					m_speed    = 1.0f/Rand()*1.0f+1.0f;
				}
				else
				{
					m_phase    = AENP_WAIT;  // attend encore ...
					m_progress = 0.0f;
					m_speed    = 1.0f/2.0f;
				}
			}
		}
	}

	if ( m_phase == AENP_BLITZ )
	{
		if ( m_progress < 1.0f && big > 0.01f )
		{
			if ( m_lastParticule+m_engine->ParticuleAdapt(0.05f) <= m_time )
			{
				m_lastParticule = m_time;
				pos = m_object->RetPosition(0);
				pos.y += 10.0f;
				speed.x = (Rand()-0.5f)*1.0f;
				speed.z = (Rand()-0.5f)*1.0f;
				speed.y = -7.0f;
				dim.x = Rand()*0.5f+0.5f;
				dim.y = dim.x;
				m_particule->CreateParticule(pos, speed, dim, PARTIFIREZ, 1.0f, 0.0f, 0.0f);
			}
		}
		else
		{
			m_phase    = AENP_WAIT;  // attend encore ...
			m_progress = 0.0f;
			m_speed    = 1.0f/2.0f;
		}
	}

	if ( m_phase == AENP_CREATE )
	{
		if ( m_progress < 1.0f )
		{
			fret = SearchMetal();
			if ( fret != 0 )
			{
				if ( fret->RetType() == OBJECT_METAL )
				{
					big -= event.rTime/ENERGY_DELAY*ENERGY_POWER;
				}
				else
				{
					big += event.rTime/ENERGY_DELAY*0.25f;
				}
				fret->SetZoom(0, 1.0f-m_progress);
			}

			fret = SearchPower();
			if ( fret != 0 )
			{
				fret->SetZoom(0, m_progress);
			}

			if ( m_lastParticule+m_engine->ParticuleAdapt(0.10f) <= m_time )
			{
				m_lastParticule = m_time;

				pos = m_object->RetPosition(0);
				c.x = pos.x;
				c.y = pos.z;
				p.x = c.x;
				p.y = c.y+2.0f;
				p = RotatePoint(c, Rand()*PI*2.0f, p);
				pos.x = p.x;
				pos.z = p.y;
				pos.y += 2.5f+Rand()*3.0f;
				speed = D3DVECTOR(0.0f, 0.0f, 0.0f);
				dim.x = Rand()*2.0f+1.0f;
				dim.y = dim.x;
				m_particule->CreateParticule(pos, speed, dim, PARTIGLINT, 1.0f, 0.0f, 0.0f);

				pos = m_object->RetPosition(0);
				pos.y += 3.0f;
				speed.x = (Rand()-0.5f)*30.0f;
				speed.z = (Rand()-0.5f)*30.0f;
				speed.y = Rand()*20.0f+10.0f;
				dim.x = Rand()*0.4f+0.4f;
				dim.y = dim.x;
				m_particule->CreateTrack(pos, speed, dim, PARTITRACK2, 2.0f, 50.0f, 1.2f, 1.2f);

				pos = m_object->RetPosition(0);
				pos.y += 10.0f;
				speed.x = (Rand()-0.5f)*1.5f;
				speed.z = (Rand()-0.5f)*1.5f;
				speed.y = -6.0f;
				dim.x = Rand()*1.0f+1.0f;
				dim.y = dim.x;
				m_particule->CreateParticule(pos, speed, dim, PARTIFIREZ, 1.0f, 0.0f, 0.0f);

				m_sound->Play(SOUND_ENERGY, m_object->RetPosition(0),
							  1.0f, 1.0f+Rand()*1.5f);
			}
		}
		else
		{
			fret = SearchMetal();
			if ( fret != 0 )
			{
				m_object->SetPower(0);
				fret->DeleteObject();  // d�truit le m�tal
				delete fret;
			}

			fret = SearchPower();
			if ( fret != 0 )
			{
				fret->SetZoom(0, 1.0f);
				fret->SetLock(FALSE);  // pile utilisable
				fret->SetTruck(m_object);
				fret->SetPosition(0, D3DVECTOR(0.0f, 3.0f, 0.0f));
				m_object->SetPower(fret);

				m_displayText->DisplayError(INFO_ENERGY, m_object);
			}

			SetBusy(FALSE);
			CAuto::UpdateInterface();

			m_phase    = AENP_SMOKE;
			m_progress = 0.0f;
			m_speed    = 1.0f/5.0f;
		}
	}
	
	if ( m_phase == AENP_SMOKE )
	{
		if ( m_progress < 1.0f )
		{
			if ( m_lastParticule+m_engine->ParticuleAdapt(0.05f) <= m_time )
			{
				m_lastParticule = m_time;

				pos = m_object->RetPosition(0);
				pos.y += 17.0f;
				pos.x += (Rand()-0.5f)*3.0f;
				pos.z += (Rand()-0.5f)*3.0f;
				speed.x = 0.0f;
				speed.z = 0.0f;
				speed.y = 6.0f+Rand()*6.0f;
				dim.x = Rand()*1.5f+1.0f;
				dim.y = dim.x;
				m_particule->CreateParticule(pos, speed, dim, PARTISMOKE3, 4.0f);
			}
		}
		else
		{
			m_phase    = AENP_WAIT;
			m_progress = 0.0f;
			m_speed    = 1.0f/2.0f;
		}
	}

	if ( big < 0.0f )  big = 0.0f;
	if ( big > 1.0f )  big = 1.0f;
	m_object->SetEnergy(big);  // m�j la grosse pile

	return TRUE;
}


// Cherche l'objet m�tal.

CObject* CAutoEnergy::SearchMetal()
{
	CObject*	pObj;
	ObjectType	type;

	pObj = m_object->RetPower();
	if ( pObj == 0 )  return 0;

	type = pObj->RetType();
	if ( type == OBJECT_METAL  ||
		 type == OBJECT_SCRAP1 ||
		 type == OBJECT_SCRAP2 ||
		 type == OBJECT_SCRAP3 )  return pObj;

	return 0;
}

// Cherche si un v�hicule est trop proche.

BOOL CAutoEnergy::SearchVehicle()
{
	CObject*	pObj;
	D3DVECTOR	cPos, oPos;
	ObjectType	type;
	float		oRadius, dist;
	int			i;

	cPos = m_object->RetPosition(0);

	for ( i=0 ; i<1000000 ; i++ )
	{
		pObj = (CObject*)m_iMan->SearchInstance(CLASS_OBJECT, i);
		if ( pObj == 0 )  break;

		type = pObj->RetType();
		if ( type != OBJECT_HUMAN    &&
			 type != OBJECT_MOBILEfa &&
			 type != OBJECT_MOBILEta &&
			 type != OBJECT_MOBILEwa &&
			 type != OBJECT_MOBILEia &&
			 type != OBJECT_MOBILEfc &&
			 type != OBJECT_MOBILEtc &&
			 type != OBJECT_MOBILEwc &&
			 type != OBJECT_MOBILEic &&
			 type != OBJECT_MOBILEfi &&
			 type != OBJECT_MOBILEti &&
			 type != OBJECT_MOBILEwi &&
			 type != OBJECT_MOBILEii &&
			 type != OBJECT_MOBILEfs &&
			 type != OBJECT_MOBILEts &&
			 type != OBJECT_MOBILEws &&
			 type != OBJECT_MOBILEis &&
			 type != OBJECT_MOBILErt &&
			 type != OBJECT_MOBILErc &&
			 type != OBJECT_MOBILErr &&
			 type != OBJECT_MOBILErs &&
			 type != OBJECT_MOBILEsa &&
			 type != OBJECT_MOBILEtg &&
			 type != OBJECT_MOBILEft &&
			 type != OBJECT_MOBILEtt &&
			 type != OBJECT_MOBILEwt &&
			 type != OBJECT_MOBILEit &&
			 type != OBJECT_MOBILEdr &&
			 type != OBJECT_MOTHER   &&
			 type != OBJECT_ANT      &&
			 type != OBJECT_SPIDER   &&
			 type != OBJECT_BEE      &&
			 type != OBJECT_WORM     )  continue;

		if ( !pObj->GetCrashSphere(0, oPos, oRadius) )  continue;
		dist = Length(oPos, cPos)-oRadius;

		if ( dist < 10.0f )  return TRUE;
	}

	return FALSE;
}

// Cr�e un objet pile.

void CAutoEnergy::CreatePower()
{
	CObject*		power;
	D3DVECTOR		pos;
	float			angle;

	pos = m_object->RetPosition(0);
	angle = m_object->RetAngleY(0);

	power = new CObject(m_iMan);
	if ( !power->CreateResource(pos, angle, OBJECT_POWER) )
	{
		delete power;
		m_displayText->DisplayError(ERR_TOOMANY, m_object);
		return;
	}
	power->SetLock(TRUE);  // pile pas encore utilisable

	pos = power->RetPosition(0);
	pos.y += 3.0f;
	power->SetPosition(0, pos);
}

// Cherche la pile en cours de fabrication.

CObject* CAutoEnergy::SearchPower()
{
	CObject*	pObj;
	D3DVECTOR	cPos, oPos;
	ObjectType	type;
	int			i;

	cPos = m_object->RetPosition(0);

	for ( i=0 ; i<1000000 ; i++ )
	{
		pObj = (CObject*)m_iMan->SearchInstance(CLASS_OBJECT, i);
		if ( pObj == 0 )  break;

		if ( !pObj->RetLock() )  continue;

		type = pObj->RetType();
		if ( type != OBJECT_POWER )  continue;

		oPos = pObj->RetPosition(0);
		if ( oPos.x == cPos.x &&
			 oPos.z == cPos.z )
		{
			return pObj;
		}
	}

	return 0;
}


// Retourne une erreur li�e � l'�tat de l'automate.

Error CAutoEnergy::RetError()
{
	CObject*	pObj;
	ObjectType	type;
	TerrainRes	res;

	if ( m_object->RetVirusMode() )
	{
		return ERR_BAT_VIRUS;
	}

	if ( m_phase != AENP_WAIT  &&
		 m_phase != AENP_BLITZ )  return ERR_OK;

	res = m_terrain->RetResource(m_object->RetPosition(0));
	if ( res != TR_POWER )  return ERR_ENERGY_NULL;

	if ( m_object->RetEnergy() < ENERGY_POWER )  return ERR_ENERGY_LOW;

	pObj = m_object->RetPower();
	if ( pObj == 0 )  return ERR_ENERGY_EMPTY;
	type = pObj->RetType();
	if ( type == OBJECT_POWER )  return ERR_OK;
	if ( type != OBJECT_METAL  &&
		 type != OBJECT_SCRAP1 &&
		 type != OBJECT_SCRAP2 &&
		 type != OBJECT_SCRAP3 )  return ERR_ENERGY_BAD;

	return ERR_OK;
}


// Cr�e toute l'interface lorsque l'objet est s�lectionn�.

BOOL CAutoEnergy::CreateInterface(BOOL bSelect)
{
	CWindow*	pw;
	FPOINT		pos, ddim;
	float		ox, oy, sx, sy;

	CAuto::CreateInterface(bSelect);

	if ( !bSelect )  return TRUE;

	pw = (CWindow*)m_interface->SearchControl(EVENT_WINDOW0);
	if ( pw == 0 )  return FALSE;

	ox = 3.0f/640.0f;
	oy = 3.0f/480.0f;
	sx = 33.0f/640.0f;
	sy = 33.0f/480.0f;

	pos.x = ox+sx*14.5f;
	pos.y = oy+sy*0;
	ddim.x = 14.0f/640.0f;
	ddim.y = 66.0f/480.0f;
	pw->CreateGauge(pos, ddim, 0, EVENT_OBJECT_GENERGY);

	pos.x = ox+sx*0.0f;
	pos.y = oy+sy*0;
	ddim.x = 66.0f/640.0f;
	ddim.y = 66.0f/480.0f;
	pw->CreateGroup(pos, ddim, 108, EVENT_OBJECT_TYPE);

	return TRUE;
}

// Met � jour l'�tat de tous les boutons de l'interface,
// suite au temps qui s'�coule ...

void CAutoEnergy::UpdateInterface(float rTime)
{
	CWindow*	pw;
	CGauge*		pg;

	CAuto::UpdateInterface(rTime);

	if ( m_time < m_lastUpdateTime+0.1f )  return;
	m_lastUpdateTime = m_time;

	if ( !m_object->RetSelect() )  return;

	pw = (CWindow*)m_interface->SearchControl(EVENT_WINDOW0);
	if ( pw == 0 )  return;

	pg = (CGauge*)pw->SearchControl(EVENT_OBJECT_GENERGY);
	if ( pg != 0 )
	{
		pg->SetLevel(m_object->RetEnergy());
	}
}


// Sauve tous les param�tres de l'automate.

BOOL CAutoEnergy::Write(char *line)
{
	char	name[100];

	if ( m_phase == AENP_STOP ||
		 m_phase == AENP_WAIT )  return FALSE;

	sprintf(name, " aExist=%d", 1);
	strcat(line, name);

	CAuto::Write(line);

	sprintf(name, " aPhase=%d", m_phase);
	strcat(line, name);

	sprintf(name, " aProgress=%.2f", m_progress);
	strcat(line, name);

	sprintf(name, " aSpeed=%.2f", m_speed);
	strcat(line, name);

	return TRUE;
}

// Restitue tous les param�tres de l'automate.

BOOL CAutoEnergy::Read(char *line)
{
	if ( OpInt(line, "aExist", 0) == 0 )  return FALSE;

	CAuto::Read(line);

	m_phase = (AutoEnergyPhase)OpInt(line, "aPhase", AENP_WAIT);
	m_progress = OpFloat(line, "aProgress", 0.0f);
	m_speed = OpFloat(line, "aSpeed", 1.0f);

	m_lastUpdateTime = 0.0f;
	m_lastParticule = 0.0f;

	return TRUE;
}

