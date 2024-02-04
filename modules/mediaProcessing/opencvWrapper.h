#pragma once

#include <string>

#include "export.h"

namespace MediaProcessing
{
    class MEDIA_PROCESSING_API openCvWrapper
    {
    public:
        openCvWrapper();
        ~openCvWrapper();

        struct VideoFrameInfos
        {
            double fps;
            double totalFrames;
        };

        int getVideoFrameInfo(const std::string& videoPath, VideoFrameInfos& videoFrameInfos);
        int addSubtitleToVideo(const std::string& inputVideoPath, const std::string& outputVideoPath, const std::string& subtitleText) const;
    };
}
