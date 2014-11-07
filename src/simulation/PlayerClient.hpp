#ifndef MAE_PLAYER_CLIENT_HPP
#define MAE_PLAYER_CLIENT_HPP

#include <stage.hh>
#include <string>

namespace mae
{

	class PlayerClient
	{
	private:
		Stg::World *world_;
	public:
		PlayerClient(Stg::World *world_);
		~PlayerClient();
		
		
		Stg::World* getClient();
		void update();
	};

}

#endif
