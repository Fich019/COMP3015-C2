#include "scenebasic_uniform.h"

#include <cstdio>
#include <cstdlib>

#include <string>
using std::string;


#include <iostream>
using std::cerr;
using std::endl;

#include <glm/gtc/matrix_transform.hpp>
using glm::vec3;
using glm::vec4;
using glm::mat4;


SceneBasic_Uniform::SceneBasic_Uniform() : tPrev(0), shadowMapWidth(2048), shadowMapHeight(2048),
teapot(14, mat4(1.0f)), plane(40.0f, 40.0f, 2, 2), torus(0.7f * 2.0f, 0.3f * 2.0f, 50, 50)
{
    obj1 = ObjMesh::load("media/FA-18F.obj");
    obj2 = ObjMesh::load("media/Chair.obj");
    obj3 = ObjMesh::load("media/CAR.obj");

    BackGround = ObjMesh::load("media/PlaneObj.obj");
}


void SceneBasic_Uniform::initScene()
{
    compile();

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glEnable(GL_DEPTH_TEST);

        angle = glm::quarter_pi<float>();
    setupFBO();

    GLuint programHandle = prog.getHandle();
    pass1Index = glGetSubroutineIndex(programHandle, GL_FRAGMENT_SHADER, "recordDepth");
    pass2Index = glGetSubroutineIndex(programHandle, GL_FRAGMENT_SHADER, "shaderWithShadows");

    shadowBias = mat4(vec4(0.5f, 0.0f, 0.0f, 0.0f),
        vec4(0.0f, 0.5f, 0.0f, 0.0f),
        vec4(0.0f, 0.0f, 0.5f, 0.0f),
        vec4(0.5f, 0.5f, 0.5f, 1.0f));

    float c = 1.65f;
    vec3 lightpos = vec3(0.0f, c * 5.25, c * 7.5f);
    lightFrustum.orient(lightpos, vec3(0.0f), vec3(0.0f, 1.0f, 0.0f));
    lightFrustum.setPerspective(150.0f, 1.0f, 1.0f, 100.0f); //Sets perspective of what shadows can reach, bigger more area that can be shaded.
    lightPV = shadowBias * lightFrustum.getProjectionMatrix() * lightFrustum.getViewMatrix();

    prog.setUniform("Light.Intensity", vec3(0.85f));
    prog.setUniform("ShadowMap", 0);                // THIS IS INSIDE TEXTURE SLOT 0

    //Normal Map and Texure in slot 1 & 2

    //GLuint texID = Texture::loadTexture("../Project_Template/media/texture/ogre_diffuse.png");

    //GLuint moss = Texture::loadTexture("../Project_Template/media/texture/ogre_normalmap.png");

    //glActiveTexture(GL_TEXTURE1);
    //glBindTexture(GL_TEXTURE_2D, texID);

    //glActiveTexture(GL_TEXTURE2);
    //glBindTexture(GL_TEXTURE_2D, moss);
}


void SceneBasic_Uniform::compile()
{
    try {
        prog.compileShader("shader/blinn-phong.vert");
        prog.compileShader("shader/blinn-phong.frag");

        prog.link();
        prog.use();

    }
    catch (GLSLProgramException& e)
    {
        cerr << e.what() << endl;
        exit(EXIT_FAILURE);
    }
}


void SceneBasic_Uniform::update(float t)
{
    float deltaT = t - tPrev;
    if (tPrev == 0.0f) deltaT = 0.0f;
    tPrev = t;

    angle += 0.2f * deltaT;
    if (angle > glm::two_pi<float>()) angle -= glm::two_pi<float>();
}


void SceneBasic_Uniform::render()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    prog.use();

    view = lightFrustum.getViewMatrix();
    projection = lightFrustum.getProjectionMatrix();

    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, shadowMapWidth, shadowMapHeight);
    glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &pass1Index);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(2.5f, 10.0f);

    draw();

    glCullFace(GL_BACK);
    glFlush();

    float c = 2.0f;
    vec3 cameraPos(c * 11.5f * cos(90), c * 7.0f, c * 11.5f * sin(90));
    view = glm::lookAt(cameraPos, vec3(0.0f), vec3(0.0f, 1.0f, 0.0f));
    prog.setUniform("Light.Position", view * vec4(lightFrustum.getOrigin(), 1.0f));
    projection = glm::perspective(glm::radians(50.0f), (float)width / height, 0.1f, 100.0f);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, width, height);
    glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &pass2Index);
    draw();

    GUIMenu();
    LoadOBJ();
    RotateOBJ();
    ScaleOBJ();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


//Creates the window box for the position of the object in the scene.
//Gets passed a variable called objpos which is then updated and used
//when rendering the scene.
void SceneBasic_Uniform::GUIMenu()
{
    {
        ImGui::Begin("Position Object");
        ImGui::SliderFloat3("Position", objPos, -10.0f, 10.0f);
        ImGui::End();
    }

}


//Creates a window box for scale slider that changes the scale of the
//object in the scene. Gets passed a variable called objScale which
//is used when rendering the scene allowing it to update live.
void SceneBasic_Uniform::ScaleOBJ()
{
    {
        ImGui::Begin("Scale Object");
        ImGui::SliderFloat("Scale", objScale, 0.0f, 3.0f);
        ImGui::End();
    }

}


//Same as position imgui object but for rotation of the 
//object instead. This method also has a button that sets 
//all of the 3 floats in the vec3 objRot to 0 effectively 
//resetting the rotation.
void SceneBasic_Uniform::RotateOBJ(){

    {
        ImGui::Begin("Rotate Object");
        ImGui::SliderFloat3("Rotation", objRot, -5.0f, 5.0f);

        if (ImGui::Button("Reset")){
            objRot[0] = 0;
            objRot[1] = 0;
            objRot[2] = 0;
        }
        ImGui::End();
    }

}


//Creates a combo box for the objects that can be loaded
//into the scene. Makes use of a variable called Selected
//items. This is updated depending on what is selecetd in 
//the combo box. The number is passed through to the draw
//method and directly effects which obj is loaded depending
//on the number.
void SceneBasic_Uniform::LoadOBJ()
{
    {
        ImGui::Combo("Object loaded", &Selecteditem, items, IM_ARRAYSIZE(items));
    }
}


void SceneBasic_Uniform::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));

    prog.setUniform("MVP", projection * mv);
    prog.setUniform("ShadowMatrix", lightPV * model);
}


void SceneBasic_Uniform::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
}


void SceneBasic_Uniform::draw()
{
    vec3 pos = vec3(objPos[0], objPos[1], objPos[2]);

    //Pass 1
    //Renders the main object in the scene.
    prog.setUniform("Pass", 1);
    vec3 color = vec3(0.2f, 0.5f, 0.9f);
    prog.setUniform("Material.Ka", color * 0.05f);
    prog.setUniform("Material.Kd", color);
    prog.setUniform("Material.Ks", vec3(0.9f, 0.9f, 0.9f));
    prog.setUniform("Material.Shininess", 150.0f);
    model = mat4(1.0f);

    model = glm::translate(model,pos);
    model = glm::rotate(model, glm::radians(objRot[0] * 60.0f), vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(objRot[1] * 60.0f), vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(objRot[2] * 60.0f), vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model,vec3(objScale[0]));
    setMatrices();
    if (Selecteditem == 0) {
        obj1->render();
    }
    else if (Selecteditem == 1) { //if else if statement that loops through objects that can 
        obj2->render();           //be loaded into the scene.
    }
    else if (Selecteditem == 2) {
        obj3->render();
    }


    //Pass 0
    //Renders the background objects.
    prog.setUniform("Pass", 0);
    prog.setUniform("Material.Kd", 0.25f, 0.25f, 0.25f);
    prog.setUniform("Material.Ks", 0.0f, 0.0f, 0.0f);
    prog.setUniform("Material.Ka", 0.05f, 0.05f, 0.05f);
    prog.setUniform("Material.Shininess", 1.0f);
    model = mat4(1.0f);
    model = glm::scale(model, vec3(3.0f));
    setMatrices();
    BackGround->render();
    model = mat4(1.0f);
    model = glm::translate(model, vec3(-13.0f, 5.0f, 0.0f));
    model = glm::rotate(model, glm::radians(-90.0f), vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, vec3(3.0f));
    setMatrices();
    BackGround->render();
    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f, 5.0f, -5.0f));
    model = glm::rotate(model, glm::radians(90.0f), vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, vec3(3.0f));
    setMatrices();
    BackGround->render();
    model = mat4(1.0f);
}


//Shadow 
void SceneBasic_Uniform::setupFBO()
{
    GLfloat border[] = { 1.0f, 0.0f,0.0f,0.0f };

    GLuint depthTex;
    glGenTextures(1, &depthTex);
    glBindTexture(GL_TEXTURE_2D, depthTex);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT24, shadowMapWidth, shadowMapHeight);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthTex);


    glGenFramebuffers(1, &shadowFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
        GL_TEXTURE_2D, depthTex, 0);

    GLenum drawBuffers[] = { GL_NONE };
    glDrawBuffers(1, drawBuffers);

    GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (result == GL_FRAMEBUFFER_COMPLETE) {
        printf("Framebuffer is complete.\n");
    }
    else {
        printf("Framebuffer is not complete.\n");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}
