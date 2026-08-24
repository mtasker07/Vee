#include "veec/cli/descriptor/CLICommandDescriptor.hpp"

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/cli/descriptor/CLIRootDescriptor.hpp"
#include "veec/cli/descriptor/CLICommandDescriptor.hpp"
#include "veec/cli/delegate/CLIDelegateTypes.hpp"
#include "veec/cli/CLI.hpp"

VEEC_NAMESPACE_BEGIN
namespace cli {
namespace descriptor {

namespace {

const CLIOptionDescriptor* findOptionDescriptor(const std::vector<CLIOptionDescriptor>& options, CLIOption option) {
    for (const CLIOptionDescriptor& optDesc : options) {
        if (optDesc.option == option) {
            return &optDesc;
        }
    }
    return nullptr;
}
const CLIOptionDescriptor* findOptionDescriptorInCommandTree(const CLICommandDescriptor& commandDesc, CLIOption option) {
    const CLIOptionDescriptor* optDesc = findOptionDescriptor(commandDesc.options, option);
    if (optDesc != nullptr) {
        return optDesc;
    }
    for (const CLICommandDescriptor& subcommandDesc : commandDesc.subcommands) {
        optDesc = findOptionDescriptorInCommandTree(subcommandDesc, option);
        if (optDesc != nullptr) {
            return optDesc;
        }
    }
    return nullptr;
}

} // namespace

const CLIOptionDescriptor* getOptionDescriptor(CLIOption option) {
    const CLIRootDescriptor& rootDesc = getRootDescriptor();
    
    // TODO: Cache in map

    // Global options
    const CLIOptionDescriptor* optDesc = findOptionDescriptor(rootDesc.globalOptions, option);
    if (optDesc != nullptr) {
        return optDesc;
    }
    
    // Command-specific options
    for (const CLICommandDescriptor& commands : rootDesc.commands) {
        optDesc = findOptionDescriptorInCommandTree(commands, option);
        if (optDesc != nullptr) {
            return optDesc;
        }
    }

    return nullptr;
}
const CLIOptionDescriptor* getOptionDescriptorByLongName(CLICommand command, std::string_view longName) {
    const std::vector<CLIOptionDescriptor>* options = nullptr;
    if (command == CLICommand::Unknown) {
        options = &getRootDescriptor().globalOptions;
    } else {
        const CLICommandDescriptor* commandDesc = getCommandDescriptor(command);
        VEE_ASSERT(commandDesc != nullptr, "Command descriptor not found for command");
        options = &commandDesc->options;
    }

    for (const CLIOptionDescriptor& optDesc : *options) {
        if (optDesc.nameLong == longName) {
            return &optDesc;
        }
    }

    return nullptr;
}
const CLIOptionDescriptor* getOptionDescriptorByShortName(CLICommand command, char shortName) {
    const std::vector<CLIOptionDescriptor>* options = nullptr;
    if (command == CLICommand::Unknown) {
        options = &getRootDescriptor().globalOptions;
    } else {
        const CLICommandDescriptor* commandDesc = getCommandDescriptor(command);
        VEE_ASSERT(commandDesc != nullptr, "Command descriptor not found for command");
        options = &commandDesc->options;
    }

    for (const CLIOptionDescriptor& optDesc : *options) {
        if (optDesc.nameShort == shortName) {
            return &optDesc;
        }
    }

    return nullptr;
}

} // namespace descriptor
} // namespace cli
VEEC_NAMESPACE_END
