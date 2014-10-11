#include "control/PlayerClient.hpp"

namespace mae
{

	PlayerClient::PlayerClient(const std::string p_host, const int p_port)
	:client_(p_host.c_str(), p_port)
	{
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
		client_.Read();
	}

}
