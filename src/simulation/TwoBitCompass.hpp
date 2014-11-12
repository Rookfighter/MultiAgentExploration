#ifndef MAE_TWO_BIT_COMPASS_HPP
#define MAE_TWO_BIT_COMPASS_HPP

#include <stage.hh>

namespace mae
{
	enum CardinalDirection { EAST=0, NORTH, WEST, SOUTH };
	
    int getDirectionDiff(const CardinalDirection p_dir1, const CardinalDirection p_dir2);
    CardinalDirection getOppositeDirection(const CardinalDirection p_direction);
    std::string getDirectionStr(const CardinalDirection p_direction);
    
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
