#ifndef RESONANCE_AUDIO_SERVER_H
#define RESONANCE_AUDIO_SERVER_H

#include "core/reference.h"
#include "resonance_audio/api/resonance_audio_api.h"
#include "core/os/os.h"
#include <thread>
#include <mutex>

class ResonanceAudioServer : public Reference {
    GDCLASS(ResonanceAudioServer, Reference);

public:
    ResonanceAudioServer();

    typedef void (*ResonanceCallback)(void *p_userdata);
	struct CallbackItem {
		ResonanceCallback callback;
		void *userdata;

		bool operator<(const CallbackItem &p_item) const {
			return (callback == p_item.callback ? userdata < p_item.userdata : callback < p_item.callback);
		}
	};
    Error init();

    static ResonanceAudioServer* get_singleton();
    vraudio::ResonanceAudioApi* get_api();

    void add_source_callback(ResonanceCallback callback, void* userdata);
    void remove_source_callback(ResonanceCallback callback, void* userdata);

    void notify_samples_needed();

    void lock();
    void unlock();

private:

    static ResonanceAudioServer* singleton;
    vraudio::ResonanceAudioApi* resonance_api;

    Set<CallbackItem> callbacks;
    Mutex* server_mutex;
};

#endif // RESONANCE_AUDIO_SERVER_H