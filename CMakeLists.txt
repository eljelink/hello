

# This is a skeleton CMakeLists.txt file, auto-generated on
# Mon Oct 31 12:50:26 CET 2022.  The developer(s) of this package should delete
# this comment as well as adding dependent targets, packages,
# etc. specific to the package. For details on how to write a package,
# please see
# https://dune-daq-sw.readthedocs.io/en/latest/packages/daq-cmake/

cmake_minimum_required(VERSION 3.12)
project(hello VERSION 0.0.0)

find_package(daq-cmake REQUIRED)

daq_setup_environment()


find_package(appfwk REQUIRED)
find_package(opmonlib REQUIRED)
#find_package(Boost COMPONENTS unit_test_framework REQUIRED)
find_package(dfmessages REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(timinglibs REQUIRED)
find_package(timing REQUIRED)
find_package(iomanager REQUIRED)
find_package(opmonlib REQUIRED)
find_package(appfwk REQUIRED)
find_package(rcif REQUIRED)
find_package(logging REQUIRED)
find_package(ers REQUIRED)
find_package(daqconf REQUIRED)

##############################################################################


# See https://dune-daq-sw.readthedocs.io/en/latest/packages/daq-cmake/#daq_codegen

daq_codegen(namereceiver.jsonnet TEMPLATES Structs.hpp.j2 Nljs.hpp.j2)
daq_codegen(namereceiverinfo.jsonnet DEP_PKGS opmonlib TEMPLATES opmonlib/InfoStructs.hpp.j2 opmonlib/InfoNljs.hpp.j2)
daq_codegen(confgen.jsonnet DEP_PKGS daqconf TEMPLATES Structs.hpp.j2)
daq_codegen(hellonameinfo.jsonnet DEP_PKGS opmonlib TEMPLATES opmonlib/InfoStructs.hpp.j2 opmonlib/InfoNljs.hpp.j2)
daq_codegen(hellotoyouinfo.jsonnet DEP_PKGS opmonlib TEMPLATES opmonlib/InfoStructs.hpp.j2 opmonlib/InfoNljs.hpp.j2)

##############################################################################


# See https://dune-daq-sw.readthedocs.io/en/latest/packages/daq-cmake/#daq_add_library

# daq_add_library( LINK_LIBRARIES ) # Any source files and/or dependent libraries to link in not yet determined

##############################################################################


# See https://dune-daq-sw.readthedocs.io/en/latest/packages/daq-cmake/#daq_add_plugin

daq_add_plugin(NameReceiver duneDAQModule LINK_LIBRARIES appfwk::appfwk) # Replace appfwk library with a more specific library when appropriate
daq_add_plugin(HelloName duneDAQModule LINK_LIBRARIES appfwk::appfwk) # Replace appfwk library with a more specific library when appropriate
daq_add_plugin(HelloToYou duneDAQModule LINK_LIBRARIES appfwk::appfwk) # Replace appfwk library with a more specific library when appropriate

##############################################################################


# See https://dune-daq-sw.readthedocs.io/en/latest/packages/daq-cmake/#daq_add_unit_test

# daq_add_unit_test(Placeholder_test LINK_LIBRARIES)  # Placeholder_test should be replaced with real unit tests

##############################################################################

daq_install()

