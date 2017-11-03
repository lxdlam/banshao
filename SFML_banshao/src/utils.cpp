#include "utils.h"
#include <mutex>
#include <boost/date_time.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/null.hpp>

namespace utils
{
	std::vector<fs::path> findFiles(fs::path p)
	{
		auto pstr = p.u16string();
#ifdef _WIN32
		size_t offset = pstr.find(u"\\*");
#else
		size_t offset = pstr.find(u"/*");
#endif
		std::vector<fs::path> res;
		if (offset == pstr.npos)
		{
			if (!pstr.empty() && pstr.find(u'*') == pstr.npos)
				res.push_back(p);
			return res;
		}
		
		std::u16string dir = pstr.substr(0, offset);
		std::u16string tail = pstr.substr(offset + 2);
		for (auto &f : fs::directory_iterator(dir))
		{
			std::u16string file = f.path().u16string();
			if (file.substr(file.length() - tail.length()) != tail)
				continue;
			res.push_back(f);
		}
		return res;
	}

	std::pair<unsigned, bool> stoub(const std::string& str)
	{
		if (str.empty())
			return { -1, false };

		int val;
		bool not;
		if (str[0] == '!')
		{
			val = std::stoi(str.substr(1));
			not = true;
		}
		else
		{
			val = std::stoi(str);
			not = false;
		}

		if (val >= 0)
			return { val, not };
		else
			return { -1, false };
	}

	void initLogging()
	{
		size_t idx = 0;
		auto timef = new boost::posix_time::time_facet("%H:%M:%S");
		logSink->imbue(std::locale(logSink->getloc(), timef));
	}

	std::ostream& getLogSink(severity_level s)
	{
#if !_DEBUG
		if (s == DEBUG)
			return logNullSink;
#endif
		auto& sink = *logSink;
		sink << std::endl;
		sink << boost::posix_time::second_clock::local_time();
		switch (s)
		{
		case DEBUG:		sink << " D: "; break;
		case INFO:		sink << " I: "; break;
		case WARNING:	sink << " W: "; break;
		case ERROR:		sink << " E: "; break;
		case FATAL:		sink << " F: "; break;
		}
		return sink;
	}
}

std::ostream& LOG(severity_level s)
{
	return utils::getLogSink(s);
}