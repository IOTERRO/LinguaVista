#pragma once

#include <map>
#include <string>

#include "export.h"

namespace MediaProcessing
{
    class MEDIA_PROCESSING_API ffmpegWrapper
    {
    public:
        struct videoInfos
        {
            std::string videoName;
            std::string audioName;
            std::string subtitle;
        };

        ffmpegWrapper();
        ~ffmpegWrapper();

        int videoCut(const std::string& videoPath, const std::string& cuttedVideosPath,  double fps,  double totalFrames,  unsigned int timePerEachVideo);
        int concatVideos(const std::string& input, const std::string& output);
        int extractAudioFromVideo(const std::string& videoPath, const std::string& audioPath);
        int addSubtitleToVideo(const std::string& inputVideoPath, std::map<std::string, std::string> subtitles,
                               const std::string& fontSize, const std::string& fontColor, const std::string& backgroundColor, 
                               int x, int y, int box, double lambdaBoxColor, int boxBorderW,
                               const std::string& outputVideoPath) const;
        std::vector<std::pair<std::string, videoInfos>> getVideosInfos();
        void setVideosInfos(const std::string& videoPath, const ffmpegWrapper::videoInfos& videoInfos);
        void clearVideosInfos();


        //UNC Path permission
        void pushd(const std::string& path);
        void popd();

    private:
        std::string ffmpegWrapper::drawFilter(const std::string& subtitle, const std::string& fontSize, const std::string& fontColor,
            const std::string& backgroundColor, int x, const int y, const int box, const double lambdaBoxColor,
            const int boxBorderW) const;
        std::vector<std::pair<std::string, videoInfos>> _videosVector;
    };
}
