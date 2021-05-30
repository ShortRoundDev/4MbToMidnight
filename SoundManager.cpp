#include "SoundManager.hpp"
#include <iostream>
#include <fstream>

#include "vorbis/codec.h"
#include "vorbis/vorbisfile.h"

#include "GameManager.hpp"

std::unique_ptr<SoundManager> SoundManager::instance = nullptr;

void SoundManager::init() {
    instance = std::make_unique<SoundManager>();
}

SoundManager::SoundManager() {
    device = alcOpenDevice(NULL);
    if(!device) {
        std::cerr << "Couldn't open AL Device!" << std::endl;
        return;
    }
    
    context = alcCreateContext(device, NULL);
    if(!alcMakeContextCurrent(context)) {
        std::cerr << "Couldn't create AL Context!" << std::endl;
        return;
    }
        
    alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);

    auto pos = GameManager::instance->player.pos;
    alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
    ALfloat orientation[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
    alListenerfv(AL_ORIENTATION, orientation);
    
    auto err = alGetError();
    if(err != AL_NO_ERROR){
        std::cerr << err << std::endl;
    }
    
    generateSource(&once, false);
    generateSource(&loop, true);
    loadFile("Resources/Audio/DoorClose.ogg");
}

SoundManager::~SoundManager() {
}

void SoundManager::update(){
    auto pos = GameManager::instance->player.pos;
    alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
    auto up = GameManager::instance->camera.cameraUp;
    auto front = GameManager::instance->camera.cameraFront;
    float orientation[] = { front.x, front.y, front.z, up.x, up.y, up.z };
    alListenerfv(AL_ORIENTATION, orientation);
}

ALuint SoundManager::generateSource(ALuint* source, bool loop) {
    alGenSources(1, source);
    alSourcef(*source, AL_PITCH, 2.0f);
    alSourcef(*source, AL_GAIN, 1.0f);
    alSource3f(*source, AL_POSITION, -100.0f, 0.0f, 0.0f);
    auto err = alGetError();
    if(err != AL_NO_ERROR){
        std::cerr << err << std::endl;
    }

    alSource3f(*source, AL_VELOCITY, 1.0f, 0, 0);
    err = alGetError();
    if(err != AL_NO_ERROR){
        std::cerr << err << std::endl;
    }
    
    alSourcef(*source, AL_ROLLOFF_FACTOR, 3.0f);
    err = alGetError();
    if(err != AL_NO_ERROR){
        std::cerr << err << std::endl;
    }
    
    alSourcef(*source, AL_REFERENCE_DISTANCE, 1.0f);
    
    alSourcef(*source, AL_MAX_DISTANCE, 10.0f);
    err = alGetError();
    if(err != AL_NO_ERROR){
        std::cerr << err << std::endl;
    }
    
    alSourcei(*source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
    err = alGetError();
    if(err != AL_NO_ERROR){
        std::cerr << err << std::endl;
    }
    
    return *source;
}

ALuint SoundManager::loadFile(std::string path) {
    ALuint buffer;
    alGenBuffers(1, &buffer);
    if(!readOgg(path, buffer)){
        sounds[path] = buffer;
    }
    
    return buffer;
}

int SoundManager::readOgg(std::string path, ALuint buffer) {
    FILE* fp = fopen(path.c_str(), "rb");
    if(!fp) {
        std::cerr << "Couldn't open OGG file " << path << std::endl;
        return -1;
    }

    OggVorbis_File vf;
    if(ov_open_callbacks(fp, &vf, NULL, 0, OV_CALLBACKS_NOCLOSE) < 0){
        std::cerr << path << " is not a valid OGG file" << std::endl;
        fclose(fp);
        return -2;
    }
    
    vorbis_info* vi = ov_info(&vf, -1);
    ALenum format = AL_FORMAT_MONO16;// : AL_FORMAT_STEREO16;
    
    size_t length = ov_pcm_total(&vf, -1) * vi->channels * 2;
    short* pcmout = (short*)malloc(length);
    int status = 0;
    for(
        size_t size = 0, offset = 0, sel = 0;
        (size = ov_read(&vf, (char*) pcmout + offset, 4096, 0, 2, 1, (int*)&sel)) != 0;
        offset += size
    ){
        if(size <= 0) {
            std::cerr << "Bad OGG File!" << std::endl;
            status = -3;
            goto exit;
        }
    }
    alBufferData(buffer, format, pcmout, length, vi->rate);
exit:
    free(pcmout);
    ov_clear(&vf);
    fclose(fp);
    return status;
}


void SoundManager::playSound(std::string path, const glm::vec3& pos) {
    auto bufferSpot = sounds.find(path);
    if(bufferSpot != sounds.end()){
        playSound(bufferSpot->second, pos);
    }
}

void SoundManager::playSound(ALuint buffer, const glm::vec3& pos) {
    alSourcei(once, AL_BUFFER, buffer);
    alSource3f(once, AL_POSITION, pos.x, pos.y, pos.z);
    alSourcePlay(once);
}