#include "veec/symbols/Symbol.hpp"

#include <string_view>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/symbols/SymbolKind.hpp"

VEEC_NAMESPACE_BEGIN
namespace symbols {

std::string_view Symbol::kindString(SymbolKind kind) {
	switch (kind) {
		case SymbolKind::Module: return "module";
		case SymbolKind::Function: return "function";
		case SymbolKind::FunctionSet: return "function";
		case SymbolKind::Variable: return "variable";
		case SymbolKind::Class: return "class";
		case SymbolKind::Field: return "field";
		default:
			VEE_UNREACHABLE("Unexpected symbol kind: {}", static_cast<u8>(kind));
	}
}

} // namespace symbols
VEEC_NAMESPACE_END
