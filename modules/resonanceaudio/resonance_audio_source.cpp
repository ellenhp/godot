#include "resonance_audio_source.h"
#include "core/engine.h"

ResonanceAudioSource::ResonanceAudioSource() {}

void ResonanceAudioSource::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_stream", "stream"), &ResonanceAudioSource::set_stream);
	ClassDB::bind_method(D_METHOD("get_stream"), &ResonanceAudioSource::get_stream);

	ClassDB::bind_method(D_METHOD("play", "from_position"), &ResonanceAudioSource::play, DEFVAL(0.0));
	ClassDB::bind_method(D_METHOD("seek", "to_position"), &ResonanceAudioSource::seek);
	ClassDB::bind_method(D_METHOD("stop"), &ResonanceAudioSource::stop);

	ClassDB::bind_method(D_METHOD("is_playing"), &ResonanceAudioSource::is_playing);
	ClassDB::bind_method(D_METHOD("get_playback_position"), &ResonanceAudioSource::get_playback_position);

	ClassDB::bind_method(D_METHOD("set_autoplay", "enable"), &ResonanceAudioSource::set_autoplay);
	ClassDB::bind_method(D_METHOD("is_autoplay_enabled"), &ResonanceAudioSource::is_autoplay_enabled);
}

void ResonanceAudioSource::create() {
    
}

void ResonanceAudioSource::transform() {
    
}

void ResonanceAudioSource::clear() {
    
}

void ResonanceAudioSource::redraw() {
    
}

void ResonanceAudioSource::free() {
    
}

void ResonanceAudioSource::set_stream(Ref<AudioStream> p_stream){}
Ref<AudioStream> ResonanceAudioSource::get_stream() const {}

void ResonanceAudioSource::play(float p_from_pos){}
void ResonanceAudioSource::seek(float p_seconds){}
void ResonanceAudioSource::stop(){}
bool ResonanceAudioSource::is_playing() const {}
float ResonanceAudioSource::get_playback_position(){}

void ResonanceAudioSource::set_autoplay(bool p_enable){}
bool ResonanceAudioSource::is_autoplay_enabled(){}

void ResonanceAudioSource::set_stream_paused(bool p_pause){}
bool ResonanceAudioSource::get_stream_paused() const {}