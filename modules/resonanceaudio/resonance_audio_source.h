#ifndef RESONANCE_AUDIO_SOURCE_H
#define RESONANCE_AUDIO_SOURCE_H

#include "scene/3d/spatial.h"
#include "servers/audio/audio_stream.h"
#include "core/math/audio_frame.h"

class ResonanceAudioSource : public Spatial {
    GDCLASS(ResonanceAudioSource, Spatial);

protected:
    static void _bind_methods();
	void _notification(int p_what);

public:
	ResonanceAudioSource();

	void set_stream(Ref<AudioStream> p_stream);
	Ref<AudioStream> get_stream() const;

	void play(float p_from_pos = 0.0);
	void seek(float p_seconds);
	void stop();
	bool is_playing() const;
	float get_playback_position();

    void set_autoplay(bool p_enable);
	bool is_autoplay_enabled();

	void set_stream_paused(bool p_pause);
	bool get_stream_paused() const;


private:
	void _get_samples();
	static void _get_samples_cb(void *self) { reinterpret_cast<ResonanceAudioSource *>(self)->_get_samples(); }

	Ref<AudioStream> stream;
	Ref<AudioStreamPlayback> stream_playback;

	Vector<AudioFrame> mix_buffer;

	bool active;
	bool autoplay;

	int source_id;
};

#endif // RESONANCE_AUDIO_SOURCE_H