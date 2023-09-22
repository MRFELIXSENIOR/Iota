#pragma once

#include <SDL_mixer.h>
#include <string>

namespace iota {
	inline unsigned int ToMillisecond(double sec) {
		return (unsigned int)(sec * 1000);
	}

	enum SoundGroupTag {
		SFX = 1,
		MUSIC = 2,
		AMBIENT = 4,
		UI_SFX = 3,
	};

	class Music {
	public:
		Music(const std::string& mp3_path);
		~Music();
		
		/*
		100: Maximum Volume
		0: Silence
		*/
		void SetVolume(int volume);
		void SetPosition(double sec);
		void SetPosition(unsigned int ms);

		double GetPosition() const;
		double GetLength() const;

		bool IsPaused() const;
		bool IsPlaying() const;

		void Stop() const;
		void Resume() const;
		void Pause() const;

		void Play() const;

		void PlayFadeIn(unsigned int ms) const;
		void PlayFadeIn(double sec) const;

		void FadeOut(unsigned int ms) const;
		void FadeOut(double sec) const;

		Mix_Music* GetDataPointer() const;
	private:
		Mix_Music* music;
	};

	class SoundEffect {
	public:
		SoundEffect(const std::string& wav_path);
		~SoundEffect();
		
		/*
		100: Maximum Volume
		0: Silence
		*/
		void SetVolume(int volume);
		
		/*
		100: Maximum Volume
		0: Silence
		*/
		void SetPanning(int left_vol, int right_vol);
		int GetChannel() const;

		bool IsPaused() const;
		bool IsPlaying() const;

		void Play();

		void PlayFadeIn(unsigned int ms);
		void PlayFadeIn(double sec);

		void FadeOut(unsigned int ms) const;
		void FadeOut(double sec) const;

		void Stop() const;
		void Pause() const;
		void Resume() const;

		static void StopAll();
		static void PauseAll();
		static void ResumeAll();

		Mix_Chunk* GetDataPointer() const;

	private:
		int channel;
		Mix_Chunk* sound;
	};
}