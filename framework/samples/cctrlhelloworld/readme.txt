-----------------------------------------------------------
---
---	CCtrlHelloWorld
---
-----------------------------------------------------------

1. Requirements

You need the C-Framework libraries to build the project.
Unzip the libraries and this sample in the same folder.



2. How to build?

Just open the project file with Visual Studio 2008 
(2005 may work, but wasn't tested) and build the project.

You can find the project file in this package under the
path 'projects/visualc/cframework/cctrlhelloworld'.



3. What to do if the sample crashes on start up or shows
   only a black screen?

Check out first if the 'Work Directory' is set correctly.
Unfortunately Visual Studio sometimes clears this field in
the settings of a project.

Right click on the project and choose 'Properties'. In the
window that shows up choose the section 'Debugging' under
'Configuration' on the left side.

Check if there's a value in the field 'Work Directory'. If
not, type in '$(OutDir)/build' there.

If the sample still crashes please ask for help in the forum
at http://cframework.oeing.eu