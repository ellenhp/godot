#include "resonance_audio_server.h"
#include "servers/audio_server.h"
#include <string>

// static Mutex* singleton_lock = Mutex::create();

ResonanceAudioServer* ResonanceAudioServer::singleton = nullptr;

ResonanceAudioServer::ResonanceAudioServer() : resonance_api(nullptr) {
	api_lock = Mutex::create();
}

ResonanceAudioServer* ResonanceAudioServer::get_singleton() {
	// singleton_lock->lock();
	if (singleton == nullptr) {
		singleton = new ResonanceAudioServer();
	}
	// singleton_lock->unlock();
    return singleton;
}
vraudio::ResonanceAudioApi* ResonanceAudioServer::get_api() {
	api_lock->lock();
    if (resonance_api == nullptr) {
        ERR_PRINT(std::to_string(AudioServer::get_singleton()->thread_get_mix_buffer_size()).c_str());
        ERR_PRINT(std::to_string(AudioServer::get_singleton()->get_mix_rate()).c_str());
        resonance_api = vraudio::CreateResonanceAudioApi(
            /* num_channels= */ 2, AudioServer::get_singleton()->thread_get_mix_buffer_size(), AudioServer::get_singleton()->get_mix_rate());
    }
	api_lock->unlock();
    return resonance_api;
}

void ResonanceAudioServer::add_source_callback(ResonanceCallback p_callback, void *p_userdata) {
	// lock();
	CallbackItem ci;
	ci.callback = p_callback;
	ci.userdata = p_userdata;
	callbacks.insert(ci);
	// unlock();
}

void ResonanceAudioServer::remove_source_callback(ResonanceCallback p_callback, void *p_userdata) {
	// lock();
	CallbackItem ci;
	ci.callback = p_callback;
	ci.userdata = p_userdata;
	callbacks.erase(ci);
	// unlock();
}

void ResonanceAudioServer::notify_samples_needed() {
	for (Set<CallbackItem>::Element *E = callbacks.front(); E; E = E->next()) {
		E->get().callback(E->get().userdata);
    }
}