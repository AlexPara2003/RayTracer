#include "Renderer/Renderer.h"
#include "Object/Sphere.h"
#include "Object/Scene.h"
#include "Object/Plane.h"
#include <iostream>

void InitScene01(Scene& scene);
void InitScene02(Scene& scene);

int main(int, char**)
{

	const int width = 800;
	const int height = 600;
	const int samples = 1;
	const float fov = 30;

	Renderer renderer;
	renderer.Initialize();
	renderer.CreateWindow(width, height);

	Canvas canvas(width, height, renderer);
	//Camera camera({ 0, 4, 4 }, { 0, 0, 0 }, { 0, 1, 0 }, 70.0f, width / (float)height);
	Camera camera({ 13, 2, 3 }, { 0, 0, 0 }, { 0, 1, 0 }, fov, width / (float)height);
	Scene scene{{1, 0.5f, 0.5f}, {0.5f, 0.5f, 1}};
	InitScene02(scene);

	//scene.AddObject(std::make_unique<Sphere>(glm::vec3{ 0, -100.5f, -1 }, 100.0f, std::make_unique<Lambertian>(color3{ 1, 1, 0.2f })));

	bool quit = false;
	while (!quit)
	{
		SDL_Event event;
		SDL_PollEvent(&event);
		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				quit = true;
				break;
			}
			break;
		}

		//render scene
		canvas.Clear({ 0, 0, 0, 1 });
		renderer.Render(canvas, scene, camera, samples);

		canvas.Update();

		renderer.CopyCanvas(canvas);
		renderer.Present();
	}
	renderer.Shutdown();
	return 0;
}
//P
void InitScene01(Scene& scene)
{
	//Objects
	scene.AddObject(std::make_unique<Plane>(glm::vec3{ 0, 0, 0 }, glm::vec3{ 0, 1, 0 }, std::make_unique<Lambertian>(color3{ 0.2f, 0.2f, 0.2f })));

	scene.AddObject(std::make_unique<Sphere>(glm::vec3{ 0, 0.5f, -1 }, 0.5f, std::make_unique<Metal>(color3{ 1, 1, 1 }, 0.1f)));
	scene.AddObject(std::make_unique<Sphere>(glm::vec3{ 1, 0.5f, -1 }, 0.5f, std::make_unique<Lambertian>(color3{ 0.2f, 0, 0.2f })));

	scene.AddObject(std::make_unique<Sphere>(glm::vec3{ 3, 0.5f, -1 }, 0.5f, std::make_unique<Dielectric>(color3{ 1.0f, 1.0f, 1.0f }, 2.0f)));

	//Light
	scene.AddObject(std::make_unique<Sphere>(glm::vec3{ 5, 5.0f, 3 }, 1.0f, std::make_unique<Emissive>(color3{ 40, 40, 40 })));
}

void InitScene02(Scene& scene)
{
	scene.AddObject(std::make_unique<Sphere>(glm::vec3{ 0, 1, 0 }, 1.0f, std::make_unique<Dielectric>(color3{ 1.0f, 1.0f, 1.0f }, 1.5f)));
	scene.AddObject(std::make_unique<Sphere>(glm::vec3{ -4, 1, 0 }, 1.0f, std::make_unique<Lambertian>(color3{ 0.4f, 0.2f, 0.1f })));
	scene.AddObject(std::make_unique<Sphere>(glm::vec3{ 4, 1, 0 }, 1.0f, std::make_unique<Metal>(color3{ 0.7f, 0.6f, 0.5f }, 0.0f)));

	scene.AddObject(std::make_unique<Plane>(glm::vec3{ 0, 0, 0 }, glm::vec3{ 0, 1, 0 }, std::make_unique<Lambertian>(color3{ 0.5f, 0.5f, 0.5f })));

	for (int x = -11; x < 11; x++)
	{
		for (int z = -11; z < 11; z++)
		{
			glm::vec3 center(x + 0.9f * random01(), 0.2f, z + 0.9f * random01());

			if ((center - glm::vec3{ 4, 0.2, 0 }).length() > 0.9f)
			{
				std::unique_ptr<Material> material;

				float choose_mat = random01();
				if (choose_mat < 0.8f) // diffuse
				{
					color3 albedo = color3{ random01(), random01(), random01() } *color3{ random01(), random01(), random01() };
					material = std::make_unique<Lambertian>(albedo);
				}
				else if (choose_mat < 0.95) // metal
				{
					color3 albedo = color3{ random(0.5f, 1), random(0.5f, 1), random(0.5f, 1) };
					float fuzz = random(0.0f, 0.5f);
					material = std::make_unique<Metal>(albedo, fuzz);
				}
				else // glass
				{
					color3 albedo = color3{ 1, 1, 1 };
					material = std::make_unique<Dielectric>(albedo, 1.5f);
				}

				scene.AddObject(std::make_unique<Sphere>(center, 0.2f, std::move(material)));
			}
		}
	}
}