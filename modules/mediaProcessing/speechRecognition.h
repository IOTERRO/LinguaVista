#pragma once

#include <string>

#include "export.h"

namespace MediaProcessing
{
    class MEDIA_PROCESSING_API speechRecognition
    {
    public:
        speechRecognition();
        ~speechRecognition();

        int convertAudioToText(const std::string& audioPath, const std::string& language, std::string& text);
    };
}
