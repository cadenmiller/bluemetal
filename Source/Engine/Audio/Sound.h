#pragma once

///////////////////////////////
// Headers
///////////////////////////////

#include "fmod.h"
#include "AudioSystem.h"

#include "Resource/Resource.h"

namespace bl
{

///////////////////////////////
// Forward Declarations
///////////////////////////////

class AudioSystem;

///////////////////////////////
// Classes
///////////////////////////////

class BLUEMETAL_API Sound
{
public:
    Sound(std::shared_ptr<AudioSystem> device, const std::filesystem::path& file);
    ~Sound();

    FMOD_SOUND* getHandle();

private:
    std::shared_ptr<AudioSystem>    m_system;
    FMOD_SOUND*                     m_pSound;
};

} // namespace bl