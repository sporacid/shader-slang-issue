#include <print>

#include "slang-com-ptr.h"
#include "slang.h"

int main(const int argc, const char* argv[])
{
    SlangResult slang_result = SLANG_OK;

    Slang::ComPtr<slang::IGlobalSession> global_session;
    slang::createGlobalSession(global_session.writeRef());

    if (global_session == nullptr)
    {
        return -1;
    }

    slang::SessionDesc session_desc;
    Slang::ComPtr<ISlangUnknown> session_memory;
    slang_result = global_session->parseCommandLineArguments(argc - 1, argv + 1, &session_desc, session_memory.writeRef());

    if (SLANG_FAILED(slang_result))
    {
        return -1;
    }

    constexpr auto make_predicate = [](const slang::CompilerOptionName compiler_option_name) {
        return [=](const slang::CompilerOptionEntry& entry) {
            return entry.name == compiler_option_name;
        };
    };

    std::println("target count: {}", session_desc.targetCount);

    for (std::size_t index = 0; index < session_desc.targetCount; ++index)
    {
        const slang::TargetDesc& target_desc = session_desc.targets[index];

        std::println("target: {}", index);
        std::println("  format: {}", static_cast<int>(target_desc.format));
        std::println("  profile: {}", static_cast<int>(target_desc.profile));

        const auto target = std::ranges::find_if(
            target_desc.compilerOptionEntries,
            target_desc.compilerOptionEntries + target_desc.compilerOptionEntryCount,
            make_predicate(slang::CompilerOptionName::Target));

        if (target != nullptr)
        {
            std::println("  target option: {}", target->value.intValue0);
        }

        const auto profile = std::ranges::find_if(
            target_desc.compilerOptionEntries,
            target_desc.compilerOptionEntries + target_desc.compilerOptionEntryCount,
            make_predicate(slang::CompilerOptionName::Profile));

        if (profile != nullptr)
        {
            std::println("  profile option: {}", profile->value.intValue0);
        }
    }

    return 0;
}
