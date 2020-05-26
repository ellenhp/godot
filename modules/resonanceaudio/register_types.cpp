#include "register_types.h"

#include "core/class_db.h"
#include "resonance_audio_server.h"
#include "resonance_audio_listener.h"
#include "resonance_audio_source.h"

void register_resonanceaudio_types() {
    ClassDB::register_class<ResonanceAudioServer>();
    ClassDB::register_class<ResonanceAudioListener>();
    ClassDB::register_class<ResonanceAudioSource>();
}
void unregister_resonanceaudio_types() {

}
