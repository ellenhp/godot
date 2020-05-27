#include "resonance_audio_server.h"
#include "servers/audio_server.h"
#include <string>

ResonanceAudioServer* ResonanceAudioServer::singleton = nullptr;

ResonanceAudioServer::ResonanceAudioServer() {
	resonance_api = vraudio::CreateResonanceAudioApi(
		/* num_channels= */ 2, AudioServer::get_singleton()->thread_get_mix_buffer_size(), AudioServer::get_singleton()->get_mix_rate());
	server_mutex = Mutex::create();
	singleton = this;
}

ResonanceAudioServer* ResonanceAudioServer::get_singleton() {
    return singleton;
}
vraudio::ResonanceAudioApi* ResonanceAudioServer::get_api() {
    return resonance_api;
}
Error ResonanceAudioServer::init() {
	return OK;
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

void ResonanceAudioServer::lock() {
	server_mutex->lock();
}
void ResonanceAudioServer::unlock() {
	server_mutex->lock();
}