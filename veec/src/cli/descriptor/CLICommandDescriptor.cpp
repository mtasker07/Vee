#include "veec/cli/descriptor/CLICommandDescriptor.hpp"

#include <string_view>
#include <vector>
#include <algorithm>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/cli/descriptor/CLIRootDescriptor.hpp"
#include "veec/cli/delegate/CLIDelegateTypes.hpp"
#include "veec/cli/CLI.hpp"

VEEC_NAMESPACE_BEGIN
namespace cli {
namespace descriptor {

namespace {

const descriptor::CLICommandDescriptor* findCommandDescriptor(const std::vector<CLICommandDescriptor>& commands, CLICommand command) {
    for (const CLICommandDescriptor& cmdDesc : commands) {
        if (cmdDesc.command == command) {
            return &cmdDesc;
        }
        if (!cmdDesc.subcommands.empty()) {
            const CLICommandDescriptor* subCmdDesc = findCommandDescriptor(cmdDesc.subcommands, command);
            if (subCmdDesc) {
                return subCmdDesc;
            }
        }
    }
    return nullptr;
}

} // namespace

const descriptor::CLICommandDescriptor* getCommandDescriptor(CLICommand command) {
    const descriptor::CLIRootDescriptor& rootDesc = getRootDescriptor();
    return findCommandDescriptor(rootDesc.commands, command);
}
const descriptor::CLICommandDescriptor* getCommandDescriptorByName(
    std::string_view name,
    const CLICommandDescriptor* parent,
    bool includeAliases
) {
    if (parent) {
        // Search through parent subcommands
        for (const CLICommandDescriptor& cmdDesc : parent->subcommands) {
            if (cmdDesc.namePrimary == name || (includeAliases && std::find(cmdDesc.nameAliases.begin(), cmdDesc.nameAliases.end(), name) != cmdDesc.nameAliases.end())) {
                return &cmdDesc;
            }
        }
    } else {
        // Search through root commands
        const descriptor::CLIRootDescriptor& rootDesc = getRootDescriptor();
        for (const CLICommandDescriptor& cmdDesc : rootDesc.commands) {
            if (cmdDesc.namePrimary == name || (includeAliases && std::find(cmdDesc.nameAliases.begin(), cmdDesc.nameAliases.end(), name) != cmdDesc.nameAliases.end())) {
                return &cmdDesc;
            }
        }
    }

    return nullptr;
}

} // namespace descriptor
} // namespace cli
VEEC_NAMESPACE_END
