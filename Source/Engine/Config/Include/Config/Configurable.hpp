#pragma once

#include <unordered_map>

#include "Config.hpp"

namespace bl
{

class CConfig;

class IConfigurable
{
    CConfig *const m_pConfig = nullptr;
public:
    IConfigurable() = default;
    IConfigurable(CConfig *const); /* Automatic subscription/unsubscription */
    ~IConfigurable();

    virtual void OnConfigReceive(const std::unordered_map<std::string, SParsedValue>&) {}
    virtual void OnConfigPreSave() {}
    virtual void OnConfigChanged(const SParsedValue&) {}
};

} // namespace bl