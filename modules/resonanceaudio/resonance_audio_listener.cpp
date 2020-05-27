#include "resonance_audio_listener.h"
#include "core/engine.h"
#include "core/math/vector3.h"
#include "core/math/quat.h"
#include "servers/audio_server.h"
#include "resonance_audio_server.h"
#include <string>

void ResonanceAudioListener::_bind_methods() {

}

void ResonanceAudioListener::_notification(int p_what) {

	if (p_what == NOTIFICATION_ENTER_TREE) {
        ERR_PRINT("listener entering tree\n");
		if (!Engine::get_singleton()->is_editor_hint()) {
            active = true;
    		AudioServer::get_singleton()->add_callback(_mix_audio_cb, this);
		}
    	mix_buffer.resize(AudioServer::get_singleton()->thread_get_mix_buffer_size());
	}

	if (p_what == NOTIFICATION_EXIT_TREE) {
		if (!Engine::get_singleton()->is_editor_hint()) {
		    AudioServer::get_singleton()->remove_callback(_mix_audio_cb, this);
        }
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

	if (p_what == NOTIFICATION_TRANSFORM_CHANGED) {
		// if (doppler_tracking != DOPPLER_TRACKING_DISABLED) {
		// 	velocity_tracker->update_position(get_global_transform().origin);
		// }
	}

	if (p_what == NOTIFICATION_INTERNAL_PHYSICS_PROCESS) {
    }
}

void ResonanceAudioListener::_mix_audio() {

    ResonanceAudioServer* server = ResonanceAudioServer::get_singleton();
    server->lock();

    server->notify_samples_needed();


    // Update the position of the listener.
    Vector3 head_position = get_global_transform().origin;
    Quat head_rotation = Quat(get_global_transform().basis);

    server->get_api()->SetHeadPosition(head_position.x, head_position.y, head_position.z);
    server->get_api()->SetHeadRotation(head_rotation.x, head_rotation.y, head_rotation.z, head_rotation.w);

    // static int count = 10;
    // if (count) {
    //     count--;
    //     return;
    // }

    AudioFrame *target = AudioServer::get_singleton()->thread_get_channel_mix_buffer(/* bus_index= */ 0, /* channel_idx= */ 0);
	int buffer_size = AudioServer::get_singleton()->thread_get_mix_buffer_size();

    float output_buffer[buffer_size * 2];

    bool did_render = server->get_api()->FillInterleavedOutputBuffer(
        /* num_channels= */ 2,  buffer_size, output_buffer);

    if (did_render) {
        for (int sample = 0; sample < buffer_size; sample++) {
            target[sample].l = output_buffer[sample * 2 + 0];
            target[sample].r = output_buffer[sample * 2 + 1];
        }
    }
    else {
        ERR_PRINT("Sound did not render!");
    }

    server->unlock();

}