#pragma once
class AudioManager
{
public:
	AudioManager();
	~AudioManager() = default;

public:
	float GetBGMVolume() { return BGMVolume_; }

	void SetBGMVolume(float volume) { BGMVolume_ = volume; }

	float GetSEVolume() { return SEVolume_; }

	void SetSEVolume(float volume) { SEVolume_ = volume; }

private:
	float BGMVolume_;
	float SEVolume_;

};

