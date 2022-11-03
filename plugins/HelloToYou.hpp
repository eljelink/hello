/**
 * @file HelloToYou.hpp
 *
 * This module receives greeting sentence and responds with a "Hello to you!" answer
 *
 * This is part of the DUNE DAQ Software Suite, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#ifndef HELLO_PLUGINS_HELLOTOYOU_HPP_
#define HELLO_PLUGINS_HELLOTOYOU_HPP_

#include "appfwk/DAQModule.hpp"
#include "iomanager/Receiver.hpp" //because two modules communicate with each other
#include "utilities/WorkerThread.hpp"
#include "ers/Issue.hpp"
#include "StringWrapper.hpp"


#include <atomic>
#include <memory>
#include <limits>
#include <string>

namespace dunedaq::hello {

class HelloToYou : public dunedaq::appfwk::DAQModule
{
public:
  explicit HelloToYou(const std::string& name);

  void init(const data_t&) override;
  void get_info(opmonlib::InfoCollector&, int /*level*/) override;

  HelloToYou(const HelloToYou&) = delete;
  HelloToYou& operator=(const HelloToYou&) = delete;
  HelloToYou(HelloToYou&&) = delete;
  HelloToYou& operator=(HelloToYou&&) = delete;

  ~HelloToYou() = default;

private:
//Commands
void do_start(const nlohmann::json& obj);
void do_stop(const nlohmann::json& obj);

//Threading
dunedaq::utilities::WorkerThread thread_;
void do_work(std::atomic<bool>&);

//Configuration
using source_t=dunedaq::iomanager::ReceiverConcept<String>;
std::shared_ptr<source_t> greetingDataQueue_;
std::chrono::milliseconds queueTimeout_;

// Statistic counters
std::atomic<int64_t> receivedCount {0};
std::atomic<int>     answerCount {0};

};

} // namespace dunedaq::hello

#endif // HELLO_PLUGINS_HELLOTOYOU_HPP_
