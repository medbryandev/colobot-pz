/*
 * This file is part of the Colobot: Gold Edition source code
 * Copyright (C) 2001-2014, Daniel Roux, EPSITEC SA & TerranovaTeam
 * http://epsiteс.ch; http://colobot.info; http://github.com/colobot
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

#include "graphics/engine/oldmodelmanager.h"

#include "app/app.h"

#include "common/logger.h"
#include "common/resources/inputstream.h"

#include "graphics/engine/engine.h"

#include "graphics/model/model_input.h"
#include "graphics/model/model_io_exception.h"

#include <cstdio>

namespace Gfx {

COldModelManager::COldModelManager(CEngine* engine)
{
    m_engine = engine;
}

COldModelManager::~COldModelManager()
{
}

bool COldModelManager::LoadModel(const std::string& fileName, bool mirrored)
{
    GetLogger()->Debug("Loading model '%s'\n", fileName.c_str());

    CModel model;
    try
    {
        CInputStream stream;
        stream.open("models/" + fileName);
        if (!stream.is_open())
            throw CModelIOException(std::string("Could not open file '") + fileName + "'");

        model = ModelInput::Read(stream, ModelFormat::Old);
    }
    catch (const CModelIOException& e)
    {
        GetLogger()->Error("Loading model '%s' failed: %s\n", fileName.c_str(), e.what());
        return false;
    }

    CModelMesh* mesh = model.GetMesh("main");
    assert(mesh != nullptr);

    ModelInfo modelInfo;
    modelInfo.baseObjRank = m_engine->CreateBaseObject();
    modelInfo.triangles = mesh->GetTriangles();

    if (mirrored)
        Mirror(modelInfo.triangles);

    FileInfo fileInfo(fileName, mirrored);
    m_models[fileInfo] = modelInfo;

    m_engine->AddBaseObjTriangles(modelInfo.baseObjRank, modelInfo.triangles);

    return true;
}

bool COldModelManager::AddModelReference(const std::string& fileName, bool mirrored, int objRank)
{
    auto it = m_models.find(FileInfo(fileName, mirrored));
    if (it == m_models.end())
    {
        if (!LoadModel(fileName, mirrored))
            return false;

        it = m_models.find(FileInfo(fileName, mirrored));
    }

    m_engine->SetObjectBaseRank(objRank, (*it).second.baseObjRank);

    return true;
}

bool COldModelManager::AddModelCopy(const std::string& fileName, bool mirrored, int objRank)
{
    auto it = m_models.find(FileInfo(fileName, mirrored));
    if (it == m_models.end())
    {
        if (!LoadModel(fileName, mirrored))
            return false;

        it = m_models.find(FileInfo(fileName, mirrored));
    }

    int copyBaseObjRank = m_engine->CreateBaseObject();
    m_engine->CopyBaseObject((*it).second.baseObjRank, copyBaseObjRank);
    m_engine->SetObjectBaseRank(objRank, copyBaseObjRank);

    m_copiesBaseRanks.push_back(copyBaseObjRank);

    return true;
}

bool COldModelManager::IsModelLoaded(const std::string& fileName, bool mirrored)
{
    return m_models.count(FileInfo(fileName, mirrored)) > 0;
}

int COldModelManager::GetModelBaseObjRank(const std::string& fileName, bool mirrored)
{
    auto it = m_models.find(FileInfo(fileName, mirrored));
    if (it == m_models.end())
        return -1;

    return (*it).second.baseObjRank;
}

void COldModelManager::DeleteAllModelCopies()
{
    for (int baseObjRank : m_copiesBaseRanks)
    {
        m_engine->DeleteBaseObject(baseObjRank);
    }

    m_copiesBaseRanks.clear();
}

void COldModelManager::UnloadModel(const std::string& fileName, bool mirrored)
{
    auto it = m_models.find(FileInfo(fileName, mirrored));
    if (it == m_models.end())
        return;

    m_engine->DeleteBaseObject((*it).second.baseObjRank);

    m_models.erase(it);
}

void COldModelManager::UnloadAllModels()
{
    for (auto& mf : m_models)
        m_engine->DeleteBaseObject(mf.second.baseObjRank);

    m_models.clear();
}

void COldModelManager::Mirror(std::vector<ModelTriangle>& triangles)
{
    for (int i = 0; i < static_cast<int>( triangles.size() ); i++)
    {
        VertexTex2  t = triangles[i].p1;
        triangles[i].p1 = triangles[i].p2;
        triangles[i].p2 = t;

        triangles[i].p1.coord.z = -triangles[i].p1.coord.z;
        triangles[i].p2.coord.z = -triangles[i].p2.coord.z;
        triangles[i].p3.coord.z = -triangles[i].p3.coord.z;

        triangles[i].p1.normal.z = -triangles[i].p1.normal.z;
        triangles[i].p2.normal.z = -triangles[i].p2.normal.z;
        triangles[i].p3.normal.z = -triangles[i].p3.normal.z;
    }
}

}

