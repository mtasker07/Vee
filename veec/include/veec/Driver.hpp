/**
 * @file Driver.hpp
 * @brief This file contains the main class for the vee compiler driver.
 */

#pragma once

#include <iostream>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"

VEEC_NAMESPACE_BEGIN

/**
 * @class Driver
 * @brief This is the main entry point for the vee compiler.
 */
class Driver {
public:
    /**
     * @brief Creates a new Driver instance.
     */
    Driver() = default;

    int main(int argc, const char** argv);
};

VEEC_NAMESPACE_END
