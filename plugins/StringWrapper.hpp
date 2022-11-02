/**
  * StringWrapper.hpp
  *
  * StringWrapper wraps a std::string so that it can be transmitted over the network using the Unified Communications
  * API (iomanager)
  *
  * This is part of the DUNE DAQ Software Suite, copyright 2020.
  * Licensing/copyright details are in the COPYING file that you should have
  * received with this code.
  *
 */

#ifndef HELLO_PLUGINS_STRINGWRAPPER_HPP_
#define HELLO_PLUGINS_STRINGWRAPPER_HPP_

#include "serialization/Serialization.hpp" // because I want to send something

#include <string> // because I work with string parameter = name or greeting sentence

namespace dunedaq {
namespace hello {
struct String
{
  std::string conversation; // defition of new variable which I will serialize

  String() = default;
  explicit String(std::string const& l)
    : conversation(l.begin(), l.end())
  {}


  DUNE_DAQ_SERIALIZE(String, conversation); // serialization done
};
} // namespace hello
DUNE_DAQ_SERIALIZABLE(hello::String);
} // namespace dunedaq

#endif // HELLO_PLUGINS_STRINGWRAPPER_HPP_

