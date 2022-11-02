/**
 * @file HelloToYou.cpp
 *
 * Implementations of HelloToYou's functions
 *
 * This is part of the DUNE DAQ Software Suite, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "HelloToYou.hpp"

//#include "hello/hellotoyou/Nljs.hpp"
//#include "hello/hellotoyouinfo/InfoNljs.hpp"
#include "appfwk/DAQModuleHelper.hpp"
#include "iomanager/IOManager.hpp" //because I am receiving data from the HelloName module
#include "logging/Logging.hpp"
#include "ers/Issue.hpp"
#include "CommonIssues.hpp"

#include <chrono>
#include <functional>
#include <thread>
#include <string>

//for logging
#define TRACE_NAME "HelloToYou" //NOLINT
#define TLVL_ENTER_EXIT_METHODS 10
#define TLVL_ANSWER 15

namespace dunedaq::hello {

HelloToYou::HelloToYou(const std::string& name)
  : dunedaq::appfwk::DAQModule(name)
, thread_(std::bind(&HelloToYou::do_work, this, std::placeholders::_1))
//, greetingDataQueue_(nullptr)
//, queueTimeout_(100)
{
  register_command("start", &HelloToYou::do_start);
  register_command("stop", &HelloToYou::do_stop);
}

void
HelloToYou::init(const nlohmann::json& obj)
{
//TLOG_DEBUG(TLVL_ENTER_EXIT_METHODS) << get_name() << ": Entering init() method";
//  auto qi = appfwk::connection_index(obj, { "greeting_input" });
//  try {
//    greetingDataQueue_ = get_iom_receiver<String>(qi["greeting_input"]);
//  } catch (const ers::Issue& excpt) {
//    throw InvalidQueueFatalError(ERS_HERE, get_name(), "greeting input", excpt);
//  }
//TLOG_DEBUG(TLVL_ENTER_EXIT_METHODS) << get_name() << ": Exiting init() method";
}


void
HelloToYou::do_start(const nlohmann::json& /*args*/)
{
  TLOG_DEBUG(TLVL_ENTER_EXIT_METHODS) << get_name() << ": Entering do_start() method";
  thread_.start_working_thread();
  TLOG() << get_name() << " successfully started";
  TLOG_DEBUG(TLVL_ENTER_EXIT_METHODS) << get_name() << ": Exiting do_start() method";
}

void
HelloToYou::do_stop(const nlohmann::json& /*args*/)
{
  TLOG_DEBUG(TLVL_ENTER_EXIT_METHODS) << get_name() << ": Entering do_stop() method";
  thread_.stop_working_thread();
  TLOG() << get_name() << " successfully stopped";
  TLOG_DEBUG(TLVL_ENTER_EXIT_METHODS) << get_name() << ": Exiting do_stop() method";
}

void
HelloToYou::do_work(std::atomic<bool>& running_flag)
{
  TLOG_DEBUG(TLVL_ENTER_EXIT_METHODS) << get_name() << ": Entering do_work() method";
  std::string greetingData;//definition of variable which holds "Hello Name" greeting sentence
  int receivedCount = 0;
  int answerCount = 0;

  while (running_flag.load()) {
    bool greetingWasSuccessfullyReceived = false;
 //   while (!greetingWasSuccessfullyReceived && running_flag.load()) {
 //     TLOG_DEBUG(TLVL_ANSWER) << get_name() << ": Going to receive data from the greeting queue";
 //     try {
 //       greetingData = greetingDataQueue_->receive(queueTimeout_).conversation;//.String
 //       greetingWasSuccessfullyReceived = true;
 //       ++receivedCount;
 //     } catch (const dunedaq::iomanager::TimeoutExpired& excpt) {
//       continue;
//      }
 //   }
//TLOG_DEBUG(TLVL_ANSWER) << get_name() << ": Received greeting" << greetingData << ". Now going to answer.";

    if (greetingWasSuccessfullyReceived) {
      std::ostringstream oss_prog;
      oss_prog << "Greeting sentence #" << receivedCount << " received: " << greetingData;
      ers::debug(ProgressUpdate(ERS_HERE, get_name(), oss_prog.str()));

      TLOG_DEBUG(TLVL_ANSWER) << get_name() << ": Answering the greeting sentence";
      std::string answerString; 
      answerString = "Hello to you!";
      ++answerCount;
    }
TLOG_DEBUG(TLVL_ANSWER) << get_name() << ": End of do_work loop";
}

  std::string answerString; 
  answerString = "Hello to you!";
  
  std::ostringstream oss_summ;
  oss_summ << ": Exiting do_work() method, received " << receivedCount << " greetings, "
           << "answered" << answerCount << " sentences: " << answerString;
  ers::info(ProgressUpdate(ERS_HERE, get_name(), oss_summ.str()));

   TLOG_DEBUG(TLVL_ENTER_EXIT_METHODS) << get_name() << ": Exiting do_work() method";
}

} // namespace dunedaq::hello

DEFINE_DUNE_DAQ_MODULE(dunedaq::hello::HelloToYou)
