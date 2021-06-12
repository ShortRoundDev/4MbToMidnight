#pragma once

#include <vector>
#include <memory>
#include <map>
#include <string>

#include "AL/al.h"
#include "AL/alc.h"
#include "ogg/ogg.h"

#include "glm/glm.hpp"

class SoundManager {
public:
    static void init();
    static std::unique_ptr<SoundManager> instance;
    
    static void update();
    
    SoundManager();
    ~SoundManager();
    
    std::vector<ALuint> sourceStack;
    
    ALCdevice* device;
    ALCcontext* context;
    
    ALuint generateSource(ALuint* source, bool loop);
    ALuint loadFile(std::string path, bool stereo);
    int readOgg(std::string path, ALuint buffer, bool stereo);
    
    void playSound(std::string path, const glm::vec3& pos);
    void playSound(ALuint buffer, const glm::vec3& pos);

    std::map<std::string, ALuint> sounds;
    
    void _update();
};