#ifndef __version__
#define __version__

#define MAJOR_VERSION_STR "1"
#define MAJOR_VERSION_INT 1

#define SUB_VERSION_STR "0"
#define SUB_VERSION_INT 0

#define RELEASE_NUMBER_STR "0"
#define RELEASE_NUMBER_INT 0

#define BUILD_NUMBER_STR "1"
#define BUILD_NUMBER_INT 1

#define FULL_VERSION_STR "1.0.0"
#define VERSION_STR  FULL_VERSION_STR

#define stringPluginName "residler"
#define stringOriginalFilename stringPluginName".vst3"

#if PLATFORM_64
#define stringFileDescription	"VST3 version of residler (64Bit)"
#else
#define stringFileDescription	"VST3 version of residler"
#endif
#define stringCompanyName		"The Finnish Open Source Crew"
#define stringLegalCopyright	"© 2013 The Finnish Open Source Crew"
#define stringLegalTrademarks	"VST is a trademark of Steinberg Media Technologies GmbH"

#endif //__version__
