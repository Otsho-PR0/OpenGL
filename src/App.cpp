#include <App.hpp>

#include <algorithm>

std::vector<AppLayer*> App::layers;

int App::Run()
{
	while (!layers.empty())
	{
		for (AppLayer* layer : layers)
		{
			if (layer->OnUpdate())
				RemoveLayer(layer);
			else
				layer->OnRender();
		}
	}

	return 0;
}

void App::AddLayer(AppLayer* layer)
{
	layers.push_back(layer);
	layer->OnAttach();
}

void App::RemoveLayer(AppLayer* layer)
{
	layers.erase(std::find(layers.begin(), layers.end(), layer));
	layer->OnDetach();
}
