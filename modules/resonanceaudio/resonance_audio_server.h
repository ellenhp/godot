#ifndef RESONANCE_AUDIO_SERVER_H
#define RESONANCE_AUDIO_SERVER_H

#include "core/reference.h"
#include "resonance_audio/api/resonance_audio_api.h"
#include "core/os/os.h"

class ResonanceAudioServer : public Reference {
    GDCLASS(ResonanceAudioServer, Reference);

public:
    typedef void (*ResonanceCallback)(void *p_userdata);
	struct CallbackItem {
		ResonanceCallback callback;
		void *userdata;

		bool operator<(const CallbackItem &p_item) const {
			return (callback == p_item.callback ? userdata < p_item.userdata : callback < p_item.callback);
		}
	};

    static ResonanceAudioServer* get_singleton();
    vraudio::ResonanceAudioApi* get_api();

    void add_source_callback(ResonanceCallback callback, void* userdata);
    void remove_source_callback(ResonanceCallback callback, void* userdata);

    void notify_samples_needed();

private:
    ResonanceAudioServer();

    static ResonanceAudioServer* singleton;
    vraudio::ResonanceAudioApi* resonance_api;

    Set<CallbackItem> callbacks;

    Mutex *api_lock;
};

#endif // RESONANCE_AUDIO_SERVER_H