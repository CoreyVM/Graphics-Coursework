#include "../../nclgl/window.h"
#include "../Graphics Coursework/Renderer.h"

#pragma comment(lib, "nclgl.lib")
int timer = 0;

int main() {
	Window w("Coursework", 1200, 800, false);
	if (!w.HasInitialised())
	{
		return -1;
	}

	Renderer renderer(w);
	if (!renderer.HasInitialised())
	{
		return -1;
	}

	w.LockMouseToWindow(true);
	w.ShowOSPointer(false);

	while (w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)) 
	{
		if (timer < 500)
		{
	//		timer += 1;
		}
		
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_1))
		{
			renderer.SwapTerrainTex();
		}
		if (timer == 500)
		{
			std::cout << "Gay boy";
			renderer.SwapTerrainTex();
		//	timer = 501;
			//return 0;
		}

		renderer.UpdateScene(w.GetTimer()->GetTimedMS());
		std::cout << w.GetTimer();
		renderer.RenderScene();

		
	}

	

	return 0;
}