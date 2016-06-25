#include "Configure.h"
#include <boost/algorithm/string.hpp>
#include <glog/logging.h>

#include "ConfigException.h"

namespace jcpp {
namespace configure {

int32_t Configure::load(const char* path, const char* conf, int32_t dup_level) {
    if (path == NULL || conf == NULL) {
        LOG(WARNING) << "Configure.load : path or filename error...";
        return CONFIG_ERROR_ERROR;
    }

    if (_m_conf_flist.size() != 0) {
        LOG(WARNING) << "Configure had at least read a file before!";
        return CONFIG_ERROR_ERROR;
    }

    if (path[0] == '\0') {
        _m_path = ".";
    } else {
        _m_path = path;
    }
    _m_dup_level = dup_level;

    _m_conf_flist.push_back(std::pair<int32_t, std::string>(0, conf));
    for (size_t idx = 0; idx < _m_conf_flist.size(); ++idx) {
        std::vector<std::string> content;
        if (ConfigUtils::read_file(_m_path + "/" + _m_conf_flist[idx].second, content) < 0) {
            LOG(WARNING) << "Can't read conf [" << _m_conf_flist[idx].second << "]";
            continue;
        }
        ConfigUtils::cur_data_pos.file_name = _m_conf_flist[idx].second;
        _m_section = this;
        _m_depth = 0;
        for (size_t k = 0; k < content.size(); ++k) {
            ConfigUtils::cur_data_pos.file_line = k + 1;
            process_line(content[k], _m_conf_flist[idx].first);
        }
    }
    print_dup();
    return 0;
}

int32_t Configure::process_line(const std::string& str, int32_t level) {
    std::string line = boost::trim_copy(str);
    if (line.size() == 0 || line[0] == '#') {
        return 0;
    }

    if (line[0] == '[') {
        std::string section_name;
        if (ConfigUtils::section_parser(line, section_name) < 0) {
            LOG(WARNING) << "Configure read section [" << str << "] failed";
            return CONFIG_ERROR_FORMAT_ERROR;
        }
        change_section(section_name);
    } else {
        std::string key;
        std::string value;
        if (ConfigUtils::line_parser(line, key, value) < 0) {
            LOG(WARNING) << "Configure read line [" << str << "] failed";
            return CONFIG_ERROR_FORMAT_ERROR;
        }
        if (key == CONFIG_INCLUDE) {
            _m_conf_flist.push_back(std::pair<int32_t, std::string>(level + 1, value));
        } else {
            push_pair(key, value);
        }
    }
    return 0;
}

int32_t Configure::push_pair(const std::string& key, const std::string& value) {
    ConfigUnit *unit = new(std::nothrow) ConfigUnit(key, value,
                                                        _m_section, ConfigUtils::cur_data_pos);
    if (unit == NULL) {
        LOG(WARNING) << "Allocate memory failed. key=[" << key << "],value=[" << value << "]";
        return CONFIG_ERROR_ERROR;
    }

    if (_m_section != NULL && _m_section->push(key, unit) != CONFIG_ERROR_OK) {
        delete unit;
        throw ConfigException();
    }
    return 0;
}

int32_t Configure::change_section(std::string section_name) {
    if (section_name[0] != '.') {
        _m_section = this;
        _m_depth = 0;
    } else {
        uint32_t k;
        for (k = 0; k < section_name.size(); ++k) {
            if (section_name[k] != '.') {
                break;
            }
        }

        if (k > _m_depth) {
            LOG(WARNING) << "Configure : Section Error [" << section_name << "] Ignored.";
            throw ConfigException();
        }
        section_name = section_name.substr(k);

        k = _m_depth - k;
        while (k > 0) {
            _m_section = static_cast<ConfigGroup*>(_m_section->get_father());
            --_m_depth;
            --k;
        }
    }

    ConfigGroup* _orig_section = _m_section;
    _m_section = (ConfigGroup *)_m_section->find_section(section_name, &_m_depth);
    if (_m_section == NULL) {
        _m_section = _orig_section;
        LOG(WARNING) << "Change section failed, can't find target section, use previous section";
    }
    return 0;
}

} // END namespace configure
} // END namespace jcpp
