#include "IotaSound.hpp"
#include "IotaException.hpp"

namespace iota {
	int ToSDLVolValue(int maximum, int volume) {
		if (volume < 0) {
			volume = 0;
		}
		else if (volume > 100) volume = 100;

		return (maximum * volume / 100);
	}

	Music::Music(const std::string& mp3_path) : music(Mix_LoadMUS(mp3_path.c_str())) {
		if (!music)
			throw RuntimeError("Could not load music, " + std::string(Mix_GetError()));
	}
	Music::~Music() { Mix_FreeMusic(music); }

	Mix_Music* Music::GetDataPointer() const { return music; }

	void Music::SetVolume(int volume) { Mix_VolumeMusic(ToSDLVolValue(MIX_MAX_VOLUME, volume)); }
	void Music::SetPosition(double sec) { Mix_SetMusicPosition(sec); }
	void Music::SetPosition(unsigned int ms) { SetPosition((double)(ms / 1000)); }

	double Music::GetPosition() const { return Mix_GetMusicPosition(music); }
	double Music::GetLength() const { return Mix_MusicDuration(music); }

	void Music::Play() const {
		if (Mix_PlayMusic(music, 0) == -1)
			throw RuntimeError("Could not play music, " + std::string(Mix_GetError()));
	}

	void Music::PlayFadeIn(unsigned int ms) const {
		if (Mix_FadeInMusic(music, 0, ms) == -1)
			throw RuntimeError("Could not play fade in music, " + std::string(Mix_GetError()));
	}
	void Music::PlayFadeIn(double sec) const { PlayFadeIn(ToMillisecond(sec)); }

	void Music::FadeOut(unsigned int ms) const { Mix_FadeOutMusic(ms); }
	void Music::FadeOut(double sec) const { FadeOut(ToMillisecond(sec)); }

	void Music::Stop() const { Mix_HaltMusic(); }
	void Music::Resume() const { Mix_ResumeMusic(); }
	void Music::Pause() const { Mix_PauseMusic(); }

	bool Music::IsPaused() const { return (bool)Mix_PausedMusic(); }
	bool Music::IsPlaying() const { return (bool)Mix_PlayingMusic(); }

	SoundEffect::SoundEffect(const std::string& wav_path) : sound(Mix_LoadWAV(wav_path.c_str())) {
		if (!sound)
			throw RuntimeError("Could not load sound effect, " + std::string(Mix_GetError()));

		channel = Mix_GroupAvailable(SFX);
		if (channel == -1)
			throw RuntimeError("There is no channel available for sound effect to play");
	}
	SoundEffect::~SoundEffect() { Mix_FreeChunk(sound); }

	Mix_Chunk* SoundEffect::GetDataPointer() const { return sound; }

	void SoundEffect::SetVolume(int volume) {
		Mix_VolumeChunk(sound, ToSDLVolValue(MIX_MAX_VOLUME, volume));
	}
	void SoundEffect::SetPanning(int left_vol, int right_vol) { Mix_SetPanning(channel, ToSDLVolValue(255U, left_vol), ToSDLVolValue(255U, right_vol)); }

	int SoundEffect::GetChannel() const { return channel; }

	bool SoundEffect::IsPaused() const { return (bool)Mix_Paused(channel); }
	bool SoundEffect::IsPlaying() const { return (bool)Mix_Playing(channel); }

	void SoundEffect::Play() {
		channel = Mix_PlayChannel(channel, sound, 0);
		if (channel == -1)
			throw RuntimeError("Could not play sound effect, " + std::string(Mix_GetError()));
	}

	void SoundEffect::PlayFadeIn(unsigned int ms) {
		channel = Mix_FadeInChannel(channel, sound, 0, ms);
		if (channel == -1)
			throw RuntimeError("Could not play sound effect fade in, " + std::string(Mix_GetError()));
	}

	void SoundEffect::PlayFadeIn(double sec) {
		PlayFadeIn(ToMillisecond(sec));
	}

	void SoundEffect::FadeOut(unsigned int ms) const {
		int result = Mix_FadeOutChannel(channel, ms);
		if (result == -1)
			throw RuntimeError("Could not play sound effect fade out, " + std::string(Mix_GetError()));
	}

	void SoundEffect::FadeOut(double sec) const {
		FadeOut(ToMillisecond(sec));
	}

	void SoundEffect::Stop() const { Mix_HaltChannel(channel); }
	void SoundEffect::StopAll() { Mix_HaltChannel(-1); }
	void SoundEffect::Pause() const { Mix_Pause(channel); }
	void SoundEffect::PauseAll() { Mix_Pause(-1); }
	void SoundEffect::Resume() const { Mix_Resume(channel); }
	void SoundEffect::ResumeAll() { Mix_Resume(-1); }
}
