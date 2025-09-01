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

#include "G4Box.hh"
#include "G4Colour.hh"
#include "G4Isotope.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4PhysicalConstants.hh"
#include "G4SubtractionSolid.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"

#include "Target90Zr.hh"

void Target92Mo::Construct(const G4ThreeVector global_coordinates) {
   

   // Define colours for Visualization

  auto grey = G4Colour(.5,.5,.5);
  auto yellow = G4Colour(1.0,1.0,0.0);
  auto doublelightgray = G4Colour(0.6, 0.6, 0.6, 0.75);

  //Get NIST manager for material definitions

  G4NistManager *nist = G4NistManager::Instance();
  auto  *PE = nist->FindOrBuildMaterial("G4_POLYETHELENE");

  auto density_90Zr1 = mass_90Zr1/(pi * Target90Zr_Radius * Target90Zr_Radius * Target90Zr_Thickness);
  auto density_90Zr2 = mass_90Zr2/(pi * Target90Zr_Radius * Target90Zr_Radius * Target90Zr_Thickness);


  //Create materials for 90Zr1 and 90Zr2

  auto *material_90Zr1 = new G4Material(name="90Zr1",z=42,atomicmass_90Zr,density_90Zr1);
  auto *material_90Zr2 = new G4Material(name="90Zr2",z=42,atomicmass_90Zr,density_90Zr2);

  //Create Isotopes for 90Zr

  G4Isotope *Zr90 = new G4Isotope("90Zr", 40, 90);
  G4Isotope *Zr91 = new G4Isotope("91Zr", 40, 91);
  G4Isotope *Zr92 = new G4Isotope("92Zr", 40, 92);
  G4Isotope *Zr94 = new G4Isotope("94Zr", 40, 94);
  G4Isotope *Zr96 = new G4Isotope("96Zr", 40, 96);
  
  
  //Construct enriched 90Zr element

  G4Element* enriched_92Mo = new G4Element("enriched 90Zr", "90Zr-GSI",5);
  enriched_90Zr->AddIsotope(Zr90,0.9887);
  enriched_90Zr->AddIsotope(Zr91,0.0069);
  enriched_90Zr->AddIsotope(Zr92,0.0030);
  enriched_90Zr->AddIsotope(Zr94,0.0012);
  enriched_90Zr->AddIsotope(Zr96,0.0002);
 

  material_90Zr1->AddElement(enriched_90Zr,1);
  material_90Zr2->AddElement(enriched_90Zr,1);

  //Creating the solids

  auto *TargetContainer_Solid = 
      new G4Tubs("TargetContainer_Solid",TargetContainer_InnerRadius,
                 TargetContainer_OuterRadius,TargetContainer_Length*0.5,0.*deg,360. *deg);

  auto *Target90Zr1_Solid = 
      new G4Tubs("Target90Zr1_Solid",0.,Target90Zr_Radius, 
                 Target90Zr_Thickness*0.5,0.*deg,360. *deg);

  auto *Target90Zr2_Solid = 
      new G4Tubs("Target90Zr2_Solid",0.,Target90Zr_Radius, 
                 Target90Zr_Thickness*0.5,0.*deg,360. *deg);  

  auto *TargetContainer_lid_top_Solid = 
      new G4Tubs("TargetContainer_lid_top_Solid",TargetContainer_lid_radius,
                TargetContainer_lid_Thickness*0.5,0.*deg,360. *deg);        

  auto *TargetContainer_lid_bottom_Solid = 
      new G4Tubs("TargetContainer_lid_bottom_Solid",TargetContainer_lid_radius,
                TargetContainer_lid_Thickness*0.5,0.*deg,360. *deg); 


 //defining the logical volumes

  
   auto *TargetContainer_Logical = 
      new G4LogicalVolume(TargetContainer_Solid,PE,"TargetContainer_Logical");
   TargetContainer_Logical->SetVisAttributes(new G4VisAttributes(grey));

   auto *Target90Zr1_Logical = 
      new G4LogicalVolume(Target90Zr1_Solid,material_90Zr1,"Target90Zr1_Logical");
   Target92Mo1_Logical->SetVisAttributes(new G4VisAttributes(yellow));

   auto* Target90Zr2_Logical = 
      new G4LogicalVolume(Target90Zr2_Solid,material_90Zr2,"Target90Zr2_Logical");
   Target90Zr2_Logical->SetVisAttributes(new G4VisAttributes(doublelightgray));    
   
   auto *TargetContainer_lid_top_Logical = 
      new G4LogicalVolume(TargetContainer_lid_top_Solid,PE,"TargetContainer_lid_Logical");
   TargetContainer_lid_Logical->SetVisAttributes(new G4VisAttributes(grey)); 

    auto *TargetContainer_lid_bottom_Logical = 
      new G4LogicalVolume(TargetContainer_lid_bottom_Solid,PE,"TargetContainer_lid_Logical");
   TargetContainer_lid_Logical->SetVisAttributes(new G4VisAttributes(grey)); 



//defining the Physical volumes


  auto *target_90Zr_physical =
       new G4PVPlacement(0,global_coordiantes,Target90Zr1_Logical,
            "Target90Zr1_Physical",World_Logical,false,0);

  new G4PVPlacement(
      0,global_coordiantes+G4ThreeVector(0.,0.,0.5*Target90Zr_Thickness),Target90Zr1_Logical,
      "Target90Zr2_Physical",World_Logical,false,0);


  new G4PVPlacement(
      0,global_coordinates + G4ThreeVector(0.,0.,0.5*TargetContainer_Length),
      TargetConatiner_Logical,"TargetContainer_Physical",World_Logical,false,0);



  new G4PVPlacement(
      0,global_coordinates + G4ThreeVector(0.,0.,0.5*2*Target90Zr_Thickness),
      TargetConatiner_lid_top_Logical,"TargetContainer_lid_top_Physical",World_Logical,false,0);

  
  new G4PVPlacement(
      0,global_coordinates + G4ThreeVector(0.,0., - 0.5*2*Target90Zr_Thickness),
      TargetConatiner_lid_bottom_Logical,"TargetContainer_lid_bottom_Physical",World_Logical,false,0);

 source_volume = std::make_shared<SourceVolumeTubs>(Target_90Zr1_Solid, Target_90Zr1_Physical, 1.);

}
