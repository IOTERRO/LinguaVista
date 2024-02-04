#include "opencvWrapper.h"

#include <opencv2/opencv.hpp>
using namespace MediaProcessing;

openCvWrapper::openCvWrapper()
{
    
}

openCvWrapper::~openCvWrapper()
{
    
}

int openCvWrapper::getVideoFrameInfo(const std::string& videoPath, VideoFrameInfos& videoFrameInfos)
{
    const cv::VideoCapture cap(videoPath);
    if (!cap.isOpened())
    {
        std::cerr << "Error opening video file.\n";
        return -1;
    }

    videoFrameInfos.totalFrames = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_COUNT));
    videoFrameInfos.fps = cap.get(cv::CAP_PROP_FPS);
    return 0;
}


int openCvWrapper::addSubtitleToVideo(const std::string& inputVideoPath, const std::string& outputVideoPath, const std::string& subtitleText) const
{
    cv::VideoCapture inputVideo(inputVideoPath);
    if (!inputVideo.isOpened())
    {
        std::cerr << "Error opening input video file.\n";
        return -1;
    }

    const cv::Size videoResolution(static_cast<int>(inputVideo.get(cv::CAP_PROP_FRAME_WIDTH)), static_cast<int>(inputVideo.get(cv::CAP_PROP_FRAME_HEIGHT)));
    cv::VideoWriter outputVideo(outputVideoPath, cv::VideoWriter::fourcc('X', '2', '6', '4'), inputVideo.get(cv::CAP_PROP_FPS), videoResolution);

    if (!outputVideo.isOpened())
    {
        std::cerr << "Error opening output video file.\n";
        return -1;
    }

    const cv::Scalar backgroundColor(0, 0, 0); // Black background
    const cv::Scalar fontColor(255, 255, 255); // White color
    constexpr int font = cv::FONT_HERSHEY_SIMPLEX;

    cv::Mat frame;
    while (inputVideo.read(frame))
    {
        constexpr int fontThickness = 4;
        constexpr double fontScale = 1.5;
        // Calculate text size and position
        int baseline = 0;
        const cv::Size textSize = cv::getTextSize(subtitleText, font, fontScale, fontThickness, &baseline);
        const cv::Point textPosition((videoResolution.width - textSize.width) / 2, videoResolution.height - 200);

        // Add black background
        cv::rectangle(frame, textPosition + cv::Point(0, baseline), textPosition + cv::Point(textSize.width, -textSize.height), backgroundColor, -1);

        // Add text to the frame
        cv::putText(frame, subtitleText, textPosition, font, fontScale, fontColor, fontThickness);

        // Write the frame to the output video
        outputVideo.write(frame);
    }

    return 0;
}
