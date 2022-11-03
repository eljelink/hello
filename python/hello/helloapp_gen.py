# This module facilitates the generation of hello DAQModules within hello apps


# Set moo schema search path                                                                              
from dunedaq.env import get_moo_model_path
import moo.io
moo.io.default_load_path = get_moo_model_path()

# Load configuration types                                                                                
import moo.otypes
moo.otypes.load_types("hello/namereceiver.jsonnet")

import dunedaq.hello.namereceiver as namereceiver

from daqconf.core.app import App, ModuleGraph
from daqconf.core.daqmodule import DAQModule
from daqconf.core.conf_utils import Endpoint, Direction

def get_hello_app(nickname, name_parameter="Name", n_wait_ms=1000, host="localhost"):#num_namereceivers,
    """
    Here the configuration for an entire daq_application instance using DAQModules from hello is generated.
    """

    modules = []
    modules += [DAQModule(name="nr", plugin="NameReceiver", conf=namereceiver.Conf(nameParameter=name_parameter, waitBetweenSendsMsec=n_wait_ms))]
    modules += [DAQModule(name="hty", plugin="HelloToYou")]
    modules += [DAQModule(name="hn", plugin="HelloName")]

    mgraph = ModuleGraph(modules)
    mgraph.connect_modules("nr.output", "hn.input", "conversation_name_parameter", 10)
    mgraph.connect_modules("hn.output", "hty.input", "hello_name", 10)

    hello_app = App(modulegraph = mgraph, host = host, name = nickname)

    return hello_app
