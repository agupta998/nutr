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

        Copyright (C) 2020-2022 Udo Friman-Gayer
*/

/*
 * General topology of a clover detector was taken from an
 * Eurysis manual for clover detectors.
 */

#include "G4Box.hh"
#include "G4Color.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4PhysicalConstants.hh"
#include "G4SubtractionSolid.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"

#include "HPGe_Clover.hh"

void HPGe_Clover::Construct_Detector(G4LogicalVolume *world_logical,
                                     G4ThreeVector global_coordinates) {

  G4NistManager *nist = G4NistManager::Instance();
  G4ThreeVector symmetry_axis(
      sin(theta) * cos(phi), sin(theta) * sin(phi),
      cos(theta)); // Symmetry axis along which the single elements of the
                   // detector are constructed

  G4RotationMatrix *rotation = new G4RotationMatrix();
  rotation->rotateZ(-phi);
  rotation->rotateY(-theta);
  if (intrinsic_rotation_angle != 0.) {
    rotation->rotateZ(intrinsic_rotation_angle);
  }

  /******** Front end cap *********/

  G4VSolid *end_cap_front_solid =
      rounded_box("end_cap_front_solid", properties.end_cap_front_side_length,
                  properties.end_cap_front_length,
                  properties.end_cap_front_rounding_radius, 20);
  G4LogicalVolume *end_cap_front_logical = new G4LogicalVolume(
      end_cap_front_solid,
      nist->FindOrBuildMaterial(properties.end_cap_material),
      detector_name + "_end_cap_front_logical");
  new G4PVPlacement(
      rotation,
      global_coordinates +
          (dist_from_center + 0.5 * properties.end_cap_front_length) *
              symmetry_axis,
      end_cap_front_logical, detector_name + "end_cap_front", world_logical, 0,
      0, false);

  /******** Vacuum around crystal ********/

  G4VSolid *vacuum_solid = rounded_box(
      "_vacuum_solid",
      properties.end_cap_front_side_length -
          2. * properties.end_cap_front_thickness,
      properties.vacuum_length, properties.end_cap_front_rounding_radius, 20);

  G4LogicalVolume *vacuum_logical = new G4LogicalVolume(
      vacuum_solid, nist->FindOrBuildMaterial("G4_Galactic"),
      detector_name + "_vacuum_logical");
  vacuum_logical->SetVisAttributes(G4Color::Cyan());
  new G4PVPlacement(0,
                    G4ThreeVector(0., 0.,
                                  -0.5 * properties.end_cap_front_length +
                                      properties.end_cap_window_thickness +
                                      0.5 * properties.vacuum_length),
                    vacuum_logical, detector_name + "_vacuum",
                    end_cap_front_logical, 0, 0, false);

  /******** Air at the back of the front end cap ********/

  G4VSolid *air_front_solid = rounded_box(
      "_air_front_solid",
      properties.end_cap_front_side_length -
          2. * properties.end_cap_front_thickness,
      properties.end_cap_front_length - properties.end_cap_window_thickness -
          properties.end_cap_front_thickness - properties.vacuum_length,
      properties.end_cap_front_rounding_radius, 20);

  G4LogicalVolume *air_front_logical =
      new G4LogicalVolume(air_front_solid, nist->FindOrBuildMaterial("G4_AIR"),
                          detector_name + "_air_front_logical");
  air_front_logical->SetVisAttributes(G4Color::Red());
  new G4PVPlacement(
      0,
      G4ThreeVector(0., 0.,
                    -0.5 * properties.end_cap_front_length +
                        properties.end_cap_window_thickness +
                        0.5 * properties.vacuum_length +
                        0.5 * (properties.end_cap_front_length -
                               properties.end_cap_front_thickness -
                               properties.end_cap_window_thickness)),
      air_front_logical, detector_name + "_air_front", end_cap_front_logical, 0,
      0, false);

  /******** Crystals ********/

  G4Tubs *crystal_original = new G4Tubs(
      detector_name + "_crystal_original", 0., properties.crystal_radius,
      properties.crystal_length * 0.5, 0., twopi);
  G4Box *subtraction_solid =
      new G4Box(detector_name + "_subtraction_solid", properties.crystal_radius,
                properties.crystal_radius, properties.crystal_length);
  G4SubtractionSolid *crystal_step1_solid = new G4SubtractionSolid(
      detector_name + "_crystal_step1_solid", crystal_original,
      subtraction_solid, 0,
      G4ThreeVector(properties.crystal_radius + 23. * mm, 0., 0.));
  G4SubtractionSolid *crystal_step2_solid = new G4SubtractionSolid(
      detector_name + "_crystal_step2_solid", crystal_step1_solid,
      subtraction_solid, 0,
      G4ThreeVector(-properties.crystal_radius - 22. * mm, 0., 0.));
  G4SubtractionSolid *crystal_step3_solid = new G4SubtractionSolid(
      detector_name + "_crystal_step3_solid", crystal_step2_solid,
      subtraction_solid, 0,
      G4ThreeVector(0., -properties.crystal_radius - 22. * mm, 0.));
  G4SubtractionSolid *crystal_step4_solid = new G4SubtractionSolid(
      detector_name + "_crystal_step4_solid", crystal_step3_solid,
      subtraction_solid, 0,
      G4ThreeVector(0., properties.crystal_radius + 23. * mm, 0.));

  sensitive_logical_volumes.push_back(new G4LogicalVolume(
      crystal_step4_solid, nist->FindOrBuildMaterial("G4_Ge"),
      detector_name + "_1"));
  sensitive_logical_volumes[0]->SetVisAttributes(
      new G4VisAttributes(G4Color::Blue()));
  new G4PVPlacement(0,
                    G4ThreeVector(22. * mm + 0.5 * properties.crystal_gap,
                                  22. * mm + 0.5 * properties.crystal_gap,
                                  -0.5 * properties.vacuum_length +
                                      0.5 * properties.crystal_length +
                                      properties.end_cap_to_crystal_gap_front),
                    sensitive_logical_volumes[0], detector_name + "_crystal_1",
                    vacuum_logical, 0, 0, false);

  G4RotationMatrix *rotate2 = new G4RotationMatrix();
  rotate2->rotateZ(-90. * deg);
  sensitive_logical_volumes.push_back(new G4LogicalVolume(
      crystal_step4_solid, nist->FindOrBuildMaterial("G4_Ge"),
      detector_name + "_2"));
  sensitive_logical_volumes[1]->SetVisAttributes(
      new G4VisAttributes(G4Color::Red()));
  new G4PVPlacement(rotate2,
                    G4ThreeVector(-22. * mm - 0.5 * properties.crystal_gap,
                                  22. * mm + 0.5 * properties.crystal_gap,
                                  -0.5 * properties.vacuum_length +
                                      0.5 * properties.crystal_length +
                                      properties.end_cap_to_crystal_gap_front),
                    sensitive_logical_volumes[1], detector_name + "_crystal_2",
                    vacuum_logical, 0, 0, false);

  G4RotationMatrix *rotate3 = new G4RotationMatrix();
  rotate3->rotateZ(-270. * deg);
  sensitive_logical_volumes.push_back(new G4LogicalVolume(
      crystal_step4_solid, nist->FindOrBuildMaterial("G4_Ge"),
      detector_name + "_3"));
  sensitive_logical_volumes[2]->SetVisAttributes(
      new G4VisAttributes(G4Color::Brown()));
  new G4PVPlacement(rotate3,
                    G4ThreeVector(22. * mm + 0.5 * properties.crystal_gap,
                                  -22. * mm - 0.5 * properties.crystal_gap,
                                  -0.5 * properties.vacuum_length +
                                      0.5 * properties.crystal_length +
                                      properties.end_cap_to_crystal_gap_front),
                    sensitive_logical_volumes[2], detector_name + "_crystal_3",
                    vacuum_logical, 0, 0, false);

  G4RotationMatrix *rotate4 = new G4RotationMatrix();
  rotate4->rotateZ(-180. * deg);
  sensitive_logical_volumes.push_back(new G4LogicalVolume(
      crystal_step4_solid, nist->FindOrBuildMaterial("G4_Ge"),
      detector_name + "_4"));
  sensitive_logical_volumes[3]->SetVisAttributes(
      new G4VisAttributes(G4Color::Green()));
  new G4PVPlacement(rotate4,
                    G4ThreeVector(-22. * mm - 0.5 * properties.crystal_gap,
                                  -22. * mm - 0.5 * properties.crystal_gap,
                                  -0.5 * properties.vacuum_length +
                                      0.5 * properties.crystal_length +
                                      properties.end_cap_to_crystal_gap_front),
                    sensitive_logical_volumes[3], detector_name + "_crystal_4",
                    vacuum_logical, 0, 0, false);

  /******** Back end cap *********/

  G4VSolid *end_cap_back_solid =
      rounded_box("end_cap_back_solid", properties.end_cap_back_side_length,
                  properties.end_cap_back_length,
                  properties.end_cap_back_rounding_radius, 20);
  G4LogicalVolume *end_cap_back_logical = new G4LogicalVolume(
      end_cap_back_solid,
      nist->FindOrBuildMaterial(properties.end_cap_material),
      detector_name + "_end_cap_back_logical");
  new G4PVPlacement(rotation,
                    global_coordinates +
                        (dist_from_center + properties.end_cap_front_length +
                         0.5 * properties.end_cap_back_length) *
                            symmetry_axis,
                    end_cap_back_logical, detector_name + "end_cap_back",
                    world_logical, 0, 0, false);

  /******** Air inside the back end cap ********/

  G4VSolid *air_back_solid = rounded_box(
      "_air_back_solid",
      properties.end_cap_back_side_length -
          2. * properties.end_cap_back_thickness,
      properties.end_cap_back_length - 2. * properties.end_cap_back_thickness,
      properties.end_cap_back_rounding_radius, 20);

  G4LogicalVolume *air_back_logical =
      new G4LogicalVolume(air_back_solid, nist->FindOrBuildMaterial("G4_AIR"),
                          detector_name + "_air_back_logical");
  air_back_logical->SetVisAttributes(G4Color::Red());
  new G4PVPlacement(
      0,
      G4ThreeVector(0., 0.,
                    -0.5 * properties.end_cap_back_length +
                        properties.end_cap_back_thickness +
                        0.5 * (properties.end_cap_back_length -
                               2. * properties.end_cap_back_thickness)),
      air_back_logical, detector_name + "_air_back", end_cap_back_logical, 0, 0,
      false);

  if (use_dewar) {
    /************* Connection dewar-detector *************/
    G4Tubs *connection_solid = new G4Tubs(
        detector_name + "connection_solid", 0., properties.connection_radius,
        properties.connection_length * 0.5, 0., twopi);
    G4LogicalVolume *connection_logical = new G4LogicalVolume(
        connection_solid,
        nist->FindOrBuildMaterial(properties.connection_material),
        detector_name + "connection_logical");
    connection_logical->SetVisAttributes(new G4VisAttributes(G4Color::White()));
    new G4PVPlacement(rotation,
                      global_coordinates +
                          (dist_from_center + properties.end_cap_front_length +
                           properties.end_cap_back_length +
                           properties.connection_length * 0.5) *
                              symmetry_axis,
                      connection_logical, detector_name + "connection",
                      world_logical, 0, 0, false);

    /************* Dewar *************/
    G4double dewar_side_length =
        properties.dewar_length - 2. * properties.dewar_wall_thickness;

    // Dewar face
    G4Tubs *dewar_face_solid = new G4Tubs(
        detector_name + "_dewar_face_solid", 0., properties.dewar_outer_radius,
        properties.dewar_wall_thickness * 0.5, 0., twopi);
    G4LogicalVolume *dewar_face_logical = new G4LogicalVolume(
        dewar_face_solid, nist->FindOrBuildMaterial(properties.dewar_material),
        detector_name + "_dewar_face_logical");
    dewar_face_logical->SetVisAttributes(new G4VisAttributes(G4Color::Brown()));
    new G4PVPlacement(rotation,
                      global_coordinates +
                          (dist_from_center + properties.end_cap_front_length +
                           properties.end_cap_back_length +
                           properties.connection_length +
                           properties.dewar_wall_thickness * 0.5) *
                              symmetry_axis,
                      dewar_face_logical, detector_name + "_dewar_face",
                      world_logical, 0, 0, false);

    // Dewar side
    G4Tubs *dewar_side_solid = new G4Tubs(
        detector_name + "_dewar_side_solid",
        properties.dewar_outer_radius - properties.dewar_wall_thickness,
        properties.dewar_outer_radius, dewar_side_length * 0.5, 0., twopi);
    G4LogicalVolume *dewar_side_logical = new G4LogicalVolume(
        dewar_side_solid, nist->FindOrBuildMaterial(properties.dewar_material),
        detector_name + "_dewar_side_logical");
    dewar_side_logical->SetVisAttributes(new G4VisAttributes(G4Color::Brown()));
    new G4PVPlacement(
        rotation,
        global_coordinates +
            (dist_from_center + properties.end_cap_front_length +
             properties.end_cap_back_length + properties.connection_length +
             properties.dewar_wall_thickness + dewar_side_length * 0.5) *
                symmetry_axis,
        dewar_side_logical, detector_name + "_dewar_side", world_logical, 0, 0,
        false);

    // Dewar base
    G4Tubs *dewar_base_solid = new G4Tubs(
        detector_name + "_dewar_base_solid", 0., properties.dewar_outer_radius,
        properties.dewar_wall_thickness * 0.5, 0., twopi);
    G4LogicalVolume *dewar_base_logical = new G4LogicalVolume(
        dewar_base_solid, nist->FindOrBuildMaterial(properties.dewar_material),
        detector_name + "_dewar_base_logical");
    dewar_base_logical->SetVisAttributes(new G4VisAttributes(G4Color::Brown()));
    new G4PVPlacement(rotation,
                      global_coordinates +
                          (dist_from_center + properties.end_cap_front_length +
                           properties.end_cap_back_length +
                           properties.connection_length + dewar_side_length +
                           1.5 * properties.dewar_wall_thickness) *
                              symmetry_axis,
                      dewar_base_logical, detector_name + "_dewar_base",
                      world_logical, 0, 0, false);
  }
}

G4VSolid *HPGe_Clover::rounded_box(const G4String name,
                                   const G4double side_length,
                                   const G4double length,
                                   const G4double rounding_radius,
                                   const G4int n_points_per_corner) {

  G4double inverse_n_points_per_corner = 1. / (n_points_per_corner - 1.);
  vector<G4TwoVector> base(4 * n_points_per_corner);

  for (int i = 0; i < n_points_per_corner; ++i) {
    base[i] =
        G4TwoVector(-0.5 * side_length + 0.5 * rounding_radius -
                        0.5 * rounding_radius *
                            sin(0.5 * pi * i * inverse_n_points_per_corner),
                    -0.5 * side_length + 0.5 * rounding_radius -
                        0.5 * rounding_radius *
                            cos(0.5 * pi * i * inverse_n_points_per_corner));
    base[i + n_points_per_corner] =
        G4TwoVector(-0.5 * side_length + 0.5 * rounding_radius -
                        0.5 * rounding_radius *
                            cos(0.5 * pi * i * inverse_n_points_per_corner),
                    0.5 * side_length - 0.5 * rounding_radius +
                        0.5 * rounding_radius *
                            sin(0.5 * pi * i * inverse_n_points_per_corner));
    base[i + 2 * n_points_per_corner] =
        G4TwoVector(0.5 * side_length - 0.5 * rounding_radius +
                        0.5 * rounding_radius *
                            sin(0.5 * pi * i * inverse_n_points_per_corner),
                    0.5 * side_length - 0.5 * rounding_radius +
                        0.5 * rounding_radius *
                            cos(0.5 * pi * i * inverse_n_points_per_corner));
    base[i + 3 * n_points_per_corner] =
        G4TwoVector(0.5 * side_length - 0.5 * rounding_radius +
                        0.5 * rounding_radius *
                            cos(0.5 * pi * i * inverse_n_points_per_corner),
                    -0.5 * side_length + 0.5 * rounding_radius -
                        0.5 * rounding_radius *
                            sin(0.5 * pi * i * inverse_n_points_per_corner));
  }

  return new G4ExtrudedSolid(name, base, length * 0.5, 0., 1., 0., 1.);
}

G4VSolid *HPGe_Clover::Filter_Shape(const string name,
                                    const Filter &filter) const {
  return rounded_box(name, filter.radius, filter.thickness * 0.5,
                     properties.end_cap_front_rounding_radius, 20);
}