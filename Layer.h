#pragma once
#include "Log.h"
#include <vector>

namespace Crack
{
	class Layer
	{
	public:
		Layer* GetLayer() { return this;  }
		virtual void Init() {}
		virtual void Update() {}
		virtual void OnClose() {}
	};
	struct LayerStack
	{
		std::vector<Layer*> layers;
		void PushLayer(Layer* layer) { layers.push_back(layer); }

	};
};