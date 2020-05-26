#ifndef RESONANCE_H
#define RESONANCE_H

#include "scene/3d/spatial.h"
#include "servers/audio/audio_stream.h"

class ResonanceAudioSource : public SpatialGizmo {
    GDCLASS(ResonanceAudioSource, SpatialGizmo);

protected:
    static void _bind_methods();

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

	virtual void create();
	virtual void transform();
	virtual void clear();
	virtual void redraw();
	virtual void free();


};

#endif // RESONANCE_H