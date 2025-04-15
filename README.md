# Image processing educational app

## Table of contents

1. [Description](#description)
2. [How algorithms effects are presented](#how-algorithms-effects-are-presented)
3. [How to run](#how-to-run)


## Description

This is an app for educational purpouses to help students visualize image processing algorithms effects on pictures and let them focus on implementing the algorithms, having some template app already prepared.

The app is there to show you comparison between the source picture and output picture along with their histograms, for which you can choose which color component from RGB histogram you want to see. It also allows for loading 2 source images for algorithms like alpha blending. There are also some custom GUI elements, useful for inputting some algorithms parameters and designed to be more intuitive to users from image processing field.

The app contains a few examplary functions from OpenCV, but overall it is a template to put some more material into it. All people interested in learning OpenCV are welcome to try it out!


## How algorithms effects are presented

Main window of the app shows comparison of source picture with the output one, along with their histograms:

![main window](https://drive.google.com/uc?export=view&id=1iUir2-Oymhe7DObpZIGCQBq7UY_HhoXg)

It is also possible to read in 2 source pictures, e.g. for alpha blending. Then another window with second source image is showed:

![2 source pictures](https://drive.google.com/uc?export=view&id=1Jm5H6rL4qjDDFR7x12BjrdHcXQRaHnEl)

An example of the custom input elements, designed to be convenient for users from image processing field, is the structuring matrix form, which can be used for getting parameters for algorithms like dilation, erosion etc.:

![structuring matrix](https://drive.google.com/uc?export=view&id=1LXTFTeEGNQ3axgiSPPOgTS06jIM86_MW)


## How to run

To just see how the app works, you can run the latest release of the app that can be accessed [here](./../../releases).

To develop the template, first you need to install:
- Visual Studio (2019 or 2022), with min. C++17
- Qt framework (in version 6.x as used in the app) and Qt VS Tools plugin in Visual Studio
- OpenCV (in version 4.x)

After that, all that's left to do is to update paths in project `Properties`, to point to your OpenCV and Qt installation paths. To achieve that:
1. You need to download the repository code and open the `.sln` project in VS 2022 (project version for VS 2019 is on `vs2019-version` branch).
2. Adding Qt paths - click on the `Extensions` option in Visual Studio, where you should see your Qt VS Tools plugin - choose it and there will open a window asking for the Qt installation path. The path should point to `qmake.exe` file - it will look like `%QT_INSTALLATION_PATH%\%QT_VERSION%\msvc2019_64\bin`.
3. Adding OpenCV paths:
    - Open options: `Project > Properties > C/C++ > General > Additional include directories` and add path to your `%OPENCV_INSTALLATION_PATH%\opencv\build\include`.
	- Open options: `Project > Properties > Linker > General > Additional library directories` and add path to your `%OPENCV_INSTALLATION_PATH%\opencv\build\x64\vc15\lib`
	- Open options: `Project > Properties > Linker > Input > Additional dependencies` and add file name of OpenCV `.lib` file, that is located in the previously configured path - the name should look like `opencv_world460.lib` for release configuration or `opencv_world460d.lib` (letter d at the end added) for debug.
	
At this point, you should be able to run the project in Visual Studio by pressing F5.