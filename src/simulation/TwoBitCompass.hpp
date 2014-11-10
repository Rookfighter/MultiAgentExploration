#ifndef MAE_TWO_BIT_COMPASS_HPP
#define MAE_TWO_BIT_COMPASS_HPP

#include <stage.hh>

namespace mae
{
	enum CardinalDirection { EAST=0, NORTH, WEST, SOUTH };
	
	class TwoBitCompass
	{
	private:
		Stg::Model *model_;
	public:
		TwoBitCompass();
		~TwoBitCompass();
		
		void connect(Stg::Model *p_model);
		void disconnect();
		
		CardinalDirection getFacingDirection() const;
	};

}

#endif
