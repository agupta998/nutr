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

        Copyright (C) 2020-2022 Udo Friman-Gayer and Oliver Papst
*/

#include <memory>
#include <vector>

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "DetectorConstruction.hh"
#include "BeamPipe.hh"
#include "CeBr3_2x2.hh"
#include "CollimatorRoom.hh"
#include "GammaVault.hh"
#include "HPGe_Clover.hh"
#include "HPGe_Coaxial.hh"
#include "HPGe_Collection.hh"
#include "LaBr3Ce_3x3.hh"
#include "LeadShieldingUTR_2022-11-14_to_2022-11-24.hh"
#include "MOLLY.hh"
#include "Mechanical.hh"
#include "Target92Mo.hh"
#include "ZeroDegreeMechanical.hh"

#include "DetectorConstructionConfig.hh"

constexpr double inch = 25.4 * mm;

constexpr double cu_thinner = 0.03858268 * inch;
constexpr double cu_thin = 0.07795276 *inch;
constexpr double cu_thick = 0.0787402 *inch;
constexpr double pb_thin = 0.0984252 * inch;
constexpr double pb_thick = 0.19685 * inch;

vector<Detector *> detectors = {
    new HPGe_Clover("clover_1", HPGe_Clover_Collection::HPGe_Clover_Yale,
                    90. * deg, 0. * deg, 5.1181 * inch,
                    {{{"G4_Cu", cu_thinner}, {"G4_Pb", pb_thick}}, true}, {},
                    1.5 * pi),
    new HPGe_Clover("clover_3", HPGe_Clover_Collection::HPGe_Clover_Yale,
                    90. * deg, 90. * deg, 5.1181 * inch,
                    {{{"G4_Cu", cu_thinner}, {"G4_Pb", pb_thick}}, true}, {},
                    0.5 * pi),
    /*new HPGe_Clover("clover_5", HPGe_Clover_Collection::HPGe_Clover_Yale,
                    90. * deg, 180. * deg, 5.88 * inch,
                    {{{"G4_Cu", cu_thin}, {"G4_Pb", pb_thin}}, true}, {},
                    0.5 * pi), */
    /*new HPGe_Clover("clover_7", HPGe_Clover_Collection::HPGe_Clover_Yale,
                    90. * deg, 315. * deg, 6.38 * inch,
                    {{{"G4_Cu", cu_thin}, {"G4_Pb", pb_thin}}, true}, {}, 0.), */

    new HPGe_Clover("clover_B2", HPGe_Clover_Collection::HPGe_Clover_Yale,
                    135. * deg, 0. * deg, 4.72441 * inch,
                    {{{"G4_Cu", cu_thinner}, {"G4_Pb", pb_thin}}, true}, {},
                    0.5 * pi),
    new HPGe_Clover("clover_B4", HPGe_Clover_Collection::HPGe_Clover_Yale,
                    125.26 * deg, 135. * deg, 4.133845* inch,
                    {{{"G4_Cu", cu_thinner}, {"G4_Pb", pb_thin}}, true}, {},
                    0.5 * pi),
    /*new HPGe_Coaxial(
        "coaxial_B4", detector_construction_config.Coaxial_B4,
        detector_construction_config.Coaxial_B4_Dewar, 125.26 * deg, 135. * deg,
        9.25 * inch,
        {
            {{"G4_Cu",
              [](const string name) {
                return new G4Box(name, 50.5 * mm, 50.5 * mm, 0.5 * cu_thin);
              },
              cu_thin},
             {"G4_Pb",
              [](const string name) {
                return new G4Box(name, 50.5 * mm, 50.5 * mm, 0.5 * pb_thinner);
              },
              pb_thinner}},
        }),*/
    /*new HPGe_Clover("clover_B5", HPGe_Clover_Collection::HPGe_Clover_Yale,
                    135. * deg, 180. * deg, 8.00 * inch,
                    {{{"G4_Cu", cu_thin}, {"G4_Pb", pb_thin}}, true}, {},
                    1.5 * pi), */

    new CeBr3_2x2("cebr_C", 90. * deg, 45.0 * deg, 2.952 * inch,
                  {{{"G4_Cu", cu_thick}}, true}),
    new CeBr3_2x2("cebr_G", 90. * deg, 135.0 * deg, 8.0 * inch,
                  {{{"G4_Cu", cu_thick}}, true}),
    new CeBr3_2x2("cebr_O", 90. * deg, 315.0 * deg, 8.0 * inch,
                  {{{"G4_Cu", cu_thick}}, true}),
    new CeBr3_2x2("cebr_BD", 135. * deg, 38.571 * deg, 8.0 * inch,
                  {{{"G4_Cu", cu_thick}}, true}),
   
    new LaBr3Ce_3x3("labr_5", 90. * deg, 180. * deg, 1.9685 * inch,
                    {{{"G4_Cu", cu_thin}}}),
    new LaBr3Ce_3x3("labr_7", 90. * deg, 270. * deg, 2.9527 * inch,
                    {{{"G4_Cu", cu_thin}}}),
    new LaBr3Ce_3x3("labr_BI", 135. * deg, 218.5714 * deg, 3.14961 * inch,
                    {{{"G4_Cu", cu_thin}}}),
    new LaBr3Ce_3x3("labr_BM", 135. * deg, 321.428 * deg, 3.14961 * inch,
                    {{{"G4_Cu", cu_thin}}}),

    new HPGe_Coaxial("zero_degree",
                     HPGe_Coaxial_Collection::HPGe_120_TUNL_40383,
                     HPGe_Coaxial_Dewar_Properties(), 0. * deg, 180. * deg,
                     ZeroDegreeMechanical::zero_degree_to_target),
    new MOLLY(0. * deg, 0. * deg, 11. * m)};

G4VPhysicalVolume *DetectorConstruction::Construct() {

  ConstructBoxWorld(2. * m, 2. * m, 12.0 * m);

  CollimatorRoom(world_logical).Construct({});
  BeamPipe(world_logical).Construct({});
  LeadShieldingUTR(world_logical).Construct({});
  Mechanical(world_logical).Construct({});
  ZeroDegreeMechanical(world_logical)
      .Construct(G4ThreeVector(zero_degree_x, zero_degree_y, 0.));
  GammaVault(world_logical).Construct({});

  for (size_t n_detector = 0; n_detector < detectors.size() - 2; ++n_detector) {
    detectors[n_detector]->Construct(world_logical, {});
    RegisterSensitiveLogicalVolumes(
        detectors[n_detector]->get_sensitive_logical_volumes());
  }
  detectors[detectors.size() - 2]->Construct(
      world_logical, G4ThreeVector(zero_degree_x, zero_degree_y, 0.));
  RegisterSensitiveLogicalVolumes(
      detectors[detectors.size() - 2]->get_sensitive_logical_volumes());
  detectors[detectors.size() - 1]->Construct(world_logical,
                                             G4ThreeVector(molly_x, 0., 0.));
  RegisterSensitiveLogicalVolumes(
      detectors[detectors.size() - 1]->get_sensitive_logical_volumes());

  if constexpr (detector_construction_config.use_target) {
    auto target = Target92Mo(world_logical);
    target.Construct({});
    source_volumes.push_back(target.get_source_volume());
  }

  return world_phys;
}
