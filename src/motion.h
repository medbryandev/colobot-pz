// motion.h

#ifndef _MOTION_H_
#define	_MOTION_H_


class CInstanceManager;
class CEngine;
class CLight;
class CParticule;
class CTerrain;
class CWater;
class CCamera;
class CBrain;
class CPhysics;
class CObject;
class CRobotMain;
class CSound;


class CMotion
{
public:
	CMotion(CInstanceManager* iMan, CObject* object);
	virtual ~CMotion();

	void	SetPhysics(CPhysics* physics);
	void	SetBrain(CBrain* brain);

	virtual void	DeleteObject(BOOL bAll=FALSE);
	virtual BOOL	Create(D3DVECTOR pos, float angle, ObjectType type, float power);
	virtual BOOL	EventProcess(const Event &event);
	virtual	Error	SetAction(int action, float time=0.2f);
	virtual int		RetAction();

	virtual BOOL	SetParam(int rank, float value);
	virtual float	RetParam(int rank);

	virtual BOOL	Write(char *line);
	virtual BOOL	Read(char *line);

	virtual void		SetLinVibration(D3DVECTOR dir);
	virtual D3DVECTOR	RetLinVibration();
	virtual void		SetCirVibration(D3DVECTOR dir);
	virtual D3DVECTOR	RetCirVibration();
	virtual void		SetInclinaison(D3DVECTOR dir);
	virtual D3DVECTOR	RetInclinaison();

protected:

protected:
	CInstanceManager* m_iMan;
	CD3DEngine*		m_engine;
	CLight*			m_light;
	CParticule*		m_particule;
	CTerrain*		m_terrain;
	CWater*			m_water;
	CCamera*		m_camera;
	CObject*		m_object;
	CBrain*			m_brain;
	CPhysics*		m_physics;
	CRobotMain*		m_main;
	CSound*			m_sound;

	int				m_actionType;
	float			m_actionTime;
	float			m_progress;

	D3DVECTOR		m_linVibration;		// vibration lin�aire
	D3DVECTOR		m_cirVibration;		// vibration circulaire
	D3DVECTOR		m_inclinaison;		// inclinaison
};


#endif //_MOTION_H_
