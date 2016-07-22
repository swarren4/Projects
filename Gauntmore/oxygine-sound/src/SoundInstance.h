#pragma once
#include "core/Object.h"
#include "Channel.h"
#include "EventDispatcher.h"
#include "Event.h"

namespace oxygine
{
	class SoundPlayer;
	class Channel;

	class SoundInstance: public Object
	{
	public:
		class SoundEvent: public Event
		{
		public:
			enum 
			{
				EVENT_ABOUT_DONE = makefourcc('S', 'I', 'A', 'D'),
				EVENT_DONE = makefourcc('S', 'I', 'D', 'N')
			};
			SoundEvent(eventType tp, SoundInstance *ins):Event(tp), instance(ins){}
			SoundInstance *instance;
		};
		SoundInstance();
		~SoundInstance();		

		void stop();
		void fadeOut(int fadeOutMS);

		Channel*	getChannel() const;
		int			getDuration() const;
		int			getPosition() const;
		float		getVolume() const;
		const sound_desc &getDesc() const {return _desc;}
		bool		isPlaying();

		void	setVolume(float v);

		/**called when sound done*/
		void	setDoneCallback(EventCallback cb){_cbDone = cb;}
		void	setAboutDoneCallback(EventCallback cb){_cbAboutDone = cb;}

	protected:
	
		friend class SoundPlayer;
		void finished();
		void update();
		void aboutDone();

	private:
		SoundPlayer *_player;
		EventCallback _cbDone;
		EventCallback _cbAboutDone;

		Channel *_channel;
		sound_desc _desc;

		float _volume;//primary volume
		unsigned int _startTime;

		int _startFadeIn;
		int _startFadeOut;

		int _fadeInMS;
		int _fadeOutMS;

		enum State
		{
			FadingIn,
			FadingOut,
			Normal
		};
		State _state;
	};
}