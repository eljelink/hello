This package is an educational example of DUNEDAQ package. It has 3 modules, which communicate with each other in order to create a simple conversation. These 3 simple DAQ modules operate with a string parameter. 
In order to run it, setup the runtime environment for the DAQ version you are using. 
To generate valid configuration file you can issue the following command: hello_gen --name Jane hello_conf
The -h option will show you the available configuration options. 
A hello_conf dorectory will be created in your working directory.
To run issue write the following command: nanorc hello_conf
You can now boot the application and send run control commands to the application 
It will be possible to monitor the output of the application in the log file (created in the working directory).