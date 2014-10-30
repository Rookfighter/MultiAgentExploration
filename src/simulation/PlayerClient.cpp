#include "simulation/PlayerClient.hpp"
#include <easylogging++.h>

namespace mae
{

	PlayerClient::PlayerClient(const std::string p_host, const int p_port)
	:client_(p_host.c_str(), p_port)
	{
		LOG(DEBUG) << "Connected PlayerClient: " << p_host << ":" << p_port;
	}

	PlayerClient::~PlayerClient()
	{
	}

	PlayerCc::PlayerClient* PlayerClient::getClient()
	{
		return &client_;
	}
	
	void PlayerClient::update()
	{
		client_.ReadIfWaiting();
	}

}
