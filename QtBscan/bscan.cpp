#include <iostream>
#include <vector>

#include <boost/program_options.hpp>
#include <QtConfig.h>
#include <ArgvParams.h>

#include <QApplication>
#include <QMainWindow>
#include <QPrintDialog>
#include <QPrinter>

#include "BscanMainWindow.h"
#include "DDSSubscriber.h"
#include "QtProductReader.h"

std::string _ORB;                ///< path to the ORB configuration file.
std::string _DCPS;               ///< path to the DCPS configuration file.
std::string _DCPSInfoRepo;       ///< URL to access DCPSInfoRepo
std::string _prodTopic;          ///< topic for incoming products
int _DCPSDebugLevel=0;           ///< the DCPSDebugLevel
int _DCPSTransportDebugLevel=0;  ///< the DCPSTransportDebugLevel

namespace po = boost::program_options;

//////////////////////////////////////////////////////////////////////
///
/// get parameters that are specified in the configuration file.
/// These can be overriden by command line specifications.
void getConfigParams()
{

    QtConfig config("Bscan", "Bscan");

    // set up the default configuration directory path
    std::string HcrDir;
    char* e = getenv("HCRDIR");
    if (e) {
        HcrDir = std::string(e) + "/conf/";
    } else {
        std::cerr << "Environment variable HCRDIR must be set." << std::endl;
        exit(1);
    }

    // and create the default DDS configuration file paths, since these
    // depend upon HCRDIR
    std::string orbFile      = HcrDir + "ORBSvc.conf";
    std::string dcpsFile     = HcrDir + "DDSClient.ini";
    std::string dcpsInfoRepo = "iiop://localhost:50000/DCPSInfoRepo";

    // get parameters
    _ORB          = config.getString("DDS/ORBConfigFile",  orbFile);
    _DCPS         = config.getString("DDS/DCPSConfigFile", dcpsFile);
    _prodTopic    = config.getString("DDS/TopicProduct",   "HCRPROD");
    _DCPSInfoRepo = config.getString("DDS/DCPSInfoRepo",   dcpsInfoRepo);
}

//////////////////////////////////////////////////////////////////////
//
/// Parse the command line options, and also set some options
/// that are not specified on the command line.
/// @return The runtime options that can be passed to the
/// threads that interact with the RR314.
void parseOptions(int argc,
        char** argv)
{

    // get the options
    po::options_description descripts("Options");
    descripts.add_options()
        ("help", "describe options")
        ("ORB", po::value<std::string>(&_ORB), "ORB service configuration file (Corba ORBSvcConf arg)")
        ("DCPS", po::value<std::string>(&_DCPS), "DCPS configuration file (OpenDDS DCPSConfigFile arg)")
        ("DCPSInfoRepo", po::value<std::string>(&_DCPSInfoRepo), "DCPSInfoRepo URL (OpenDDS DCPSInfoRepo arg)")
        ("DCPSDebugLevel", po::value<int>(&_DCPSDebugLevel), "DCPSDebugLevel ")
        ("DCPSTransportDebugLevel", po::value<int>(&_DCPSTransportDebugLevel), "DCPSTransportDebugLevel ")
        ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, descripts), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << descripts << std::endl;
        exit(1);
    }
}



int main(int argc, char *argv[]) {

    // get the configuration parameters from the configuration file
    getConfigParams();

    // parse the command line options, substituting for config params.
    parseOptions(argc, argv);

    ArgvParams newargv("bscan");
    newargv["-ORBSvcConf"] = _ORB;
    newargv["-DCPSConfigFile"] = _DCPS;
    newargv["-DCPSInfoRepo"] = _DCPSInfoRepo;
    
    std::cout << "ORB service configuration: " << _ORB << std::endl;
    std::cout << "DCPS configuration: " << _DCPS << std::endl;
    std::cout << "DCPSInfoRepo: " << _DCPSInfoRepo << std::endl;
    if (_DCPSDebugLevel > 0) {
    	std::cout << "setting DCPSDebugLevel to " << _DCPSDebugLevel << std::endl;
    	std::ostringstream oss;
    	oss << _DCPSDebugLevel;
        newargv["-DCPSDebugLevel"] = oss.str();
    }
    if (_DCPSTransportDebugLevel > 0) {
    	std::cout << "setting DCPSTransportDebugLevel to " << 
    		_DCPSTransportDebugLevel << std::endl;
    	std::ostringstream oss;
    	oss << _DCPSTransportDebugLevel;
        newargv["-DCPSTransportDebugLevel"] = oss.str();
    }

    QApplication* app = new QApplication(argc, argv);

    // create the DDS reader for incoming product data
    DDSSubscriber subscriber(newargv.argc(), newargv.argv());
    if (subscriber.status()) {
        std::cerr << "Unable to create a subscriber, exiting." << std::endl;
        exit(1);
    }
    QtProductReader *productReader = new QtProductReader(subscriber, _prodTopic);
    
    QMainWindow* mainWindow = new BscanMainWindow(productReader);
    mainWindow->show();
    
    return app->exec();
}
