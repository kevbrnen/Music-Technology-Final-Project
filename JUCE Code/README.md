# JUCE Code
This folder contains all code for my Effects Suite Plug-in.

pictures of plugin

### Structure
* "NewProject.jucer" Is the JUCE project file
* "Source" Contains all source code used for the Plug-in
* "JuceLibraryCode" Contains some JUCE Modules used in my project
* "Builds" Contains some files related to the XCode build of the VST3 plug-in

### Installation/Build information
#### Installing The Plug-in
The .vst3 files are located [here](./Plug-in%20builds/) for Windows and MacOS. You can use a website like [this](https://download-directory.github.io/) for downloading directly from the repository, just copy the URL of the file you want to download (i.e. the Mac/Windows folder in Plug-in Builds). The plug-in was built using XCode on an Intel Mac. The Windows build was created using JUCE on a Windows virtual machine and Visual Studio 2022.

#### Building The Plug-in
In order to build the plug-in yourself. First make sure you have [XCode](https://developer.apple.com/xcode/) (for Mac) or [Visual Studio](https://visualstudio.microsoft.com/) (for Windows). Also make sure to install [JUCE](https://juce.com/get-juce/) for your desired platform and follow [this](https://docs.juce.com/master/tutorial_new_projucer_project.html) tutorial to get everything up and running.
You can download the [NewProject.jucer](./NewProject/NewProject.jucer) file, along with all of the [source code](./NewProject/Source). 
* Open the jucer file using the Projucer. 
* Make sure the source code paths in the Projucer are correct for wherever you downloaded them to.
* Make sure to have the below modules selected.
<img width="361" alt="Screenshot 2024-05-09 at 10 17 07" src="https://github.com/kevbrnen/Music-Technology-Final-Project/assets/147145956/dac93a21-889b-4600-bee4-9d5d01fe3e6c">

* Choose your selected exporter from the dropdown menu and press the icon to open the IDE.
* Once the IDE is open, you can build the VST3. 
* Once the build is complete the plug-in should show up in the products/builds folder.

### Using the Plug-in
Once you have the .vst3 file for your platform, you can use it in any DAW you wish. You can either link your DAW directly to wherever the plug-in is located on your system (Usually under Plug-Ins in preferences/settings in your DAW). Alternatively you can place the .vst3 file in your systems VST folder. Then rescan for plug-ins in your DAW and it should show up.
- On Windows:
    "C:\Program Files\Common Files\VST3"
- On Mac:
    "/Library/Audio/Plug-Ins/VST3"

