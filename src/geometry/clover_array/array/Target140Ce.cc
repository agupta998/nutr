
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

#include "Target140Ce.hh"

void Target140Ce::Construct(const G4ThreeVector global_coordinates) {

  auto darkgray = G4Colour(0.2, 0.2, 0.2);
  auto lightgray = G4Colour(0.6, 0.6, 0.6, 0.5);
  auto doublelightgray = G4Colour(0.6, 0.6, 0.6, 0.75);

  G4NistManager *nist = G4NistManager::Instance();
  auto *material_pe = nist->FindOrBuildMaterial("G4_POLYETHYLENE");

  auto radius_140Ce = 0.5 * diameter_140Ce;
  auto thickness_pe = (0.5 * mass_pe / density_pe) / (side_pe * side_pe);
  auto thickness_140Ce = thickness_tot - 2 * thickness_pe;
  auto mass_pe_disc =
      pi * radius_140Ce * radius_140Ce * thickness_pe * density_pe;
  auto volume_pe_shell = 2 * pi * radius_140Ce * thickness_pe * thickness_140Ce;
  auto mass_pe_shell = volume_pe_shell * density_pe;
  auto mass_pe_squarehole = mass_pe - 2 * mass_pe_disc - mass_pe_shell;
  auto radius_pe_hole = radius_140Ce + thickness_pe;
  auto area_pe_squarehole =
      side_pe * side_pe - pi * radius_pe_hole * radius_pe_hole;
  auto thickness_pe_squarehole =
      mass_pe_squarehole / density_pe / area_pe_squarehole;
  auto radius_pe_disc = radius_140Ce + thickness_pe;
  auto area_pe_disc = pi * radius_pe_disc * radius_pe_disc;
  auto thickness_pe_disc = mass_pe_disc / density_pe / area_pe_disc;

  G4Isotope *Ce140 = new G4Isotope("140Ce", 58, 140);

  G4Element *enriched_140Ce = new G4Element("enriched 140Ce", "140Ce-hzdr", 1);
  enriched_140Ce->AddIsotope(Ce140, 0.995);

  auto density_140Ce = mass_140Ce / (radius_140Ce * radius_140Ce * pi);
  auto *material_140Ce = new G4Material("mat_140Ce", density_140Ce, 1);
  material_140Ce->AddElement(enriched_140Ce, 1);

  auto *target_140Ce_solid = new G4Tubs("target_140Ce_solid", 0., radius_140Ce,
                                       0.5 * thickness_140Ce, 0., 2. * pi);

  auto *target_pe_top_solid =
      new G4Tubs("target_pe_top_solid", 0., radius_pe_disc,
                 0.5 * thickness_pe_disc, 0., 2. * pi);
  auto *target_pe_bottom_solid =
      new G4Tubs("target_pe_bottom_solid", 0., radius_pe_disc,
                 0.5 * thickness_pe_disc, 0., 2. * pi);
  auto *target_pe_square_solid =
      new G4Box("target_pe_square", 0.5 * side_pe, 0.5 * side_pe,
                0.5 * thickness_pe_squarehole);
  auto *target_pe_hole_solid = new G4Tubs("target_pe_hole", 0., radius_pe_hole,
                                          thickness_pe_squarehole, 0., 2. * pi);
  auto *target_pe_squarehole_solid = new G4SubtractionSolid(
      "target_pe_squarehole", target_pe_square_solid, target_pe_hole_solid);
  auto *target_pe_shell_solid =
      new G4Tubs("target_pe_shell", radius_140Ce, radius_pe_hole,
                 0.5 * thickness_140Ce, 0., 2. * pi);

  auto *target_140Ce_logical = new G4LogicalVolume(
      target_140Ce_solid, material_140Ce, "target_140Ce_logical");
  target_140Ce_logical->SetVisAttributes(darkgray);

  auto *target_pe_top_logical = new G4LogicalVolume(
      target_pe_top_solid, material_pe, "target_pe_top_logical");
  target_pe_top_logical->SetVisAttributes(lightgray);
  auto *target_pe_bottom_logical = new G4LogicalVolume(
      target_pe_bottom_solid, material_pe, "target_pe_bottom_logical");
  target_pe_bottom_logical->SetVisAttributes(lightgray);
  auto *target_pe_squarehole_logical = new G4LogicalVolume(
      target_pe_squarehole_solid, material_pe, "target_pe_squarehole_logical");
  target_pe_squarehole_logical->SetVisAttributes(doublelightgray);
  auto *target_pe_shell_logical = new G4LogicalVolume(
      target_pe_shell_solid, material_pe, "target_pe_shell_logical");
  target_pe_shell_logical->SetVisAttributes(lightgray);

  auto *target_140Ce_physical =
      new G4PVPlacement(0, global_coordinates, target_140Ce_logical,
                        "target_140Ce", world_logical, false, 0, false);
  new G4PVPlacement(0, global_coordinates, target_pe_shell_logical,
                    "target_pe_shell", world_logical, false, 0, false);
  new G4PVPlacement(0, global_coordinates, target_pe_squarehole_logical,
                    "target_pe_squarehole", world_logical, false, 0, false);
  new G4PVPlacement(
      0,
      global_coordinates +
          G4ThreeVector(0., 0., 0.5 * (thickness_140Ce + thickness_pe_disc)),
      target_pe_top_logical, "target_pe_top", world_logical, false, 0, false);
  new G4PVPlacement(
      0,
      global_coordinates +
          G4ThreeVector(0., 0., -0.5 * (thickness_140Ce + thickness_pe_disc)),
      target_pe_bottom_logical, "target_pe_bottom", world_logical, false, 0,
      false);

  source_volume = std::make_shared<SourceVolumeTubs>(target_140Ce_solid, target_140Ce_physical, 1.);
}

