# COMP3015
This is the repo readme for coursework 2 of COMP3015 

## Version and operating system
For this project, I used visual studio 2019 version 16.10.0 and windows 10 version 20H2.

## About the project
This product is a multiple shader implementation with customisable features and object manipulation. I got the idea for this project while searching for models to use for another project, having to import them into unity to see if the model works and how it interacts with shadows created by unity’s lights can take a while especially if there are file extension issues. This program was created to help speed up the process while also being able to manipulate the object in the scene on top of viewing it with shaders.
I believe that I have achieved my goal in creating a program that uses multiple shaders and allows for user interaction with objects (being able to change translation, rotation, and scale as well as loading different models). If I were able to go back and change anything in the project, I would allow for the user to add objects into a folder and the project automatically add it to an array which would then allow them to load it into the scene. Also, when adding the use of shadows into the program, it caused issues with textures and normal maps and so I had to disable them. With better time allocation I would have tried to find a fix for this allowing the use of both textures and normal maps as well as shadows.

![Screenshot1](https://i.imgur.com/ufdsOoU.png)

## How to use the software?

**Run it with .exe**
* Navigate into the "Project_Template" folder 
* Open the "x64" folder 
* Find and open the "Release" folder
* Locate the "Project_Template.exe" file. Run the exe

**Run it in Visual Studio**
* Navigate into the "Project_Template" folder 
* Double click the "Project_Template.sln" file
* Once visual studio is open, select debug and run the program

Once opened, the program is very easy to follow, there are 3 collapsible windows located at the top of the program, they each control an aspect of the model and the ability to manipulate it. They are labelled "Position Object" which allows you to change the position of the object and the x,y and z coordinates. "Scale Object" lets you change the scale of the current object rendered in the scene. "Rotate Object" lets you rotate the object on the x,y and z axis. There is also a button on this window that will reset the rotation of the object to 0 and each of the axes. Finally, located in the bottom right is a debug window that has a name of an object (3 max) currently displayed. This is the object that is currently loaded into the scene, in order to change to another object, simply select another of the choices available to you (currently Plane, Chair and Car).

To control any one of these simply move your cursor and left click on the sliders and drag your mouse to the left to reduce the value and to the right to increase it.

## How does it work?
  The code runs through a loop which renders the scene and all of the objects in it. In the process of rendering the scene, it applies both the shaders that I have designed for this scene. In this case it is shadows and Blinn phong shader. Each of the methods do one function allowing for efficiency of the program. As well as this, the methods are evenly spaced to make sure that that along with the comments, the code is easy to read and understand where aspects of it end and others begin.

## Link to developer walkthrough and images

[**YouTube video link of developer walkthrough**](https://www.youtube.com/watch?v=J2wI_Vbnm0g)
