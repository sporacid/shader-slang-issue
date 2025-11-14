#include <fstream>
#include <print>
#include <sstream>
#include <string>
#include <string_view>

#include "slang-com-ptr.h"
#include "slang.h"

bool read_file(const std::string_view path, std::string& content)
{
    const std::ifstream stream(path.data());

    if (!stream.is_open())
    {
        return false;
    }

    std::stringstream string;
    string << stream.rdbuf();
    content = string.str();
    return !stream.bad();
}

int main(const int argc, const char* argv[])
{
    SlangResult slang_result = SLANG_OK;

    std::string shader_data;
    if (not read_file("shader.slang", shader_data))
    {
        return -1;
    }

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

    Slang::ComPtr<slang::ISession> session;
    slang_result = global_session->createSession(session_desc, session.writeRef());

    if (SLANG_FAILED(slang_result))
    {
        return -1;
    }

    slang::IModule* module = session->loadModuleFromSourceString(
        "shader",
        "shader.slang",
        shader_data.data(),
        nullptr);

    if (module == nullptr)
    {
        return -1;
    }

    for (std::int32_t index_entry_point = 0; index_entry_point < module->getDefinedEntryPointCount(); ++index_entry_point)
    {
        Slang::ComPtr<slang::IEntryPoint> entry_point;
        slang_result = module->getDefinedEntryPoint(index_entry_point, entry_point.writeRef());

        if (SLANG_FAILED(slang_result))
        {
            continue;
        }

        for (std::int32_t index_target = 0; index_target < session_desc.targetCount; ++index_target)
        {
            if (slang::ProgramLayout* program_layout = module->getLayout(index_target))
            {
                if (slang::EntryPointLayout* entry_point_layout = program_layout->getEntryPointByIndex(index_entry_point))
                {
                    std::println("entry point found for target: name={} stage={}",
                        entry_point->getFunctionReflection()->getName(), static_cast<int>(entry_point_layout->getStage()));
                }
            }
        }
    }

    return 0;
}
