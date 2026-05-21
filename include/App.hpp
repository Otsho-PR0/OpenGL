#pragma once

#include <vector>

class AppLayer
{
public:
	virtual void OnAttach() = 0;
	virtual void OnDetach() = 0;
	virtual bool OnUpdate() = 0;
	virtual void OnRender() = 0;
};

class App
{
public:
	static int Run();

	static void AddLayer(AppLayer*);
	static void RemoveLayer(AppLayer*);

private:
	static std::vector<AppLayer*> layers;
};
