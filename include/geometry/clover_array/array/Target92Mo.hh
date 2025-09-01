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

#include "CLHEP/Units/SystemOfUnits.h"

#pragma once

#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "SourceVolumeTubs.hh"

/**
 * \brief 92Mo target made by GSI
 */
class Target92Mo {
public:
  Target92Mo(G4LogicalVolume *world_log) : world_logical(world_log){};
  void Construct(const G4ThreeVector global_coordinates);
  
  static constexpr double TargetContainer_Length =2 * CLHEP::cm ;
  static constexpr double TargetContainer_Wall_Thickness = 0.1 * CLHEP::cm;
  static constexpr double TargetContainer_OuterRadius = 1.1 * CLHEP::cm;
  static constexpr double TargetContainer_InnerRadius = 0.975 * CLHEP::cm;
  static constexpr double Target92Mo_Radius = 0.9 * CLHEP::cm;
  static constexpr double Target92Mo_Thickness = 0.3 * CLHEP::cm;
  static constexpr double mass_92Mo1 = 2274.505 * CLHEP::mg;
  static constexpr double mass_92Mo2 = 2263.59 * CLHEP::mg;
  //static constexpr double atomicmass_92Mo = 91.906811 *u;
  static constexpr double TargetContainer_lid_Thickness = 0.2 * CLHEP::cm; 
  static constexpr double TargetContainer_lid_Radius = 1. * CLHEP::cm;

  std::shared_ptr<SourceVolume> get_source_volume() {
    return source_volume;
  }

protected:
  G4LogicalVolume *world_logical;
  std::shared_ptr<SourceVolume> source_volume;
};

