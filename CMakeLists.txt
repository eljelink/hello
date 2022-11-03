cmake_minimum_required(VERSION 3.12)
project(hello VERSION 0.0.0)

find_package(daq-cmake REQUIRED)

daq_setup_environment()

find_package(appfwk REQUIRED)
find_package(opmonlib REQUIRED)
find_package(iomanager REQUIRED)
find_package(logging REQUIRED)
find_package(ers REQUIRED)
find_package(daqconf REQUIRED)

##############################################################################

daq_codegen(namereceiver.jsonnet TEMPLATES Structs.hpp.j2 Nljs.hpp.j2)
daq_codegen(namereceiverinfo.jsonnet DEP_PKGS opmonlib TEMPLATES opmonlib/InfoStructs.hpp.j2 opmonlib/InfoNljs.hpp.j2)
daq_codegen(confgen.jsonnet DEP_PKGS daqconf TEMPLATES Structs.hpp.j2)
daq_codegen(hellonameinfo.jsonnet DEP_PKGS opmonlib TEMPLATES opmonlib/InfoStructs.hpp.j2 opmonlib/InfoNljs.hpp.j2)
daq_codegen(hellotoyouinfo.jsonnet DEP_PKGS opmonlib TEMPLATES opmonlib/InfoStructs.hpp.j2 opmonlib/InfoNljs.hpp.j2)

##############################################################################

daq_add_plugin(NameReceiver duneDAQModule LINK_LIBRARIES appfwk::appfwk) # Replace appfwk library with a more specific library when appropriate
daq_add_plugin(HelloName duneDAQModule LINK_LIBRARIES appfwk::appfwk) # Replace appfwk library with a more specific library when appropriate
daq_add_plugin(HelloToYou duneDAQModule LINK_LIBRARIES appfwk::appfwk) # Replace appfwk library with a more specific library when appropriate

##############################################################################

daq_install()

