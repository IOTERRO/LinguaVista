#pragma once

#ifdef MEDIA_PROCESSING_EXPORTS
#define MEDIA_PROCESSING_API __declspec(dllexport)
#else
#define MEDIA_PROCESSING_API __declspec(dllimport)
#endif