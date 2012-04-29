// * This file is part of the COLOBOT source code
// * Copyright (C) 2012, Polish Portal of Colobot (PPC)
// *
// * This program is free software: you can redistribute it and/or modify
// * it under the terms of the GNU General Public License as published by
// * the Free Software Foundation, either version 3 of the License, or
// * (at your option) any later version.
// *
// * This program is distributed in the hope that it will be useful,
// * but WITHOUT ANY WARRANTY; without even the implied warranty of
// * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// * GNU General Public License for more details.
// *
// * You should have received a copy of the GNU General Public License
// * along with this program. If not, see  http://www.gnu.org/licenses/.

// math/const.h

/* Math constants */

#pragma once

// Math module namespace
namespace Math
{
  //! Tolerance level -- minimum accepted float value
  const float TOLERANCE = 1e-6f;

  //! PI
  const float PI = 3.14159265358979323846f;
  //! 2 * PI
  const float PI_MUL_2 = 6.28318530717958623200f;
  //! PI / 2
  const float PI_DIV_2 = 1.57079632679489655800f;
  //! PI / 4
  const float PI_DIV_4 = 0.78539816339744827900f;
  //! 1 / PI
  const float INV_PI   = 0.31830988618379069122f;

  //! Degrees to radians multiplier
  const float DEG_TO_RAD =  0.01745329251994329547f;
  //! Radians to degrees multiplier
  const float RAD_TO_DEG = 57.29577951308232286465f;
};
