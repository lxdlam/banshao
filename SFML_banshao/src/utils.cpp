#include "utils.h"

INITIALIZE_EASYLOGGINGPP

namespace utils
{
	void initLogging()
	{
		el::Configurations conf;
		conf.setGlobally(el::ConfigurationType::Filename, "log.txt");
		conf.setRemainingToDefault();
		conf.setGlobally(el::ConfigurationType::Format, "%datetime{%Y-%M-%d %H:%m:%s} %levshort: %msg");
		el::Loggers::reconfigureLogger("default", conf);
	}
}