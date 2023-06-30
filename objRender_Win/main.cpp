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
	0, 1, 2,	//Триугольник 1 :. (нижний левый)
	0, 2, 3		//Триугольник 2 ': (верхний правый)
};
unsigned nrOfIndices = sizeof(indices) / sizeof(GLuint);

//работа кнопки Esc - закрывание окна
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

bool loadShaders(GLuint &program)
{
	bool loadSuccess = true;
	char infoLog[512];
	GLint success;
	std::string temp = "";
	std::string src = "";

	std::ifstream in_file;

	//Vertex
	in_file.open("vertex_core.glsl");//.c_str() для Linux

	if (in_file.is_open())
	{
		while (std::getline(in_file, temp))
			src += temp + "\n";
	}
	else
	{
		std::cout << "ERROR::LOADSHADERS::COULD_NOT_OPEN_VERTEX_FILE" << "\n";
		loadSuccess = false;
	}

	in_file.close();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* vertSrc = src.c_str();
	glShaderSource(vertexShader, 1, &vertSrc, NULL);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::LOADSHADERS::COULD_NOT_COMPILE_VERTEX_SHADER" << "\n";
		std::cout << infoLog << "\n";
		loadSuccess = false;
	}

	temp = "";
	src = "";

	//Фрагмент
	in_file.open("fragment_core.glsl");
	//in_file.open("vertex_core.glsl");//если не будет работать верхняя строчка

	if (in_file.is_open())
	{
		while (std::getline(in_file, temp))
			src += temp + "\n";
	}
	else
	{
		std::cout << "ERROR::LOADSHADERS::COULD_NOT_OPEN_FRAGMENT_FILE" << "\n";
		loadSuccess = false;
	}

	in_file.close();

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* fragSrc = src.c_str();
	glShaderSource(fragmentShader, 1, &fragSrc, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::LOADSHADERS::COULD_NOT_COMPILE_FRAGMENT_SHADER" << "\n";
		std::cout << infoLog << "\n";
		loadSuccess = false;
	}

	//Program
	program = glCreateProgram();

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if(!success)
	{
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << "ERROR::LOADSHADERS::COULD_NOT_LINK_PROGRAM" << "\n";
		std::cout << infoLog << "\n";
		loadSuccess = false;
	}

	//End
	glUseProgram(0);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return loadSuccess;
}

//перемещение объекта через кнопки WSAD
void updateInit(GLFWwindow* window, glm::vec3& position, glm::vec3& rotation, glm::vec3& scale)
{
	//перемещение вперед(дальше от камеры) через кнопку W
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		position.z -= 0.01f;
	}
	//перемещение назад(ближе к камере) через кнопку S
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		position.z += 0.01f;
	}
	//перемещение влево через кнопку A
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		position.x -= 0.01f;
	}
	//перемещение вправо через кнопку D
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		position.x += 0.01f;
	}
	//поворот по часовой стрелке через кнопку Q
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		rotation.y -= 0.1f;
	}
	//поворот против часовой стрелки через кнопку E
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		rotation.y += 0.1f;
	}
	//увеличение через кнопку Z
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
	{
		scale += 0.1f;
	}
	//уменьшение через кнопку X
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
	{
		scale -= 0.1f;
	}
}

int main()
{
	//инициализация GLFW
	glfwInit();

	//создание окна
	const int WINDOW_WIDTH = 640;
	const int WINDOW_HEIGHT = 480;
	int framebufferWidth = 0;
	int framebufferHeight = 0;

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //для MAC OS

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OBJ_RENDER", NULL, NULL);
	
	//Важно для матрицы перспективы!
	glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
	glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);
	
	//glViewport(0, 0, framebufferWidth, framebufferHeight); //окно без возможности растягивания, мальенькое

	//Важно для вывода окна рендера!
	glfwMakeContextCurrent(window);

	//инициализация GLEW (необходимо окно и контекст OPENGL)
	glewExperimental = GL_TRUE;

	//Ошибка
	if (glewInit() != GLEW_OK)
	{
		std::cout << "ERROR::MAIN.CPP::GLEW_INIT_FAILED" << "\n";
		glfwTerminate();
	}

	//OPENGL параметры
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);//обратная сторона треугольника - по часовой стрелке
	glFrontFace(GL_CCW);//передняя сторона треуголника - против часовой стрелки

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//инициализация SHADER
	GLuint core_program;
	if (!loadShaders(core_program))
		glfwTerminate();

	//модель


	//VAO, VBO, EBO - буферы
	
	//генерация VAO и связка
	GLuint VAO;
	glCreateVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//генерация VBO(буфер вершин), связка и отправка данных
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//GL_STATIC_DRAW - если отрисовка происходит редко
	//GL_DYNAMIC_DRAW - если отрисовка происходит часто

	//генерация EBO(фактическое индексирование вершин), связка и отправка данных
	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//набор атрибутов вершин и Enable
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

	//привязка VAO к 0
	glBindVertexArray(0);

	//инициализация текстуры

	//текстура0 передается на нуливой вход видеокарты
	int image_width = 0;
	int image_height = 0;
	unsigned char* image = SOIL_load_image("Images/teapot.png", &image_width, &image_height, NULL, SOIL_LOAD_RGBA);
	
	GLuint texture0;
	glGenTextures(1, &texture0);
	glBindTexture(GL_TEXTURE_2D, texture0);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//S-координата x;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//T-координата y;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	if (image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "ERROR::TEXTURE_LOADING_FAILED" << "\n";
	}

	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);

	//текстура1 передается на первый вход видеокарты
	int image_width1 = 0;
	int image_height1 = 0;
	unsigned char* image1 = SOIL_load_image("Images/table.png", &image_width1, &image_height1, NULL, SOIL_LOAD_RGBA);

	GLuint texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//S-координата x;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//T-координата y;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	if (image1)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width1, image_height1, 0, GL_RGBA, GL_UNSIGNED_BYTE, image1);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "ERROR::TEXTURE_LOADING_FAILED" << "\n";
	}

	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image1);

	//Инициализация мтриц
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

	//Проекционная матрица песпективы
	float fov = 90.f;
	float nearPlane = 0.1f;
	float farPlane = 100.f;//если хорошее железо можно увеличит до 1000
	glm::mat4 ProjectionMatrix(1.f);

	ProjectionMatrix = glm::perspective(
		glm::radians(fov), 
		static_cast<float>(framebufferWidth) / framebufferHeight,
		nearPlane,
		farPlane
	);

	//Свет
	glm::vec3 lightPos0(0.f, 0.f, 1.f);//навигация "лампочки" 0.f, 0.f, 0.12f для проверки

	//Инициализвция Uniforms
	glUseProgram(core_program);

	glUniformMatrix4fv(glGetUniformLocation(core_program, "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(ModelMatrix));
	glUniformMatrix4fv(glGetUniformLocation(core_program, "ViewMatrix"), 1, GL_FALSE, glm::value_ptr(ViewMatrix));
	glUniformMatrix4fv(glGetUniformLocation(core_program, "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));
	
	glUniform3fv(glGetUniformLocation(core_program, "lightPos0"), 1, glm::value_ptr(lightPos0));
	glUniform3fv(glGetUniformLocation(core_program, "cameraPos"), 1, glm::value_ptr(camPosition));

	glUseProgram(0);

	//основной цикл
	while (!glfwWindowShouldClose(window))
	{
		//ВВОД ОБНОВЛЕНИЙ ---
		glfwPollEvents();
		updateInit(window, position, rotation, scale);

		//ОБНОВЛЕНИЕ ---
		updateInput(window);

		//ОТРИСОВКА ---

		//Очистка
		glClearColor(0.f, 0.f, 0.f, 1.f);//цвет фона окна
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
		//Использование программы
		glUseProgram(core_program);

		//Update uniforms
		glUniform1i(glGetUniformLocation(core_program, "texture0"), 0);
		glUniform1i(glGetUniformLocation(core_program, "texture1"), 1);

		//Move, rotate, scale
		/* //так можно делать анимации
		position.z -= 0.0005f;
		rotation.y += 0.02f;
		scale += 0.001f; */
		
		ModelMatrix = glm::mat4(1.f);
		ModelMatrix = glm::translate(ModelMatrix, position);
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f));
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));
		ModelMatrix = glm::scale(ModelMatrix, scale);
		 
		glUniformMatrix4fv(glGetUniformLocation(core_program, "ModelMatrix"), 1, GL_FALSE, glm::value_ptr(ModelMatrix));
		
		glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
		
		//ProjectionMatrix = glm::mat4(1.f);
		ProjectionMatrix = glm::perspective(
			glm::radians(fov),
			static_cast<float>(framebufferWidth) / framebufferHeight,
			nearPlane,
			farPlane
		);
		glUniformMatrix4fv(glGetUniformLocation(core_program, "ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));

		//Активная текстура
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture1);

		//Связка массива вершин (вложена в объект)
		glBindVertexArray(VAO);

		//Отрисовка вершин
		//glDrawArrays(GL_TRIANGLES, 0, nrOfVertices);//отрисовка всего массива вершин
		glDrawElements(GL_TRIANGLES, nrOfIndices, GL_UNSIGNED_INT, 0);//отрисовка вершин по индексам GLuint indices

		//Конец Отрисовки
		glfwSwapBuffers(window);
		glFlush();

		glBindVertexArray(0);
		glUseProgram(0);
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	//Конец program
	glfwSwapBuffers(window);
	glfwTerminate();

	//Удалить program
	glDeleteProgram(core_program);

	//Удалить VAO и Buffers
	return 0;
}