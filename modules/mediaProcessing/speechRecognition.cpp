#include "speechRecognition.h"

#include <array>
#include <memory>
#include <stdexcept>


using namespace MediaProcessing;

speechRecognition::speechRecognition()
= default;

speechRecognition::~speechRecognition()
= default;

int speechRecognition::convertAudioToText(const std::string& audioPath, const std::string& language, std::string& text)
{

    std::array<char, 128> buffer;
    std::string result;

    //const std::string cmd = "python script.py \"output_audio_francais.wav\" --language fr-FR";
    const std::string script = "python script.py ";
    const std::string cmd = script  + "\"" + audioPath + "\"" + " --language " + language;

    const std::unique_ptr<FILE, decltype(&_pclose)> pipe(_popen(cmd.c_str(), "r"), _pclose);

    if (!pipe) {
        return -1;
    }

    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    text = result;
    return 0;
}
