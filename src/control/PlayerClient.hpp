#ifndef MAE_PLAYER_CLIENT_HPP
#define MAE_PLAYER_CLIENT_HPP

#include <libplayerc++/playerc++.h>
#include <string>

namespace mae
{

	class PlayerClient
	{
	private:
		PlayerCc::PlayerClient client_;
	public:
		PlayerClient(const std::string p_host="localhost", const int p_port=6665);
		~PlayerClient();
		
		
		PlayerCc::PlayerClient* getClient();
		void update();
	};

}

#endif
