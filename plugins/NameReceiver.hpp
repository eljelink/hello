/**
 * @file NameReceiver.hpp
 *
 * Developer(s) of this DAQModule have yet to replace this line with a brief description of the DAQModule.
 *
 * This is part of the DUNE DAQ Software Suite, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef HELLO_PLUGINS_NAMERECEIVER_HPP_
#define HELLO_PLUGINS_NAMERECEIVER_HPP_

#include "appfwk/DAQModule.hpp" //default
#include "iomanager/Sender.hpp" // because I want to send the name parameter to HelloName module
#include "ers/Issue.hpp" //because I have ers messages
#include "utilities/WorkerThread.hpp"
//#include "hello/namereceiver/Nljs.hpp"
#include "hello/namereceiver/Structs.hpp"

#include "StringWrapper.hpp"

#include <atomic>
#include <memory>
#include <vector>
#include <limits>
#include <thread>
#include <string>

namespace dunedaq {
namespace hello {

class NameReceiver : public dunedaq::appfwk::DAQModule
{
public:
  explicit NameReceiver(const std::string& name);

  void init(const nlohmann::json& obj) override;

  void get_info(opmonlib::InfoCollector&, int /*level*/) override;

  NameReceiver(const NameReceiver&) = delete;
  NameReceiver& operator=(const NameReceiver&) = delete;
  NameReceiver(NameReceiver&&) = delete;
  NameReceiver& operator=(NameReceiver&&) = delete;

  ~NameReceiver() = default;

private:
//Commands
  void do_conf(const nlohmann::json& obj);
  void do_start(const nlohmann::json& obj);
  void do_stop(const nlohmann::json& obj);
  void do_scrap(const nlohmann::json& obj);


//Threading
dunedaq::utilities::WorkerThread thread_;
void do_work(std::atomic<bool>&);


//Configuration
std::string nameParameter;
int waitBetweenSendsMsec;
using sink_t = dunedaq::iomanager::SenderConcept<String>;
std::shared_ptr<sink_t> outputNameQueue_;
std::chrono::milliseconds queueTimeout_;
namereceiver::Conf cfg_; //invalid use of incomplete type 'class dunedaq::hello::NameReceiver'

// Statistic counters
std::atomic<int64_t> receivedCount {0};
std::atomic<int>     sentCount {0};

};

} // namespace hello
ERS_DECLARE_ISSUE_BASE(hello,
 NoOutputQueuesAvailableWarning,
 appfwk::GeneralDAQModuleIssue,
 "No output queues were available, so the configuration parameter will be dropped. Has initialization been successfully completed?",
 ((std::string)name),
 ERS_EMPTY)

} // namespace dunedaq
#endif // HELLO_PLUGINS_NAMERECEIVER_HPP_

