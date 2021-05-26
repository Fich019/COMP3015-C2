#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include "helper/scene.h"

#include "IMGUI/imgui.h"
#include "IMGUI/imgui_impl_opengl3.h"
#include "IMGUI/imgui_impl_glfw.h"
//#include "helper/stb/stb_image.h"

#include "Additional Files/plane.h"
#include "Additional Files/objmesh.h"
#include <glad/glad.h>
#include "helper/glslprogram.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Additional Files/texture.h"
#include "helper/frustum.h"
#include "helper/teapot.h"
#include "helper/torus.h"

class SceneBasic_Uniform : public Scene
{
private:
    Plane plane;                        //Plane surface

    int shadowMapWidth, shadowMapHeight;

    std::unique_ptr<ObjMesh> obj1;      //Object1
    std::unique_ptr<ObjMesh> obj2;      //Object1
    std::unique_ptr<ObjMesh> obj3;      //Object1

    std::unique_ptr<ObjMesh> BackGround;      //Object
    Frustum lightFrustum;
    GLSLProgram prog;
    float tPrev;
    float angle;

    float objPos[3] = { 0.0f, 3.0f, 0.0f };
    float objRot[3] = { 0.0f, 0.0f, 0.0f };
    float objScale[3] = { 1.0f, 1.0f, 1.0f };

    
    int Selecteditem = 0;
    const char* items[3] = { "Plane", "Chair", "Car" };

    GLuint shadowFBO, pass1Index, pass2Index;
    glm::mat4 rotationMatrix, shadowBias, lightPV;

    Teapot teapot;
    Torus torus;

    //Frustum lightFrustum;

    void compile();
    void setMatrices();

public:
    SceneBasic_Uniform();

    void LoadOBJ();
    void GUIMenu();
    void RotateOBJ();
    void ScaleOBJ();

    void setupFBO();
    void initScene();
    void update( float t );
    void render();
    void resize(int, int);
    void draw();
};

#endif // SCENEBASIC_UNIFORM_H
