#pragma once

#include "WorldSound.h"
#include "Avionics/AttitudeIndicator/GyroSound.h"

namespace cockpit
{
	class AirDrivenGyroSound : public GyroSound
	{ 
	public:
		AirDrivenGyroSound(avBaseGyro& gyro);
		~AirDrivenGyroSound();

		virtual void create_sounds (Sound::Host* soundHost) override;
		virtual void play() override;

	private:

		Sound::Source gyro_1_snd;
		Sound::Source gyro_2_snd;
		Sound::Source gyro_BG_snd;

		bool  spinning_;
		double unfreeze_time_;
		double last_percent_;

		void play_gyro_1();
		void play_gyro_2();
		void play_gyro_bg();

		ed::vector<float> gain_gyro_2_x_v;
		ed::vector<float> gain_gyro_2_y_v;

		ed::vector<float> gain_gyro_1_x_v;
		ed::vector<float> gain_gyro_1_y_v;
		ed::vector<float> gain_gyro_1_stop_x_v;
		ed::vector<float> gain_gyro_1_stop_y_v;

		ed::vector<float> gain_gyro_bg_x_v;
		ed::vector<float> gain_gyro_bg_y_v;
		ed::vector<float> gain_gyro_bg_stop_x_v;
		ed::vector<float> gain_gyro_bg_stop_y_v;
	};
}