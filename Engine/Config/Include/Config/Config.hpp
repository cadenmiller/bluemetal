#pragma once

//===========================//
#include <stdexcept>
#include <memory>
#include <string>
#include <string_view>
#include <vector>
//===========================//
#include "Config/ConfigParser.hpp"
//===========================//

class IConfigurable;
class CConfig
{
public:
    static inline const std::string DEFAULT_CONFIG_PATH = "config/config.nwdl";
    static const std::string DEFAULT_CONFIG;

    CConfig(const std::string& configPath = DEFAULT_CONFIG_PATH);
    ~CConfig();

    bool HasValue(const std::string&) const noexcept;
    void RequireStringValue(const std::string&, const std::string&);
    void RequireIntValue(const std::string&, int);
    int  GetIntValue(const std::string&) const;
    const std::string_view GetStringValue(const std::string&) const;
    void SetIntValue(const std::string&, int);
    void SetStringValue(const std::string&, const std::string&);

    int GetIntOrSetDefault(const std::string&, int);

    void Reset();
    void Save();

    void SubscribeEvents(IConfigurable const*);
    void UnsubscribeEvents(IConfigurable const*);
private:
    void EnsureDirectoriesExist();
    void ResetIfConfigDoesNotExist();
    void ParseInto();
    void NotifySubscribers();

    struct SValueNode
    {
        std::vector<SValueNode*> children;
        bool isValue;
        std::string name;
    };

    SValueNode*                                     m_pConfigTree;
    std::string                                     m_path = DEFAULT_CONFIG_PATH;
    std::unordered_map<std::string, SParsedValue>   m_values;
    std::vector<const IConfigurable*>               m_subscribers;
};

inline std::unique_ptr<CConfig> g_pConfig;