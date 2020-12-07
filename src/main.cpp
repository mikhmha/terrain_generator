#pragma once
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <texture.h>
#include <shader.h>
#include <Camera.hpp>
#include <terrain_gen.h>
#include <terrain_mesh.h>
#include <terrain_model.hpp>

//*****SCENE OBJECTS******/
#define TERRAIN_GEN
#define LIGHTCUBE
//#define SKYBOX
//*****END SCENE OBJECTS******/

//*********SCREEN RESOLUTION SETTINGS********
constexpr int WINDOW_WIDTH = 2560;
constexpr int WINDOW_HEIGHT = 1440;
#define RENDER_FULLSCREEN				// IF NOT DEFINED, WINDOW MODE
//***END SCREEN RESOLOUTION SETTINGS***

//******TERRAIN GENERATION SETTINGS********
float tess_factor = 5.0;				// INITIAL TESSELATION FACTOR. CAN BE INCREASED/DECREASED USING ARROW KEYS
constexpr int n = 400;					// n * n bezier surfaces are used to construct the terrain, higher n = more detailed terrain
// PERLIN NOISE GENERATION FACTORS
constexpr std::int32_t seed = 45;		// GENERATION SEED																							seed		[0 .. 2^32-1]
constexpr std::int32_t octaves = 8;		//																											octaves		[1 .. 8 .. 16]
constexpr float frequency = 3.0;		// HIGHER FREQUENCY = MORE FREQUENT and higher PEAKS														frequency	[0.1 .. 64.0]
// SCALE FACTORS
auto s = 5000.0f;
auto x_s = s * 1.0f;
auto y_s = s * 1.0f;
auto z_s = s * 1.0f;
//****END TERRAIN GENERATION SETTINGS*****


//******CAMERA SETTINGS********
auto pos = glm::vec3(0.0f, 1.30 * y_s, 1.0 * z_s);	// INITIAL CAMERA POSITION
auto speed = s * 0.15f;	// camera speed
Camera camera(pos, speed);
auto draw_distance = s * 5.0f;
//****END CAMERA SETTINGS*****













// deltaTime
float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

// cursor
bool firstMouse = true;
float lastX = WINDOW_WIDTH / 2.0f;
float lastY = WINDOW_HEIGHT / 2.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void logInput(GLFWwindow* window);
void gen_cubeBuffers(unsigned int& vao, unsigned int& vbo);





int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 8);

#ifndef RENDER_FULLSCREEN
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);
#elif defined RENDER_FULLSCREEN 
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", glfwGetPrimaryMonitor(), NULL);
#endif 
	

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_CULL_FACE);
	glPatchParameteri(GL_PATCH_VERTICES, 16);



#ifdef  TERRAIN_GEN
	auto terrain_surfaces = gen_Terrain(n, seed, octaves, frequency);
	std::vector<Texture> terrain_textures
	{
		loadMatTexture("resources/terrain_textures/grass_norm.jpg", "texture_normal", false),
		loadMatTexture("resources/terrain_textures/mountain_disp.jpg", "texture_displacement", false)
	};

	std::vector<TerrainMesh> tmesh;
	for (const auto& bsurface : terrain_surfaces)
		tmesh.emplace_back(bsurface, terrain_textures);

	auto terrain = TerrainModel(std::move(tmesh));
	terrain.setScale(glm::vec3(x_s, y_s, z_s));


	Shader tesselationShader("shaders/bezier_tesselation/btess.vs", "shaders/bezier_tesselation/btess.tesc", "shaders/bezier_tesselation/btess.tese", "shaders/bezier_tesselation/btess.fs");
	tesselationShader.use();
	tesselationShader.setFloat("tess_factor", tess_factor);
	tesselationShader.setFloat("disp_factor", 0.90);

#endif



#ifdef LIGHTCUBE
	unsigned int lightcubeVAO, lightcubeVBO;
	gen_cubeBuffers(lightcubeVAO, lightcubeVBO);
	auto lightScale = glm::vec3(s * 0.05f, s * 0.05f, s * 0.05f);
	auto lightPos = glm::vec3(0.0f, 1.25 * y_s, -0.5 * z_s);
	Shader lightCubeShader("shaders/light/light.vs", "shaders/light/light.fs");
#endif

#ifdef SKYBOX

	unsigned int skyboxVAO, skyboxVBO;
	gen_cubeBuffers(skyboxVAO, skyboxVBO);


	std::vector<std::string> faces
	{
		"resources/skybox/right.jpg",
		"resources/skybox/left.jpg",
		"resources/skybox/top.jpg",
		"resources/skybox/bottom.jpg",
		"resources/skybox/front.jpg",
		"resources/skybox/back.jpg",
	};
	auto skyboxTexture = loadCubemapTexture(faces);
	Shader skyboxShader("shaders/skybox/skybox.vs", "shaders/skybox/skybox.fs");
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

#endif


	while (!glfwWindowShouldClose(window))
	{

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		processInput(window);


		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
 
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, draw_distance);
		glm::mat4 view = camera.GetViewMatrix();

#ifdef	TERRAIN_GEN
		tesselationShader.use();
		tesselationShader.setFloat("tess_factor", tess_factor);
		tesselationShader.setVec3("lightPos", lightPos);
		tesselationShader.setVec3("viewPos", camera.Position);
		tesselationShader.setMat4("view", view);
		tesselationShader.setMat4("projection", projection);
		terrain.Draw(tesselationShader);
#endif

#ifdef LIGHTCUBE
		constexpr glm::mat4 drawMat(1.0f);
		//auto t = 0.50f * glfwGetTime();
		//auto lightX = (x_s * 1.50) * sin(t);
		//auto lightZ = (z_s * 1.50) * cos(t);
		//lightPos = glm::vec3(lightX, 0.75 * y_s, lightZ);
		auto worldMat = glm::translate(drawMat, lightPos);
		worldMat = glm::scale(worldMat, lightScale);

		lightCubeShader.use();
		lightCubeShader.setMat4("projection", projection);
		lightCubeShader.setMat4("view", view);
		lightCubeShader.setMat4("model", worldMat);
		glBindVertexArray(lightcubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
#endif


#ifdef SKYBOX
		view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
		glDepthFunc(GL_LEQUAL); 
		skyboxShader.use();
		skyboxShader.setMat4("view", view);
		skyboxShader.setMat4("projection", projection);
		glBindVertexArray(skyboxVAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);
#endif



		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}






void gen_cubeBuffers(unsigned int& vao, unsigned int& vbo)
{
	constexpr float cubeVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};
	// maybe use EBO representation in the future

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

}


/* CALLBACK FUNCTIONS*/

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);



	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		tess_factor++;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		tess_factor--;
		if (tess_factor < 0)
			tess_factor = 0.0;
	}

	logInput(window);


}


void logInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		std::clog << "TESSELATION FACTOR: " << tess_factor << '\n';
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

/* END CALLBACK FUNCTIONS*/



