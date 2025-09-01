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

#include "Target92Mo.hh"

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




void Target92Mo::Construct(const G4ThreeVector global_coordinates) {
   

   // Define colours for Visualization

  auto grey = G4Colour(.5,.5,.5);
  auto yellow = G4Colour(1.0,1.0,0.0);
  auto doublelightgray = G4Colour(0.6, 0.6, 0.6, 0.75);

  //Get NIST manager for material definitions

  G4NistManager *nist = G4NistManager::Instance();
  auto  *material_pe = nist->FindOrBuildMaterial("G4_POLYETHYLENE");

 

  //Create Isotopes for 92Mo

  G4Isotope *Mo92 = new G4Isotope("92Mo", 42, 92);
  G4Isotope *Mo94 = new G4Isotope("94Mo", 42, 94);
  G4Isotope *Mo95 = new G4Isotope("95Mo", 42, 95);
  G4Isotope *Mo96 = new G4Isotope("96Mo", 42, 96);
  G4Isotope *Mo97 = new G4Isotope("97Mo", 42, 97);
  G4Isotope *Mo98 = new G4Isotope("98Mo", 42, 98);
  G4Isotope *Mo100 = new G4Isotope("100Mo", 42, 100);
  
  //Construct enriched 92Mo element

  G4Element* enriched_92Mo = new G4Element("enriched 92Mo", "92Mo-GSI",7);
  enriched_92Mo->AddIsotope(Mo92,0.9520);
  enriched_92Mo->AddIsotope(Mo94,0.0090);
  enriched_92Mo->AddIsotope(Mo95,0.0099);
  enriched_92Mo->AddIsotope(Mo96,0.0090);
  enriched_92Mo->AddIsotope(Mo97,0.0047);
  enriched_92Mo->AddIsotope(Mo98,0.0120);
  enriched_92Mo->AddIsotope(Mo100,0.0039);

  auto density_92Mo1 = mass_92Mo1/(pi * Target92Mo_Radius * Target92Mo_Radius * Target92Mo_Thickness); //Why i have taken account of the thickness
  auto density_92Mo2 = mass_92Mo2/(pi * Target92Mo_Radius * Target92Mo_Radius * Target92Mo_Thickness);


  //Create materials for 92Mo1 and 92Mo2

  auto *material_92Mo1 = new G4Material("mat_92Mo1", density_92Mo1,1);
  auto *material_92Mo2 = new G4Material("mat_92Mo2", density_92Mo2,1);

  material_92Mo1->AddElement(enriched_92Mo,1);
  material_92Mo2->AddElement(enriched_92Mo,1);

  /*auto density_96mo = mass_96mo / (radius_96mo * radius_96mo * pi);
  auto *material_96mo = new G4Material("mat_96Mo", density_96mo, 1);
  material_96mo->AddElement(enriched_96Mo, 1);*/


  //Creating the solids

  auto *TargetContainer_Solid = new G4Tubs("TargetContainer_Solid",TargetContainer_InnerRadius,
                 TargetContainer_OuterRadius,TargetContainer_Length*0.5, 0., 2. * pi);

  auto *Target92Mo1_Solid = new G4Tubs("Target92Mo1_Solid",0.,Target92Mo_Radius, 
                 Target92Mo_Thickness*0.5, 0., 2. * pi);

  auto *Target92Mo2_Solid = new G4Tubs("Target92Mo2_Solid",0.,Target92Mo_Radius, 
                 Target92Mo_Thickness*0.5, 0., 2. * pi);  

  auto *TargetContainer_lid_top_Solid = new G4Tubs("TargetContainer_lid_top_Solid",0.,TargetContainer_lid_Radius,
                TargetContainer_lid_Thickness*0.5, 0., 2. * pi);        

  auto *TargetContainer_lid_bottom_Solid = new G4Tubs("TargetContainer_lid_bottom_Solid",0., TargetContainer_lid_Radius,
                TargetContainer_lid_Thickness*0.5, 0., 2. * pi); 


 //defining the logical volumes

  /*G4LogicalVolume* trackerLog
  = new G4LogicalVolume(trackerTube, Al, "Tracker");*/

   auto *TargetContainer_logical = new G4LogicalVolume(
     TargetContainer_Solid, material_pe,"TargetContainer_logical");
   TargetContainer_logical->SetVisAttributes(new G4VisAttributes(grey));

   /*auto *target_96Mo_logical = new G4LogicalVolume(
      target_96Mo_solid, material_96mo, "target_96Mo_logical");*/

   auto *Target92Mo1_logical = 
      new G4LogicalVolume(Target92Mo1_Solid,material_92Mo1,"Target92Mo1_Logical");
   Target92Mo1_logical->SetVisAttributes(new G4VisAttributes(yellow));

   auto* Target92Mo2_logical = 
      new G4LogicalVolume(Target92Mo2_Solid,material_92Mo2,"Target92Mo2_logical");
   Target92Mo2_logical->SetVisAttributes(new G4VisAttributes(doublelightgray));    
   
   auto *TargetContainer_lid_top_logical = 
      new G4LogicalVolume(TargetContainer_lid_top_Solid,material_pe,"TargetContainer_lid_logical");
   TargetContainer_lid_top_logical->SetVisAttributes(new G4VisAttributes(grey)); 

    auto *TargetContainer_lid_bottom_logical = 
      new G4LogicalVolume(TargetContainer_lid_bottom_Solid,material_pe,"TargetContainer_lid_logical");
   TargetContainer_lid_bottom_logical->SetVisAttributes(new G4VisAttributes(grey)); 



//defining the Physical volumes


  auto *target_92Mo_physical =
       new G4PVPlacement(0,global_coordinates,Target92Mo1_logical,
            "Target92Mo",world_logical,false,0,false);

  new G4PVPlacement(
      0,global_coordinates+G4ThreeVector(0.,0.,Target92Mo_Thickness),Target92Mo1_logical,
      "Target92Mo2",world_logical,false,0,false);


  new G4PVPlacement(
      0,global_coordinates + G4ThreeVector(0.,0.,(TargetContainer_lid_Thickness+Target92Mo_Thickness+0.5*Target92Mo_Thickness)),
      TargetContainer_logical,"TargetContainer",world_logical,false,0,false);



  new G4PVPlacement(
      0,global_coordinates + G4ThreeVector(0.,0.,(0.5*(Target92Mo_Thickness+TargetContainer_lid_Thickness))+Target92Mo_Thickness),
      TargetContainer_lid_top_logical,"TargetContainer_lid_top",world_logical,false,0,false);

    /*new G4PVPlacement(
      0,
      global_coordinates +
          G4ThreeVector(0., 0., 0.5 * (thickness_96mo + thickness_pe_disc)),
      target_pe_top_logical, "target_pe_top", world_logical, false, 0, false);*/

  
  new G4PVPlacement(
      0,global_coordinates + G4ThreeVector(0.,0., -0.5*(Target92Mo_Thickness+TargetContainer_lid_Thickness)),
      TargetContainer_lid_bottom_logical,"TargetContainer_lid_bottom",world_logical,false,0,false);

 source_volume = std::make_shared<SourceVolumeTubs>(Target92Mo1_Solid, target_92Mo_physical, 1.);
 //source_volume = std::make_shared<SourceVolumeTubs>(target_96Mo_solid, target_96Mo_physical, 1.);
}
