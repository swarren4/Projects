#pragma once
#include "Channel.h"

namespace oxygine
{
	class SoundSystemEmscripten;

	class ChannelEmscripten : public Channel
	{
	public:
		ChannelEmscripten();
		~ChannelEmscripten();

		void play(const sound_desc &);
		void continuePlay(const sound_desc &);
		void pause();
		void resume();
		void stop();
		bool update();

		Sound*	getSound() const;
		float				getVolume() const;
		const sound_desc&	getSoundDesc() const;
		int					getPosition() const;

		void setVolume(float v);

		bool isFree() const;
		void _updateMasterVolume(float){}

	protected:
		friend class SoundSystemEmscripten;
		void init(SoundSystemEmscripten *, int index);

	private:
		SoundSystemEmscripten *_soundSystem;

		sound_desc _desc;
		int _handle;
		int _index;
	};
}