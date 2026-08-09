#include <vector>
#include <string>
#include <string_view>
#include <iostream>

#include "veec/compilation/Compilation.hpp"
#include "veec/compilation/CompilationContext.hpp"
#include "veec/compilation/CompilationConfig.hpp"
#include "veec/io/StreamWriter.hpp"
#include "veec/source/SourceManager.hpp"
#include "veec/diagnostics/DiagnosticEngine.hpp"
#include "veec/diagnostics/rendering/TerminalDiagnosticRenderer.hpp"

int main() {
    using namespace veec;

    constexpr std::string_view sourceCode = R"(

func add<T>(a: T, b: T) -> T {
    return a + b;
}

func main() -> i32 {
    let x: i64 = 6;
    let y: i32 = add<i32>(x, 10);
    return y;
}

)";
    
    compilation::CompilationConfig config{};
    compilation::Compilation compilation(config);

    compilation.sources().addVirtualFile("main.vee", sourceCode);

    compilation::CompilationResult result = compilation.compile();

    if (!result.success) {
        io::StreamWriter terminalWriter(std::cout);
        diagnostics::TerminalDiagnosticRenderer diagRenderer;
        for (const auto& diag : result.diagnostics) {
            diagRenderer.render(diag, terminalWriter);
            terminalWriter.write("\n");
        }
        return 1;
    }
    std::cout << "Compilation succeeded!" << std::endl;
    return 0;
}
