#include <sstream>
#include "control/Marker.hpp"

namespace mae
{
	std::string const boolToString(bool b)
	{
	  return b ? "true" : "false";
	}

	Marker::Marker()
	{
	}

	Marker::~Marker()
	{
	}

	std::string Marker::str()
	{
		std::stringstream ss;
		ss.precision(2);
		ss << "id: " << id << " " << name << ",p=" << pose.str() << ",r=" << range << ",iu=" << boolToString(inUse) << ",v=" << value; 
		
		return ss.str();
	}
}
