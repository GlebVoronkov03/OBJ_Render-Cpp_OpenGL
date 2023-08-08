#include"libs.h"

Vertex vertices[] =
{
	//Position                      //Color                     //Texcoords				//Normal
	glm::vec3(-0.5f, 0.5f, 0.f),	glm::vec3(1.f, 0.f, 0.f),	glm::vec2(0.f, 1.f),	glm::vec3(0.f, 0.f, -1.f),
	glm::vec3(-0.5f, -0.5f, 0.f),	glm::vec3(0.f, 1.f, 0.f),	glm::vec2(0.f, 0.f),	glm::vec3(0.f, 0.f, -1.f),
	glm::vec3(0.5f, -0.5f, 0.f),	glm::vec3(0.f, 0.f, 1.f),	glm::vec2(1.f, 0.f),	glm::vec3(0.f, 0.f, -1.f),
	glm::vec3(0.5f, 0.5f, 0.f),		glm::vec3(1.f, 1.f, 0.f),	glm::vec2(1.f, 1.f),	glm::vec3(0.f, 0.f, -1.f)
};
unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

GLuint indices[] =
{
	0, 1, 2,	//Triangle 1:. (bottom left)
	0, 2, 3		//Triangle 2': (upper right)
};
unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

//operation of the Esc button - closing the window
void updateInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

void framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH)
{
	glViewport(0, 0, fbW, fbH);
}

//moving an object via WSAD buttons
void updateInit(GLFWwindow* window, glm::vec3& position, glm::vec3& rotation, glm::vec3& scale)
{
	//moving forward (away from the camera) via the W button
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		position.z -= 0.01f;
	}
	//moving back (closer to the camera) via the S button
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		position.z += 0.01f;
	}
	//move left via button A
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		position.x -= 0.01f;
	}
	//move right via D button
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		position.x += 0.01f;
	}
	//turn clockwise via Q button
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		rotation.y -= 0.1f;
	}
	//turn counterclockwise via button E
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		rotation.y += 0.1f;
	}
	//increase via Z button
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
	{
		scale += 0.1f;
	}
	//decrease via X button
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
	{
		scale -= 0.1f;
	}
}

int main()
{
	//INIT (initialization) GLFW
	glfwInit();

	//window creation
	const int WINDOW_WIDTH = 640;
	const int WINDOW_HEIGHT = 480;
	int framebufferWidth = 0;
	int framebufferHeight = 0;

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //for MACOS

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OBJ_RENDER", NULL, NULL);
	
	//Important for perspective matrix!
	glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
	glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);
	
	//Important for displaying the render window!
	glfwMakeContextCurrent(window);

	//INIT (initialization) GLEW (need window and OPENGL context)
	glewExperimental = GL_TRUE;

	//Error
	if (glewInit() != GLEW_OK)
	{
		std::cout << "ERROR::MAIN.CPP::GLEW_INIT_FAILED" << "\n";
		glfwTerminate();
	}

	//OPENGL options
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);//opposite side of the triangle - clockwise
	glFrontFace(GL_CCW);//front side of the triangle - counterclockwise

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//INIT (initialization) SHADER
	Shader core_program("vertex_core.glsl", "fragment_core.glsl");
	
	//Model Mesh


	//VAO, VBO, EBO - buffers
	
	//VAO generation and linking
	GLuint VAO;
	glCreateVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//VBO generation (vertex buffer), linking and sending data
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//GL_STATIC_DRAW - if drawing is rare
	//GL_DYNAMIC_DRAW - if drawing occurs frequently

	//EBO generation (actual vertex indexing), linking and sending data
	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//set of vertex attributes and Enable
	//Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
	glEnableVertexAttribArray(1);

	//Texcoord
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
	glEnableVertexAttribArray(2);
	
	//Normal
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(3);

	//linking VAO to 0
	glBindVertexArray(0);

	//Texture INIT

	//Texture 0 is passed to the zero input of the video card

	Texture texture0("Images/teapot.png", GL_TEXTURE_2D, 0); 
	 

	//Texture 1 is passed to the first input of the video card
	Texture texture1("Images/table.png", GL_TEXTURE_2D, 1);

	//INIT Matrices
	glm::vec3 position(0.f);
	glm::vec3 rotation(0.f);
	glm::vec3 scale(1.f);

	glm::mat4 ModelMatrix(1.f);
	ModelMatrix = glm::translate(ModelMatrix, position);
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f));
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));
	ModelMatrix = glm::scale(ModelMatrix, scale);

	glm::vec3 camPosition(0.f, 0.f, 1.f);//положение камеры
	glm::vec3 worldUp(0.f, 1.f, 0.f);
	glm::vec3 camFront(0.f, 0.f, -1.f);
	glm::mat4 ViewMatrix(1.f);
	ViewMatrix = glm::lookAt(camPosition, camPosition + camFront, worldUp);

	//Perspective projection matrix
	float fov = 90.f;
	float nearPlane = 0.1f;
	float farPlane = 100.f;//if good "iron" can be increased to 1000
	glm::mat4 ProjectionMatrix(1.f);

	ProjectionMatrix = glm::perspective(
		glm::radians(fov), 
		static_cast<float>(framebufferWidth) / framebufferHeight,
		nearPlane,
		farPlane
	);

	//Light
	glm::vec3 lightPos0(0.f, 0.f, 1.f);//navigation "light bulbs" 0.f, 0.f, 0.12f for validation
	
	//INIT Uniforms
	core_program.setMat4fv(ModelMatrix, "ModelMatrix");
	core_program.setMat4fv(ViewMatrix, "ViewMatrix");
	core_program.setMat4fv(ProjectionMatrix, "ProjectionMatrix");
	
	core_program.setVec3f(lightPos0, "lightPos0");
	core_program.setVec3f(camPosition, "cameraPos");
	
	//main loop
	while (!glfwWindowShouldClose(window))
	{
		//INPUT UPDATES ---
		glfwPollEvents();
		updateInit(window, position, rotation, scale);

		//UPDATE ---
		updateInput(window);

		//DRAWING ---

		//cleaning
		glClearColor(0.f, 0.f, 0.f, 1.f);//цвет фона окна
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
		//Update uniforms
		core_program.set1i(texture0.getTextureUnit(), "texture0");
		core_program.set1i(texture1.getTextureUnit(), "texture1");
		
		ModelMatrix = glm::mat4(1.f);
		ModelMatrix = glm::translate(ModelMatrix, position);
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f));
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));
		ModelMatrix = glm::scale(ModelMatrix, scale);
		 
		core_program.setMat4fv(ModelMatrix, "ModelMatrix");
		
		glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
		
		ProjectionMatrix = glm::perspective(
			glm::radians(fov),
			static_cast<float>(framebufferWidth) / framebufferHeight,
			nearPlane,
			farPlane
		);
		core_program.setMat4fv(ProjectionMatrix, "ProjectionMatrix");

		//Using the program
		core_program.use();


		//active texture
		texture0.bind();
		texture1.bind();

		//Vertex array binding (nested in object)
		glBindVertexArray(VAO);

		//Vertex drawing
		//glDrawArrays(GL_TRIANGLES, 0, nrOfVertices); //drawing the entire array of vertices
		glDrawElements(GL_TRIANGLES, nrOfIndices, GL_UNSIGNED_INT, 0); //drawing vertices by GLuint indices

		//End of Rendering
		glfwSwapBuffers(window);
		glFlush();

		glBindVertexArray(0);
		glUseProgram(0);
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	//End of program
	glfwDestroyWindow(window);
	glfwTerminate();

	//Remove VAO and Buffers
	return 0;
}
