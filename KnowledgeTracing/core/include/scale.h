//
// Created by couland-q on 24/10/22.
//

#ifndef KNOWLEDGE_TRACING_SCALE_H
#define KNOWLEDGE_TRACING_SCALE_H

#include <string>

#include <pl.h>

/**
 * Struct that quantifies the level of mastery of a skill.
 * For now it's either "1- insuffisant" or "2 - Satisfaisant"
 */
struct Scale{
    plLabelType scale_values;

    /**
     * Default constructor for the Scale class, default value is {"1 - insuffisant", "2 - Satisfaisant"}.
     */
    Scale() {
        scale_values = plLabelType({"1 - insuffisant", "2 - satisfaisant"});
    }

    /**
     * Constructor for the Scale class.
     * @params scale_values Level of mastery of a skill (from 1 to 4 for example).
     */
    explicit Scale(const plLabelType& scale_values): scale_values(scale_values) {};


    Scale(const Scale& os) {
        this->scale_values = plLabelType(os.scale_values);
    }

    /**
     * Gets the number of masteries of a skill. Default value is 2.
     * @return Returns the size as a size_t.
     */
    size_t getNumberOfValues() const {
        return scale_values.get_values().size();
    }

    std::string getHighestValue() const {
        std::vector<std::string> all_values = scale_values.get_values();
        return all_values.back();
    }

    std::string getValueFromIdx(size_t val_idx) const {
        if (val_idx < this->scale_values.get_values().size()) {
            return this->scale_values.get_values()[val_idx];
        }

        else {
            std::cerr << "[scale.h] ERROR: index is higher than number of values in the scale (index is " << val_idx
            << ", number of values in scale is " << this->scale_values.get_values().size() << "). Returning "
            << "last value instead (expect crashes or weird behaviours." << std::endl;

            return this->scale_values.get_values().back();
        }

    }
};


#endif //KNOWLEDGE_TRACING_SCALE_H
