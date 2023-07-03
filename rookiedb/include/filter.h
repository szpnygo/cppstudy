#pragma once
#include "extra_attributes.h"

enum class ConditionType {
    Equal,
    NotEqual,
    GreaterThan,
    GreaterThanOrEqual,
    LessThan,
    LessThanOrEqual,
};

struct Condition {
    ConditionType type;
    Value value;
};

class Filter {
  public:
    void setEqual(const std::string& key, Value value) {
        conditions[key] = {ConditionType::Equal, value};
    }

    void setNotEqual(const std::string& key, Value value) {
        conditions[key] = {ConditionType::NotEqual, value};
    }

    void setGreaterThan(const std::string& key, Value value) {
        conditions[key] = {ConditionType::GreaterThan, value};
    }

    void setGreaterThanOrEqual(const std::string& key, Value value) {
        conditions[key] = {ConditionType::GreaterThanOrEqual, value};
    }

    void setLessThan(const std::string& key, Value value) {
        conditions[key] = {ConditionType::LessThan, value};
    }

    void setLessThanOrEqual(const std::string& key, Value value) {
        conditions[key] = {ConditionType::LessThanOrEqual, value};
    }

    bool matchAttributes(const Attributes& attributes) const {
        if (conditions.empty()) {
            return true;
        }
        for (const auto& [key, condition] : conditions) {
            auto it = attributes.find(key);
            if (it == attributes.end()) {
                return false;
            }
            switch (condition.type) {
            case ConditionType::Equal:
                if (it->second != condition.value) {
                    return false;
                }
                break;
            case ConditionType::NotEqual:
                if (it->second == condition.value) {
                    return false;
                }
                break;
            case ConditionType::GreaterThan:
                if (it->second <= condition.value) {
                    return false;
                }
                break;
            case ConditionType::GreaterThanOrEqual:
                if (it->second < condition.value) {
                    return false;
                }
                break;
            case ConditionType::LessThan:
                if (it->second >= condition.value) {
                    return false;
                }
                break;
            case ConditionType::LessThanOrEqual:
                if (it->second > condition.value) {
                    return false;
                }
                break;
            }
        }
        return true;
    }

  private:
    std::unordered_map<std::string, Condition> conditions;
};