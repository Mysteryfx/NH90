#pragma once

namespace cockpit
{
	class avBaseGyro;

	class GyroSound
	{
	public:
		virtual void create_sounds (Sound::Host* soundHost) = 0;
		virtual void play() = 0;
		virtual ~GyroSound(){}

	protected:
		GyroSound(avBaseGyro& gyro)
			: gyro_(gyro) {}

		avBaseGyro& gyro_;
	};

}