/**
 * @file HelloName.hpp
 *
 * Developer(s) of this DAQModule have yet to replace this line with a brief description of the DAQModule.
 *
 * This is part of the DUNE DAQ Software Suite, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef HELLO_PLUGINS_HELLONAME_HPP_
#define HELLO_PLUGINS_HELLONAME_HPP_

#include "appfwk/DAQModule.hpp"
#include "iomanager/Receiver.hpp" //because we want to receive name parameter from the NameReceiver module
#include "iomanager/Sender.hpp" //because we want to send a greeting data to HelloToYou module
#include "utilities/WorkerThread.hpp"
#include "ers/Issue.hpp"
#include "StringWrapper.hpp"

#include <memory>
#include <atomic>
#include <limits>
#include <string>


namespace dunedaq {
namespace hello {

class HelloName : public dunedaq::appfwk::DAQModule
{
public:
  explicit HelloName(const std::string& name);

  void init(const data_t&) override;
  void get_info(opmonlib::InfoCollector&, int /*level*/) override;

  HelloName(const HelloName&) = delete;
  HelloName& operator=(const HelloName&) = delete;
  HelloName(HelloName&&) = delete;
  HelloName& operator=(HelloName&&) = delete;

  ~HelloName() = default;

private:
//Commands
void do_start(const nlohmann::json& obj);
void do_stop(const nlohmann::json& obj);

//Threading
dunedaq::utilities::WorkerThread thread_;
void do_work(std::atomic<bool>&);

//Configuration
using source_t = dunedaq::iomanager::ReceiverConcept<String>;
std::shared_ptr<source_t> inputQueue_;
using sink_t = dunedaq::iomanager::SenderConcept<String>;
std::shared_ptr<sink_t> outputQueue_;
std::chrono::milliseconds queueTimeout_;
};

// Statistic counters
std::atomic<int64_t> receivedCount {0};
std::atomic<int>     sentCount {0};


}// namespace hello
}//namespace dunedaq

#endif // HELLO_PLUGINS_HELLONAME_HPP_
