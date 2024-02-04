#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <windows.h>
#include <winuser.h>

#include "ffmpegWrapper.h"
#include "opencvWrapper.h"
#include "speechRecognition.h"


static std::string Root = "/";
static std::string TemporaryFramesFolder = "wave_frames_temp/";
static std::string ListFile = "/list.txt";

using namespace MediaProcessing;

std::shared_ptr<openCvWrapper> OpencvWrapper = std::make_shared<MediaProcessing::openCvWrapper>();
std::shared_ptr<ffmpegWrapper> FFmpegWrapper = std::make_shared<MediaProcessing::ffmpegWrapper>();
std::shared_ptr<speechRecognition> SpeechRecognition = std::make_shared<MediaProcessing::speechRecognition>();

auto CurrentPath = std::filesystem::current_path().string();


bool IsProcessRunning(const std::string& processName) {
    std::string command = "tasklist /FI \"IMAGENAME eq " + processName + "\" /NH";
    std::ostringstream resultStream;

    // Run the tasklist command and capture the output
    FILE* pipe = _popen(command.c_str(), "r");
    if (!pipe) {
        std::cerr << "Error executing tasklist command.\n";
        return false;
    }

    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        resultStream << buffer;
    }

    _pclose(pipe);

    // Check if the process is in the output
    return resultStream.str().find(processName) != std::string::npos;
}

int KillProcessByName(const std::string& processName) {
    if (IsProcessRunning(processName)) {
        std::string command = "taskkill /IM " + processName + " /F";
        if (const auto result = std::system(command.c_str()); result != 0)
        {
            return -1;
        }
    }
    else {
        std::cout << "Process " << processName << " is not running.\n";
    }

    return 0;
}


void CreateFolder(const std::string& folderPath)
{
    try {
        // Create the directory
        std::filesystem::create_directory(folderPath);

        std::cout << "Directory created successfully.\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error creating directory: " << e.what() << "\n";
    }

}

int CreateFile(const std::string& fileName, std::ofstream& outFile)
{

    std::replace(CurrentPath.begin(), CurrentPath.end(), '\\', '/');
    outFile = std::ofstream(CurrentPath + fileName);

    if (!outFile.is_open()) {
        std::cerr << "Error creating " + fileName + " file." << std::endl;
        return -1;
    }

    return 0;
}

void CleanFolder(const std::string& folderPath)
{
    try
    {
        for (const auto& entry : std::filesystem::directory_iterator(folderPath))
        {
            std::filesystem::remove(entry);
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error cleaning folder: " << e.what() << std::endl;
    }
}

void RemoveDirectory(const std::string& dirPath)
{
    try {
        // Remove the directory
        std::filesystem::remove_all(dirPath);

        std::cout << "Directory deleted successfully.\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error deleting directory: " << e.what() << "\n";
    }
}

void RemoveFile(const std::string& filePath)
{
 
    // Attempt to delete the file
    if (std::remove(filePath.c_str()) != 0) {
        std::cerr << "Error deleting file.\n";
    }

    std::cout << "File deleted successfully.\n";
}

int ExtractAudiosFromCuttedVideos()
{
    const auto videosInfosVector = FFmpegWrapper->getVideosInfos();
    FFmpegWrapper->clearVideosInfos();
    unsigned int audioNumber = 0;
    for (const auto& entry : videosInfosVector)
    {
        auto videoName = entry.second.videoName;
        auto audioName = "audio_" + std::to_string(audioNumber) + ".wav";
        if (FFmpegWrapper->extractAudioFromVideo(entry.first, CurrentPath + Root + TemporaryFramesFolder + audioName) != 0)
        {
            return -1;
        }

        ffmpegWrapper::videoInfos vi;
        vi.videoName = entry.second.videoName;
        vi.audioName = audioName;
        FFmpegWrapper->setVideosInfos(entry.first, vi);

        ++audioNumber;
    }
    return 0;
}

int CreateListOfVideosName(const std::string& fileName)
{
    std::ofstream outFile;
    if (CreateFile(fileName, outFile) != 0)
    {
        return -1;
    }

    // Write each video name with 'file ' prefix to the file
    const auto videosInfosVector = FFmpegWrapper->getVideosInfos();
    for (const auto& entry : videosInfosVector) {
        outFile << "file '" << "subtitled_" + entry.second.videoName << "'" << std::endl;
    }

    // Close the file
    outFile.close();
    return 0;
}

int ConvertExtractedAudiosToText(const std::string& audioLanguage)
{
    std::string text;
    const auto videosInfosVector = FFmpegWrapper->getVideosInfos();
    FFmpegWrapper->clearVideosInfos();
    for (const auto& entry : videosInfosVector) {
        const auto audioPath = TemporaryFramesFolder + entry.second.audioName;
        if(SpeechRecognition->convertAudioToText(audioPath, audioLanguage, text) != 0)
        {
            return -1;
        }

        ffmpegWrapper::videoInfos vi;
        vi.videoName = entry.second.videoName;
        vi.audioName = entry.second.audioName;
        if (const auto pos = text.find("Transcription: "); pos != std::string::npos) {
            // If found, erase it
            text.erase(pos, std::string("Transcription: ").length());
        }
        vi.subtitle = text;
        FFmpegWrapper->setVideosInfos(entry.first, vi);
    }
    return 0;
}

std::map<std::string, std::string> WrappString(const std::string& str, const int maxLineWidth)
{
    //const int maxLineWidth = 12; // Choisissez la largeur maximale souhaitée
    std::istringstream iss(str);
    std::ostringstream wrappedSubtitle;
    std::string line;
    std::map<std::string, std::string>wrappeSubtitlesMap;
    int subtitleNumber = 1;
    while (std::getline(iss, line, ' '))
    {
        auto wrappedSubtitleLength = wrappedSubtitle.str().length();
        auto lineLength = line.length();
        if (wrappedSubtitle.str().length() + line.length() > maxLineWidth)
        {
            wrappeSubtitlesMap["subtitle_" + std::to_string(subtitleNumber)] = wrappedSubtitle.str();
            wrappedSubtitle.str("");
            ++subtitleNumber;
        }
        wrappedSubtitle << line << " ";
    }

    //Add the last subtitle to the map if it was not included
    if (!wrappedSubtitle.str().empty())
    {
        wrappeSubtitlesMap["subtitle_" + std::to_string(subtitleNumber)] = wrappedSubtitle.str();
    }

    return wrappeSubtitlesMap;
}

int AddSubtitleToCuttedVideos()
{
    //const std::string inputVideoPath = TemporaryFramesFolder + "video_3.mp4";
    const std::string fontSize = "50";
    const std::string fontColor = "white";
    const std::string backgroundColor = "blue";
    constexpr int x = -1; // Centered horizontally
    constexpr int y = -300; // 250 pixels from the bottom
    constexpr int box = 1; // 0: with background or 1: without background 
    constexpr double lambdaBoxColor = 0.6; //from 0 to 1
    constexpr int boxBorderW = 5; // border width
    //const std::string outputVideoPath = TemporaryFramesFolder + "subtitled_video_0.mp4";

    const auto videosInfosVector = FFmpegWrapper->getVideosInfos();
    for (const auto& entry : videosInfosVector)
    {
        auto subtitle = entry.second.subtitle;
        subtitle.erase(std::remove(subtitle.begin(), subtitle.end(), '\n'), subtitle.end());

        const auto subtitles = WrappString(subtitle, 30);

        if (FFmpegWrapper->addSubtitleToVideo(entry.first, subtitles, fontSize, fontColor, backgroundColor, x, y, box,
                                              lambdaBoxColor, boxBorderW, TemporaryFramesFolder + "subtitled_" + entry.second.videoName) != 0)
        {
            return -1;
        }

        RemoveFile(entry.first);
        RemoveFile(TemporaryFramesFolder + entry.second.audioName);
    }
    return 0;
}


int VideoTranslation(const std::string& videoPath, const std::string& language)
{
    std::replace(CurrentPath.begin(), CurrentPath.end(), '\\', '/');


    //Kill FFmpeg exe
    const std::string processName = "ffmpeg.exe";

    if(KillProcessByName(processName) != 0)
    {
        return -1;
    }
    Sleep(1000);

    //Remove the temporary folder
    RemoveDirectory(TemporaryFramesFolder);
    Sleep(1000);

    //Create a temporary folder
    CreateFolder(TemporaryFramesFolder);

    //Get video infos
    openCvWrapper::VideoFrameInfos videoFrameInfos{};
    if(OpencvWrapper->getVideoFrameInfo(videoPath, videoFrameInfos) != 0)
    {
        return -1;
    }

    //Cut video
    if(FFmpegWrapper->videoCut(videoPath, TemporaryFramesFolder, videoFrameInfos.fps, 
                               videoFrameInfos.totalFrames, 5) != 0)
    {
        return -1;
    }

    //Extract audios from cutted videos
    if(ExtractAudiosFromCuttedVideos() != 0)
    {
        return -1;
    }

    //Convert audios to text
    if(ConvertExtractedAudiosToText(language) != 0)
    {
        return -1;
    }

    //Add subtitle to cutted videos
    if(AddSubtitleToCuttedVideos() != 0)
    {
        return -1;
    }


    //Create a file named list.txt
    if (CreateListOfVideosName(Root + TemporaryFramesFolder + ListFile) != 0)
    {
        return -1;
    }

    //Concat videos
    CleanFolder("final_version/");
    if(FFmpegWrapper->concatVideos(TemporaryFramesFolder + "list.txt", "final_version/final_version.mp4") != 0)
    {
        return -1;
    }

    //Remove the temporary folder
    RemoveDirectory(TemporaryFramesFolder);

    return 0;
}


int main() {
    // UNC path to the directory
    const std::string uncPath = "\\\\192.168.10.1\\c\\myApps\\LinguaVista\\Debug";
    //Hide console window
    ShowWindow(GetConsoleWindow(), SW_SHOW);
    if(VideoTranslation("french_video/frenchDialogue.mp4", "fr-FR") != 0)
    {
        throw std::runtime_error("Tanslation failed !");
    }

    return 0;
}