#include "veec/cli/descriptor/CLIRootDescriptor.hpp"

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/cli/commands/Compile.hpp"
#include "veec/cli/descriptor/CLICommandDescriptor.hpp"
#include "veec/cli/delegate/CLIDelegateTypes.hpp"

VEEC_NAMESPACE_BEGIN
namespace cli {
namespace descriptor {

const descriptor::CLIRootDescriptor& getRootDescriptor() {
    //
    // GLOBAL OPTIONS
    //

    static const std::vector<descriptor::CLIOptionDescriptor> globalOptions = {
        // version
        {
            /* option               */ CLIOption::Version,
            /* nameShort            */ 'v',
            /* nameLong             */ "version",
            /* description          */ "Display version information",
            /* type                 */ CLIOptionType::Flag,
            /* valueType            */ CLIValueType::None,
            /* defaultValue         */ {},
            /* flagDefaultValue     */ false,
            /* minValues            */ 0,
            /* maxValues            */ 1,
            /* required             */ false,
            /* validateValue        */ nullptr,
        }
    };

    //
    // ROOT DESCRIPTOR
    //

    static const descriptor::CLIRootDescriptor rootDescriptor = {
        // defaultCommandIndex
        0,

        // commands
        {
            // compile
            commands::getCompileCommandDescriptor(),
        },

        // globalOptions
        globalOptions,
    };

    return rootDescriptor;
}

} // namespace descriptor
} // namespace cli
VEEC_NAMESPACE_END
