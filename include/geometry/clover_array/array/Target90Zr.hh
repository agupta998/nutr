/*
    This file is part of nutr.

    nutr is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    nutr is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with nutr.  If not, see <https://www.gnu.org/licenses/>.

    Copyright (C) 2021 Oliver Papst
*/

#pragma once

#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "SourceVolumeTubs.hh"

/**
 * \brief 90Zrtarget made by GSI
 */
class Target90Zr {
public:
  Target92Mo(G4LogicalVolume *world_log) : world_logical(world_log){};
  void Construct(const G4ThreeVector global_coordinates);

  static constexpr double TargetContainer_Length =20 *mm ;
  static constexpr double TargetContainer_Wall_Thickness = 1. *mm;
  static constexpr double TargetContainer_OuterRadius = 11. *mm;
  static constexpr double TargetContainer_InnerRadius = 9.75 *mm;
  static constexpr double Target90Zr_Radius = 9. *mm;
  static constexpr double Target90Zr_Thickness = 3. *mm;
  static constexpr double mass_90Zr1 = 2029.58 *mg;
  static constexpr double mass_90Zr2 = 2029.58 *mg;
  static constexpr double atomicmass_90Zr = 89.904*u;
  static constexpr double TargetConatiner_lid_Thickness = 2 *mm; 
  static constexpr double TargetConatiner_lid_Radius = 10. *mm;

  std::shared_ptr<SourceVolume> get_source_volume() {
    return source_volume;
  }

protected:
  G4LogicalVolume *world_logical;
  std::shared_ptr<SourceVolume> source_volume;
};