#ifndef RESONANCE_AUDIO_LISTENER_H
#define RESONANCE_AUDIO_LISTENER_H

#include "scene/3d/spatial.h"
#include "resonance_audio/api/resonance_audio_api.h"
#include "core/math/audio_frame.h"

class ResonanceAudioListener : public Spatial {
    GDCLASS(ResonanceAudioListener, Spatial);

protected:
    static void _bind_methods();
	void _notification(int p_what);

public:

private:
	void _mix_audio();
	static void _mix_audio_cb(void *self) { reinterpret_cast<ResonanceAudioListener *>(self)->_mix_audio(); }

    bool active;
	Vector<AudioFrame> mix_buffer;

};

#endif // RESONANCE_AUDIO_LISTENER_H