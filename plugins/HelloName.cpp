/**
 * @file HelloName.cpp
 *
 * Implementations of HelloName's functions
 *
 * This is part of the DUNE DAQ Software Suite, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "HelloName.hpp"

#include "StringWrapper.hpp" //because I need to serialize data sent between modules
#include "ers/Issue.hpp"
#include "appfwk/DAQModuleHelper.hpp" //I need that for iomanager definition
#include "iomanager/IOManager.hpp" //because I want to send and receive data
#include "logging/Logging.hpp"

#include "CommonIssues.hpp"
#include "hello/hellonameinfo/InfoNljs.hpp"

#include <string>
#include <thread>
#include <chrono>

//for logging
#define TRACE_NAME "HelloName" //NOLINT
#define TLVL_ENTER_EXIT_METHODS 10
#define TLVL_GREETING 15

namespace dunedaq {
namespace hello {

HelloName::HelloName(const std::string& name)
  : dunedaq::appfwk::DAQModule(name)
  , thread_(std::bind(&HelloName::do_work, this, std::placeholders::_1))
  , inputQueue_(nullptr)
  , outputQueue_(nullptr)
  , queueTimeout_(100)
{
  register_command("start", &HelloName::do_start);
  register_command("stop", &HelloName::do_stop);
}

void
HelloName::init(const nlohmann::json& initobj)
{
  TLOG_DEBUG(TLVL_ENTER_EXIT_METHODS) << get_name() << ": Entering init() method";

  auto qi = appfwk::connection_index(initobj, { "input", "output" });
  try {
    outputQueue_ = get_iom_sender<String>(qi["output"]);
  } catch (const ers::Issue& excpt) {
    throw InvalidQueueFatalError(ERS_HERE, get_name(), "output", excpt);
  }
  try {
    inputQueue_ = get_iom_receiver<String>(qi["input"]);
  } catch (const ers::Issue& excpt) {
    throw InvalidQueueFatalError(ERS_HERE, get_name(), "input", excpt);
  }
 TLOG_DEBUG(TLVL_ENTER_EXIT_METHODS) << get_name() << ": Exiting init() method";
}

void
HelloName::do_start(const nlohmann::json& /*startobj*/)
{
  TLOG_DEBUG(TLVL_ENTER_EXIT_METHODS) << get_name() << ": Entering do_start() method";
  thread_.start_working_thread();
  TLOG() << get_name() << " successfully started";
  TLOG_DEBUG(TLVL_ENTER_EXIT_METHODS) << get_name() << ": Exiting do_start() method";
}

void
HelloName::do_stop(const nlohmann::json& /*stopobj*/)
{
  TLOG_DEBUG(TLVL_ENTER_EXIT_METHODS) << get_name() << ": Entering do_stop() method";
  thread_.stop_working_thread();
  TLOG() << get_name() << " successfully stopped";
  TLOG_DEBUG(TLVL_ENTER_EXIT_METHODS) << get_name() << ": Exiting do_stop() method";
}

void
HelloName::do_work(std::atomic<bool>& running_flag)
{
  TLOG_DEBUG(TLVL_ENTER_EXIT_METHODS) << get_name() << ": Entering do_work() method";
  int receivedCount = 0;
  int sentCount = 0;
  std::string workingString;

  while (running_flag.load()) {
     bool nameWasSuccessfullyReceived = false;
    while (!nameWasSuccessfullyReceived && running_flag.load()) {
      TLOG_DEBUG(TLVL_GREETING) << get_name() << ": Going to receive data from the greeting queue";
      try {
        workingString = inputQueue_->receive(queueTimeout_).conversation;
        nameWasSuccessfullyReceived = true;
        ++receivedCount;
      } catch (const dunedaq::iomanager::TimeoutExpired& excpt) {
       continue;
      }
    }

    TLOG_DEBUG(TLVL_GREETING) << get_name() << ": Received name parameter"  << workingString << ". Using it in the greeting sentence";
    std::string greetingString;
    greetingString = "Hello " + workingString + "!";

std::ostringstream oss_prog;
oss_prog << "Greeting sentence: " << greetingString;
ers::debug(ProgressUpdate(ERS_HERE, get_name(), oss_prog.str()));
    bool successfullyWasSent = false;
    while (!successfullyWasSent && running_flag.load()) {
      TLOG_DEBUG(TLVL_GREETING) << get_name() << ": Pushing the greeting onto the output queue";
      try {
        String wrapped(workingString);
        outputQueue_->send(std::move(wrapped), queueTimeout_);
        successfullyWasSent = true;
        ++sentCount;
      } catch (const dunedaq::iomanager::TimeoutExpired& excpt) {
         throw InvalidQueueFatalError(ERS_HERE, get_name(), "output", excpt);
        std::ostringstream oss_warn;
        oss_warn << "Push to output queue \"" << outputQueue_->get_name() << "\"";
        ers::warning(dunedaq::iomanager::TimeoutExpired(ERS_HERE, get_name(), oss_warn.str(),
        std::chrono::duration_cast<std::chrono::milliseconds>(queueTimeout_).count()));
     }
    }
    TLOG_DEBUG(TLVL_GREETING) << get_name() << ": End of do_work loop";
  }

std::ostringstream oss_sum;
oss_sum << ": Exiting do_work() method, received " << receivedCount << " names and successfully sent " << sentCount << " greeting sentences.";
ers::info(ProgressUpdate(ERS_HERE, get_name(), oss_sum.str()));
TLOG_DEBUG(TLVL_ENTER_EXIT_METHODS) << get_name() << ": Exiting do_work() method";
}

void
HelloName::get_info(opmonlib::InfoCollector& ci, int /* level */)
{
  hellonameinfo::Info info;
  info.parameter_names = receivedCount;
  info.greeting_sentences = sentCount.exchange(0);

  ci.add(info);
}

}// namespace hello
}//namespace dunedaq
DEFINE_DUNE_DAQ_MODULE(dunedaq::hello::HelloName)
