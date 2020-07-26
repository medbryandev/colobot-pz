/*
 * This file is part of the Colobot: Gold Edition source code
 * Copyright (C) 2001-2020, Daniel Roux, EPSITEC SA & TerranovaTeam
 * http://epsitec.ch; http://colobot.info; http://github.com/colobot
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see http://gnu.org/licenses
 */

#include "graphics/engine/particle.h"

#include "graphics/pyro/pyro.h"
#include "graphics/pyro/pyro_manager.h"

#include "object/object.h"

#include "sound/sound.h"

using namespace Gfx;

void CWaypointHitPyro::AfterCreate()
{
    m_sound->Play(SOUND_WAYPOINT, m_pos);
    LightOperAdd(0.00f, 1.0f,  4.0f,  4.0f,  2.0f);  // yellow
    LightOperAdd(1.00f, 0.0f,  4.0f,  4.0f,  2.0f);  // yellow
    CreateLight(m_pos, 40.0f);
    m_speed = 1.0f/8.0f;
    m_object->SetLock(true);  // object more functional
}

void CWaypointHitPyro::AfterEnd()
{
    DeleteObject(true, true);
}

void CWaypointHitPyro::UpdateEffect()
{
    float factor;
    if (m_progress < 0.25f)
        factor = 0.0f;
    else
        factor = powf((m_progress-0.25f)/0.75f, 2.0f)*30.0f;

    if ( m_progress < 0.85f &&
         m_lastParticle+m_engine->ParticleAdapt(0.10f) <= m_time )
    {
        m_lastParticle = m_time;

        Math::Vector pos = m_pos;
        pos.y += factor;
        pos.x += (Math::Rand()-0.5f)*3.0f;
        pos.z += (Math::Rand()-0.5f)*3.0f;
        Math::Vector speed;
        speed.x = 0.0f;
        speed.z = 0.0f;
        speed.y = 5.0f+Math::Rand()*5.0f;
        Math::Point dim;
        dim.x = Math::Rand()*1.5f+1.5f;
        dim.y = dim.x;
        m_particle->CreateParticle(pos, speed, dim, PARTIGLINT, 2.0f);
    }

    if(m_object != nullptr)
    {
        Math::Vector angle = m_object->GetRotation();
        angle.y = m_progress*20.0f;
        angle.x = sinf(m_progress*49.0f)*0.3f;
        angle.z = sinf(m_progress*47.0f)*0.2f;
        m_object->SetRotation(angle);

        Math::Vector pos = m_pos;
        pos.y += factor;
        m_object->SetPosition(pos);

        if ( m_progress > 0.85f )
        {
            m_object->SetScale(1.0f-(m_progress-0.85f)/0.15f);
        }
    }
}
