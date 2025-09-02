# This file is part of nutr.
#
# nutr is free software: you can redistribute it and/or modify it under the
# terms of the GNU General Public License as published by the Free Software
# Foundation, either version 3 of the License, or (at your option) any later
# version.
#
# nutr is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
# A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with
# nutr.  If not, see <https://www.gnu.org/licenses/>.
#
# Copyright (C) 2020-2022 Udo Friman-Gayer and Oliver Papst

#set(COAXIAL_B4
#   "HPGe_ANL_41203"
 #   CACHE STRING "Detector at position B4")
#set_property(CACHE COAXIAL_B4 PROPERTY STRINGS "HPGe_ANL_41203"
 #                                      "HPGe_ANL_31670")
#set(COAXIAL_B4_DEWAR
  #  "${COAXIAL_B4}_Dewar"
   # CACHE STRING "Dewar of detector B4")
option(USE_TARGET "Place the target at the target position" ON)
configure_file(
  "${CMAKE_CURRENT_LIST_DIR}/DetectorConstructionConfig.hh.in"
  "${PROJECT_BINARY_DIR}/src/geometry/clover_array/2025-09-01/DetectorConstructionConfig.hh"
)

add_library(detectorConstruction_2025-09-01   
            DetectorConstruction.cc)
target_include_directories(
  detectorConstruction_2025-09-01
  PUBLIC
    ${PROJECT_SOURCE_DIR}/include/detectors
    ${PROJECT_SOURCE_DIR}/include/geometry
    ${PROJECT_SOURCE_DIR}/include/geometry/clover_array/2025-09-01
    ${PROJECT_SOURCE_DIR}/include/geometry/clover_array/array
    ${PROJECT_SOURCE_DIR}/include/sensitive_detector/${SENSITIVE_DETECTOR_DIR}
    ${PROJECT_BINARY_DIR}/src/geometry/clover_array/2025-09-01)
target_link_libraries(
  detectorConstruction_2025-09-01
  beamPipe
  cebr3_2x2
  collimatorRoom
  target140Ce
  gamma_vault
  hpgeClover
  hpgeCoaxial
  labr3ce_3x3
  leadShieldingUTR_2022-11-14_to_2022-11-24
  mechanical
  molly
  nDetectorConstruction
  SensitiveDetector
  zero_degree_mechanical)

# Cmake command that creates a directory using the 'file(Make_Directory)'
# command. It creats a new directory with specified
# path relative to the '${PROJECT_BINARY_DIR}/2025-09-01'

file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/2025-09-01) 

# Cmake command that defines an executable
# target using the 'add_executable()' command. It specifies the target name as
# "gps_2025-09-01" and includes a source file named "nutr.cc" located in 
#the '${PROJECT_SOURCE_DIR}/src/fundamentals'

add_executable(gps_2025-09-01 
               ${PROJECT_SOURCE_DIR}/src/fundamentals/nutr.cc)
set_target_properties(
  gps_2025-09-01
  PROPERTIES RUNTIME_OUTPUT_DIRECTORY
             ${CMAKE_BINARY_DIR}/2025-09-01)
target_include_directories(
  gps_2025-09-01
  PUBLIC ${PROJECT_SOURCE_DIR}/include/fundamentals
         ${PROJECT_SOURCE_DIR}/include/geometry
         ${PROJECT_SOURCE_DIR}/include/physics)
target_link_libraries(
  gps_2025-09-01 actionInitialization
  ${Boost_LIBRARIES} detectorConstruction_2025-09-01 physics
  ${Geant4_LIBRARIES})

file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/2025-09-01)
add_executable(angcorr_2025-09-01
               ${PROJECT_SOURCE_DIR}/src/fundamentals/nutr.cc)
set_target_properties(
  angcorr_2025-09-01
  PROPERTIES RUNTIME_OUTPUT_DIRECTORY
             ${CMAKE_BINARY_DIR}/2025-09-01)
target_include_directories(
  angcorr_2025-09-01
  PUBLIC ${PROJECT_SOURCE_DIR}/include/fundamentals
         ${PROJECT_SOURCE_DIR}/include/geometry
         ${PROJECT_SOURCE_DIR}/include/physics)
target_link_libraries(
  angcorr_2025-09-01
  actionInitialization ${Boost_LIBRARIES}
  detectorConstruction_2025-09-01 physics ${Geant4_LIBRARIES})
