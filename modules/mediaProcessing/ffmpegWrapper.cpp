#include <iomanip>
#include <iostream>
#include <sstream>
#include <filesystem>

#include "ffmpegWrapper.h"


using namespace MediaProcessing;

ffmpegWrapper::ffmpegWrapper()
= default;

ffmpegWrapper::~ffmpegWrapper()
= default;

int ffmpegWrapper::videoCut(const std::string& videoPath, const std::string& cuttedVideosPath, const double fps, const double totalFrames, const unsigned int timePerEachVideo)
{
    const double frameDuration = 1.0 / fps;
    const int origineVideoDuration = static_cast<int>(totalFrames) / static_cast<int>(fps);

    //Number of frames per video
    const int frameBatchSize = static_cast<int>(fps) * static_cast<int>(timePerEachVideo);

    //Extract all frames
    unsigned int iterationNb = 0;
    for (int startFrame = 1; startFrame <= totalFrames; startFrame += frameBatchSize)
    {
        // Build output filename
        std::ostringstream outputNameStream;
        outputNameStream << cuttedVideosPath + "video_" << iterationNb << ".mp4";
        std::string outputVideoName = outputNameStream.str();

        //Get current path
        auto currentPath = std::filesystem::current_path().string();
        std::replace(currentPath.begin(), currentPath.end(), '\\', '/');

        //Fill videos map
        videoInfos vInfos;
        vInfos.videoName = "video_" + std::to_string(iterationNb) + ".mp4";
        auto cpath = currentPath + "/" + cuttedVideosPath + vInfos.videoName;
        _videosVector.emplace_back(cpath, vInfos);

        //Calculate the number of frames to extract in this iteration
        const int framesToExtract = std::min(frameBatchSize, static_cast<int>(totalFrames) - startFrame + 1);

        //Calculate start point and duration for FFmpeg command
        const int ss = static_cast<int>((startFrame - 1) * frameDuration);
        unsigned int t;
        if (const int iterationSize = (static_cast<int>(totalFrames) / frameBatchSize) * frameBatchSize; startFrame >= iterationSize)
        {
            t = origineVideoDuration % 2 == 0 ? timePerEachVideo : timePerEachVideo + 1;
        }
        else
        {
            t = static_cast<int>(framesToExtract * frameDuration);
        }

        //Build FFmpeg cut command
        std::string ffmpegCommand = "ffmpeg -i " + currentPath + "/" + videoPath +
            " -ss " + std::to_string(ss) + " -t " + std::to_string(t) +
            " -c:a aac -strict experimental " + outputVideoName;

        //Run FFmpeg cut command
        if (const int result = std::system(ffmpegCommand.c_str()); result != 0)
        {
            std::cerr << "Error extracting frames.\n";
            return -1;
        }
        ++iterationNb;
    }

    return 0;
}

int ffmpegWrapper::concatVideos(const std::string& input, const std::string& output)
{
    //Build FFmpeg concat command
    std::string ffmpegCommand = "ffmpeg -f concat -safe 0 -i " + input + " -c copy " + output;

    //Run FFmpeg concat command
    if (const int result = std::system(ffmpegCommand.c_str()); result != 0)
    {
        std::cerr << "Error concat videos.\n";
        return -1;
    }
    return 0;
}

std::vector<std::pair<std::string, ffmpegWrapper::videoInfos>> ffmpegWrapper::getVideosInfos()
{
    return _videosVector;
}

void ffmpegWrapper::setVideosInfos(const std::string&videoPath, const ffmpegWrapper::videoInfos& videoInfos)
{
    _videosVector.emplace_back(videoPath,videoInfos);
}

void ffmpegWrapper::clearVideosInfos()
{
    _videosVector.clear();
}

// Function to execute pushd
void ffmpegWrapper::pushd(const std::string& path) {
    // Build the pushd command
    std::string pushdCommand = "pushd " + path;

    // Execute the pushd command
    int result = std::system(pushdCommand.c_str());

    // Check the result
    if (result != 0) {
        // Handle the error, if needed
    }
}

// Function to execute popd
void ffmpegWrapper::popd() {
    // Execute the popd command
    int result = std::system("popd");

    // Check the result
    if (result != 0) {
        // Handle the error, if needed
    }
}


int ffmpegWrapper::extractAudioFromVideo(const std::string& videoPath, const std::string& audioPath) {
 
    //Build extracting audio FFmpeg command
    std::string ffmpegCommand = "ffmpeg -i " + videoPath + " -vn -acodec pcm_s16le -ar 44100 -ac 2 " + audioPath;

    //Run extracting audio FFmpeg command
    if (const int result = std::system(ffmpegCommand.c_str()); result != 0) {
        std::cerr << "Error extracting audio from " << videoPath << std::endl;
        return -1;
    }

    std::cout << "Audio extracted successfully: " << audioPath << std::endl;
    return 0;
}

int ffmpegWrapper::addSubtitleToVideo(const std::string& inputVideoPath, std::map<std::string, std::string> subtitles,  const std::string& fontSize,
    const std::string& fontColor, const std::string& backgroundColor, int x, const int y, const int box, const double lambdaBoxColor,
    const int boxBorderW, const std::string& outputVideoPath) const
{
    //// Build the drawtext filter string
    //const std::string drawtextFilter1 = "drawtext=text='" + subtitles["subtitle_1"] + "':fontsize=" + fontSize + ":fontcolor=" + fontColor +
    //    ":x=(w-text_w)/2" + ":y=h" + std::to_string(y) +
    //    ":box=" + std::to_string(box) +
    //    ":boxcolor=" + backgroundColor + "@" + std::to_string(lambdaBoxColor) +
    //    ":boxborderw=" + std::to_string(boxBorderW);


    std::string drawtextFilter;
    auto newY = y;
    for (const auto& entry : subtitles)
    {
        drawtextFilter += drawFilter(entry.second, fontSize, fontColor, backgroundColor, x, newY, box, lambdaBoxColor, boxBorderW);
        newY = newY + 100;
        drawtextFilter += ",";
    }


    if(drawtextFilter.empty())
    {
        const std::string s = "";
        drawtextFilter = "drawtext=text='" + s + "':fontsize=" + fontSize + ":fontcolor=" + fontColor +
            ":x=(w-text_w)/2" + ":y=h" + std::to_string(y) +
            ":box=" + std::to_string(box) +
            ":boxcolor=" + backgroundColor + "@" + std::to_string(lambdaBoxColor) +
            ":boxborderw=" + std::to_string(boxBorderW);
    }

    // Build the FFmpeg command
    std::string ffmpegCommand= "ffmpeg -i \"" + inputVideoPath + "\" -vf \"" + drawtextFilter + "\" -c:a copy \"" + outputVideoPath + "\"";


    // Run the FFmpeg command
    if (const int result = std::system(ffmpegCommand.c_str()); result != 0)
    {
        std::cerr << "Error adding subtitles to video.\n";
        return -1;
    }

    return 0;
}

std::string ffmpegWrapper::drawFilter(const std::string& subtitle, const std::string& fontSize, const std::string& fontColor,
                                      const std::string& backgroundColor, int x, const int y, const int box, const double lambdaBoxColor,
                                      const int boxBorderW) const
{
    return "drawtext=text='" + subtitle + "':fontsize=" + fontSize + ":fontcolor=" + fontColor +
            ":x=(w-text_w)/2" + ":y=h" + std::to_string(y) +
            ":box=" + std::to_string(box) +
            ":boxcolor=" + backgroundColor + "@" + std::to_string(lambdaBoxColor) +
            ":boxborderw=" + std::to_string(boxBorderW);
}
