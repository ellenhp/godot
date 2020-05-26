#include "resonance_audio_source.h"
#include "core/engine.h"
#include "resonance_audio_server.h"
#include "core/print_string.h"
#include "resonance_audio/api/resonance_audio_api.h"
#include <string>

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

    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "stream", PROPERTY_HINT_RESOURCE_TYPE, "AudioStream"), "set_stream", "get_stream");
	// ADD_PROPERTY(PropertyInfo(Variant::BOOL, "playing", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_EDITOR), "_set_playing", "is_playing");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "autoplay"), "set_autoplay", "is_autoplay_enabled");


}

void ResonanceAudioSource::_notification(int p_what) {

	if (p_what == NOTIFICATION_ENTER_TREE) {
        ERR_PRINT("source entering tree\n");
        ResonanceAudioServer::get_singleton()->add_source_callback(_get_samples_cb, this);
        source_id = ResonanceAudioServer::get_singleton()->get_api()->CreateSoundObjectSource(
            vraudio::RenderingMode::kStereoPanning);
        ResonanceAudioServer::get_singleton()->get_api()->SetSourceDistanceModel(
            source_id,
            vraudio::DistanceRolloffModel::kLinear,
            /* min_distance= */ 0,
            /* max_distance= */ 0);
        ResonanceAudioServer::get_singleton()->get_api()->SetSourceVolume(
            source_id, /* volume= */ 1);
		if (autoplay && !Engine::get_singleton()->is_editor_hint()) {
            active = true;
		}

	}

	if (p_what == NOTIFICATION_EXIT_TREE) {
        ResonanceAudioServer::get_singleton()->remove_source_callback(_get_samples_cb, this);
        ResonanceAudioServer::get_singleton()->get_api()->DestroySource(source_id);
        active = false;
    }

	if (p_what == NOTIFICATION_PAUSED) {
		if (!can_process()) {
            active = false;
		}
	}

	if (p_what == NOTIFICATION_UNPAUSED) {
		active = true;
	}

	if (p_what == NOTIFICATION_INTERNAL_PHYSICS_PROCESS) {
        // Update the position of the source.
        Vector3 source_position = get_global_transform().origin;
        Quat source_rotation = Quat(get_global_transform().basis);

        ResonanceAudioServer::get_singleton()->get_api()->SetSourcePosition(
            source_id, source_position.x, source_position.y, source_position.z);
        ResonanceAudioServer::get_singleton()->get_api()->SetSourceRotation(
            source_id, source_rotation.x, source_rotation.y, source_rotation.z, source_rotation.w);
    }
}


void ResonanceAudioSource::set_stream(Ref<AudioStream> p_stream) {
	AudioServer::get_singleton()->lock();

	if (stream_playback.is_valid()) {
		stream_playback.unref();
		stream.unref();
		active = false;
	}

	if (p_stream.is_valid()) {
    	mix_buffer.resize(AudioServer::get_singleton()->thread_get_mix_buffer_size());
		stream = p_stream;
		stream_playback = p_stream->instance_playback();
	}

	static bool is_playing = false;
	if (stream_playback != nullptr && !is_playing && active) {
		stream_playback->start(0.0);
	}

	AudioServer::get_singleton()->unlock();

	if (p_stream.is_valid() && stream_playback.is_null()) {
		stream.unref();
	}
}
Ref<AudioStream> ResonanceAudioSource::get_stream() const {
    return stream;
}

void ResonanceAudioSource::play(float p_from_pos) {
	if (stream_playback.is_valid()) {
		active = true;
		set_physics_process_internal(true);
	}
}
void ResonanceAudioSource::seek(float p_seconds) {}
void ResonanceAudioSource::stop() {
    active = false;
}
bool ResonanceAudioSource::is_playing() const {
    return active;
}
float ResonanceAudioSource::get_playback_position(){
    return 0;
}

void ResonanceAudioSource::set_autoplay(bool p_enable){
    autoplay = p_enable;
}
bool ResonanceAudioSource::is_autoplay_enabled(){
    return autoplay;
}

void ResonanceAudioSource::set_stream_paused(bool p_pause){
    // No-op.
}
bool ResonanceAudioSource::get_stream_paused() const {
    return false;
}

void ResonanceAudioSource::_get_samples() {

	AudioFrame *buffer = mix_buffer.ptrw();
	int buffer_size = mix_buffer.size();

    stream_playback->mix(buffer, 1.0, buffer_size);

    float source_buffer[buffer_size * 2];

    for (int sample = 0; sample < buffer_size; sample++) {
        source_buffer[sample * 2 + 0] = buffer[sample].l;
        source_buffer[sample * 2 + 1] = buffer[sample].r;
    }

    ResonanceAudioServer::get_singleton()->get_api()->SetInterleavedBuffer(
        source_id, source_buffer, /* num_channels= */ 2, buffer_size);

}