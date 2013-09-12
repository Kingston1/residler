residler
========

VST synthesizer plugin using reSID software emulation of the [MOS6581 SID](http://en.wikipedia.org/wiki/MOS_Technology_SID "MOS Technology SID") chip

Currently supported plugin formats:

VST2.4
- win32
- x64


VST3
- win32
- x64

There will be OSX VST and AU support in the future.

		
Build Instructions for Microsoft Visual C++ 2010
------------------------------------------------


1. Install Windows 7.1 SDK in order to build x64 targets
2. Download and extract latest Steinberg VST3 SDK (current version used in development: vstsdk352_25_09_2012_build_16)
3. Clone residler git repository in the same directory with \public.sdk and \vstgui4
4. Open \VST3 SDK\residler\win\residler.sln
5. Visual C++ residler project Property Pages info

- Configuration Properties -> General -> Target Extension. You can set the output file to .dll or .vst3 here to select plugin format. Or you can just rename the file after building.
- Configuration Properties -> General -> Platform Toolset. Make sure this is set to Windows7.1SDK.
- Configuration Properties -> Linker -> General -> Output File. Choose your plugin target folder here. For example C:\Program Files\Common Files\VST3 or C:\VstPlugins

