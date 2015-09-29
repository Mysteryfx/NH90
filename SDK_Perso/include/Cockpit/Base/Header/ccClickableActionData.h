#pragma once

#include "Utilities/avBasicTimer.h"
#include "Utilities/ccUtilities.h"

namespace cockpit
{
    class clickableElementData;

	struct InitInfoCommon
	{
		unsigned char	control_type;
		int				command;
	};

    class clickableElementActionData 
    {
	public:
		enum SoundUnDef
		{
			SOUND_NOSND = -2,
			SOUND_UNDEF = -1, //no sound
		};

        clickableElementActionData(clickableElementData* parent, const InitInfoCommon& init);
       
		unsigned char	control_type;
		int				command_;
		int				after_stop_command;
		bool            repeatable;
		int			    drawingIndex;
		float			drawingIndexValue; /*command delta*/
		limits<float>	drawingIndexValueLimit;
		bool            relative;          
		float           gain;                   //axis only
		bool			use_release_message;
		float			after_stop_value;
		bool			have_stop_value;
		int				sound_id_up;	//positive arg changing sound
		int				sound_id_down;  //negative arg changing sound

		virtual bool set_argument(float& arg_value /*next arg value*/, 
			float& command_value /*command value which will be sent to SetCommand*/, int command = 0); /*returns if command should been called*/
	
		void force_set_argument(float arg_value);

		virtual bool animated() const { return false; }
		bool is_custom_sound() const { return sound_id_up != SOUND_UNDEF || sound_id_down != SOUND_UNDEF; }

	private:
		clickableElementActionData(const clickableElementActionData&){}
		clickableElementActionData& operator=(const clickableElementActionData&){}

	protected:
	
		void play_sound(char  direction);
		virtual float set_argument_internal(float animation_value);

		clickableElementData* parent_;
	};

	class clickableElementActionDataAnimated;

	class clickableElement_Animation : public avBasicTimer<clickableElementActionDataAnimated>
	{ 
	public:
		clickableElement_Animation(clickableElementActionDataAnimated * p);

		virtual void NextEvent();
		virtual bool StartAnimation();
	};

	class clickableElementActionDataAnimated : public clickableElementActionData
	{ 
	public:
		friend class clickableElement_Animation;

		clickableElementActionDataAnimated(clickableElementData* parent,
			const InitInfoCommon& init,
			float animation_speed);

		~clickableElementActionDataAnimated();
		
		virtual bool set_argument(float& arg_value /*argument value which was set*/, 
			float& command_value /*command value which will be sent to SetCommand*/, int command = 0) override; /*returns if command should been called*/

		virtual bool animated() const override { return true; }

	protected:

		void cancel() {if( animation_timer!= nullptr ) animation_timer->Cancel(); }
		bool active() const { if( animation_timer!= nullptr )  return animation_timer->IsActive(); return false;  } 

		bool ready_for_action () const {
			return animation_timer != nullptr ? !animation_timer->IsActive() : true; 
		}

		bool try_start_animation (float command_value /*relative: value_in - anim step; else - next value*/); /*return: returns if animation started*/
		virtual bool start_animation(float command_value);

		bool update(wModelTime dt);

		float				step_;
		float				target_; /*relative animation delta*/
		float				start_;
		int					command_to_send_; /*we need the variable because of case of sending after_stop_command */

		clickableElement_Animation* animation_timer;

		float                       animation_speed_;

	};

	class SlaveData;

	class clickableElementActionDataMaster : public clickableElementActionDataAnimated
	{ 
	public:
		clickableElementActionDataMaster(clickableElementData* parent, 
			const InitInfoCommon& init,
			float animation_speed, 
			Lua::Config& config);

		~clickableElementActionDataMaster();
	protected:
		SlaveData* l_read_slave(Lua::Config& config, limits<float>& master_limit, char level = 0);
		
		virtual float set_argument_internal(float anim_delta = 0.0f) override;
		
		virtual bool start_animation(float command_value);
		
		//slave
		SlaveData* slave_data_;
	};

	/*Slave Data class*/
	class SlaveData
	{
		friend class clickableElementActionDataMaster;

	public:
		SlaveData(int arg, limits<float>& limit, 
			limits<float>& engage_limit, 
			limits<float>& master_limit, 
			float ratio,
			SlaveData* child_data)
			: arg_(arg), draw_limit_(limit), engage_limit_(engage_limit),
			master_limit_(master_limit),
			ratio_(ratio), active_(false),
			child_slave_(child_data)
		{}

		~SlaveData()
		{
			if(child_slave_)
				delete child_slave_;
		}

		void on_start(float target, float step);
		void animate(float anim_step);

		static SlaveData* l_read_slave(Lua::Config& config, limits<float>& master_limit, char level = 0);
	private:
		int					arg_;

		limits<float>	draw_limit_;
		limits<float>	engage_limit_;
		limits<float>	master_limit_;

		float			ratio_;

		SlaveData*			child_slave_;

		bool				active_; /*animation active*/
	};
}
