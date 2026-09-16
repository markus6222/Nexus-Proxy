#include "events.h"
#include "dialog.h"
#include "gt.hpp"
#include "proton/hash.hpp"
#include "proton/rtparam.hpp"
#include "proton/variant.hpp"
#include "server.h"
#include "utils.h"
#include <thread>
#include <limits.h>
namespace host {
	//player 1 location
	std::string pos1x = "";
	std::string pos1y = "";
	//player 2 location
	std::string pos2x = "";
	std::string pos2y = "";
	//player 1 display location
	std::string posd1x = "";
	std::string posd1y = "";
	//player 2 display location
	std::string posd2x = "";
	std::string posd2y = "";
	//host locations
	std::string startlocx = "";
	std::string startlocy = "";
}