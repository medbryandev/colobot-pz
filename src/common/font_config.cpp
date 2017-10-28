/*
 * This file is part of the Colobot: Gold Edition source code
 * Copyright (C) 2001-2016, Daniel Roux, EPSITEC SA & TerranovaTeam
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
 
#include "common/font_config.h"

#include "common/logger.h"
#include "common/make_unique.h"

#include "common/resources/inputstream.h"
#include "common/resources/outputstream.h"

#include "common/system/system.h"

#include "graphics/engine/text.h"

#include <map>
#include <memory>
#include <utility>
#include <cstring>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/regex.hpp>

namespace bp = boost::property_tree;

CFontConfig::CFontConfig()
   : m_needsSave(false)
   , m_loaded(false)
{
}

CFontConfig::~CFontConfig()
{
}

bool CFontConfig::Init()
{
    try
    {
        std::unique_ptr<std::istream> stream;
        bool good;
        auto inputStream = MakeUnique<CInputStream>("/fonts/fonts.ini");
        good = inputStream->is_open();
        stream = std::move(inputStream);
        
        if (good)
        {
            bp::ini_parser::read_ini(*stream, m_propertyTree);
            GetLogger()->Debug("Fonts config file loaded correctly. \n");
            m_loaded = true;
        }
        else
        {
            return false;
        }
    }
    catch (std::exception & e)
    {
        GetLogger()->Error("Error on parsing config file: %s\n", e.what());
        return false;
    }
    return true;
}

std::string CFontConfig::GetFont(Gfx::FontType type)
{
    return std::string("/fonts/") + m_propertyTree.get<std::string>(GetFontType(type), GetDefaultFont(type));
}

std::string CFontConfig::GetDefaultFont(Gfx::FontType type) const
{
    return defaultFont.at(type);
}

std::string CFontConfig::GetFontType(Gfx::FontType type) const
{
    return fontType.at(type);
}