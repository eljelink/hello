/**
 * @file NameReceiver.cpp
 *
 * Implementations of NameReceiver's functions
 *
 * This is part of the DUNE DAQ Software Suite, copyright 2020.
 * Licensing/copyright details are in the COPYING file that you should have
 * received with this code.
 */

#include "NameReceiver.hpp"

#include "hello/namereceiver/Nljs.hpp"
#include "hello/namereceiverinfo/InfoNljs.hpp"
#include "ers/ers.hpp"

#include "appfwk/DAQModuleHelper.hpp"
#include "appfwk/app/Nljs.hpp"
#include "iomanager/IOManager.hpp"
#include "logging/Logging.hpp"
#include "ers/Issue.hpp"
#include "CommonIssues.hpp"
#include "StringWrapper.hpp"

#include <string>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <set>
#include <vector>

// for logging
#define TRACE_NAME "NameReceiver" // NOLINT
#define TLVL_ENTER_EXIT_METHODS 10
#define TLVL_NAME_CONFIGURATION 15

namespace dunedaq::hello {

NameReceiver::NameReceiver(const std::string& name)
  : dunedaq::appfwk::DAQModule(name)
  , thread_(std::bind(&NameReceiver::do_work, this, std::placeholders::_1))
  , outputNameQueue_()
  , queueTimeout_(100)
{
  register_command("conf", &NameReceiver::do_conf, std::set<std::string>{ "INITIAL" });
  register_command("start", &NameReceiver::do_start, std::set<std::string>{ "CONFIGURED" });
  register_command("stop", &NameReceiver::do_stop, std::set<std::string>{ "TRIGGER_SOURCES_STOPPED"});
  register_command("scrap", &NameReceiver::do_scrap, std::set<std::string>{ "CONFIGURED" });

}

void
NameReceiver::init(const nlohmann::json& iniobj)
{
TLOG_DEBUG(TLVL_ENTER_EXIT_METHODS)  << get_name() << ": Entering init() method";
auto qi = appfwk::connection_index(iniobj, { "output" });
//  iomanager::IOManager iom;
  try {
    outputNameQueue_ = get_iom_sender<String>(qi["output"]);
  } catch (const ers::Issue& excpt) {
    throw InvalidQueueFatalError(ERS_HERE, get_name(), "output", excpt);
  }

//try {
//  outputNameQueue_ = get_iom_sender<iomanager::ConnectionRef const& conn_ref>(qi["output"]);
//} catch (const ers::Issue& excpt) {
//   throw InvalidQueueFatalError(ERS_HERE, get_name(), "output", excpt);
// }
TLOG_DEBUG(TLVL_ENTER_EXIT_METHODS) << get_name() << ": Exiting init() method";
}


void
NameReceiver::do_start(const nlohmann::json& /*args*/)
{
  TLOG_DEBUG(TLVL_ENTER_EXIT_METHODS) << get_name() << ": Entering do_start() method for NameReceiver";
  thread_.start_working_thread(); //in listrev example is thread_ not pthread_t?!!
  TLOG() << get_name() << " successfully started";
  TLOG_DEBUG(TLVL_ENTER_EXIT_METHODS) << get_name() << ": Exiting do_start() method for NameReceiver";
}

void
NameReceiver::do_stop(const nlohmann::json& /*args*/)
{
  TLOG_DEBUG(TLVL_ENTER_EXIT_METHODS) << get_name() << ": Entering do_stop() method";
  thread_.stop_working_thread();
  TLOG() << get_name() << " successfully stopped";
  TLOG_DEBUG(TLVL_ENTER_EXIT_METHODS) << get_name() << ": Exiting do_stop() method";
}

void
NameReceiver::do_scrap(const nlohmann::json& /*args*/)
{
  TLOG_DEBUG(TLVL_ENTER_EXIT_METHODS) << get_name() << ": Entering do_unconfigure() method";
  cfg_ = namereceiver::Conf{}; // reset to defaults
  TLOG_DEBUG(TLVL_ENTER_EXIT_METHODS) << get_name() << ": Exiting do_unconfigure() method";
}


void
NameReceiver::do_conf(const nlohmann::json& obj)
{
//  auto conf_as_cpp = obj.get<namereceiver::Conf>();
cfg_ = obj.get<namereceiver::Conf>();
nameParameter = cfg_.nameParameter;
waitBetweenSendsMsec = cfg_.waitBetweenSendsMsec;
}

void
NameReceiver::do_work(std::atomic<bool>& running_flag)
{
  TLOG_DEBUG(TLVL_ENTER_EXIT_METHODS) << get_name() << ": Entering do_work() method";
  size_t sentCount = 0;
  int receivedCount = 0;
  int m_generated = 0;

  while (running_flag.load()) {
    TLOG_DEBUG(TLVL_NAME_CONFIGURATION) << get_name() << ": Creating name parameter " << cfg_.nameParameter;
    std::string confName(cfg_.nameParameter);
    ++receivedCount;
    ++m_generated;
    std::ostringstream oss_prog;
    oss_prog << "Received name configuration parameter #" << receivedCount << " with contents " << confName;
    ers::debug(ProgressUpdate(ERS_HERE, "NameReceiver", oss_prog.str()));

    TLOG_DEBUG(TLVL_NAME_CONFIGURATION) << get_name() << ": Pushing name parameter onto " << outputNameQueue_ << " outputQueues";

      std::string thisQueueName = outputNameQueue_->get_name();
      bool successfullyWasSent = false;
      while (!successfullyWasSent && running_flag.load()) {
        TLOG_DEBUG(TLVL_NAME_CONFIGURATION) << get_name() << ": Pushing the name parameter onto queue " << thisQueueName;
        try {
          String wrapped(confName);
          outputNameQueue_->send(std::move(wrapped), queueTimeout_);
          successfullyWasSent = true;
          ++sentCount;
        } catch (const dunedaq::iomanager::TimeoutExpired& excpt) {
          std::ostringstream oss_warn;
          oss_warn << "push to output queue \"" << thisQueueName << "\"";
          ers::warning(dunedaq::iomanager::TimeoutExpired(
            ERS_HERE,
            "NameReceiver",
            oss_warn.str(),
            std::chrono::duration_cast<std::chrono::milliseconds>(queueTimeout_).count()));
        }
      }
//    if (outputNameQueue_.size() == 0) {
//      ers::warning(dunedaq::hello::NoOutputQueuesAvailableWarning(ERS_HERE, "NameReceiver"));
//    }

    TLOG_DEBUG(TLVL_NAME_CONFIGURATION) << get_name() << ": Start of sleep between sends";
    std::this_thread::sleep_for(std::chrono::milliseconds(cfg_.waitBetweenSendsMsec));//cgf_.waitBetweenSendsMsec
    TLOG_DEBUG(TLVL_NAME_CONFIGURATION) << get_name() << ": End of do_work loop";
  }

  std::ostringstream oss_summ;
  oss_summ << ": Exiting the do_work() method, received " << receivedCount << " name parameters and successfully sent "
           << sentCount << " name parameters. ";//receivedCount.load() was in the listrev example
  ers::info(ProgressUpdate(ERS_HERE, "NameReceiver", oss_summ.str()));

  TLOG_DEBUG(TLVL_ENTER_EXIT_METHODS) << get_name() << ": Exiting do_work() method";
}

void
NameReceiver::get_info(opmonlib::InfoCollector& ci, int /* level */)
{
  namereceiverinfo::Info info;
  info.parameter_names = receivedCount;
  info.new_parameter_names = sentCount.exchange(0);

  ci.add(info);
}


} // namespace dunedaq::hello

DEFINE_DUNE_DAQ_MODULE(dunedaq::hello::NameReceiver)
