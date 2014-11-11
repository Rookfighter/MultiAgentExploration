#ifndef MAE_COMPASS_STATE_HPP
#define MAE_COMPASS_STATE_HPP

namespace mae
{
	class CompassStateProperties
	{

	};

	class CompassState
	{
	protected:
		CompassStateProperties properties_;
	public:
		CompassState(const &CompassStateProperties p_properties)
			:properties_(p_properties)
		{ }
		
		virtual ~CompassState() { }

		virtual void update() = 0;

	};

}

#endif
