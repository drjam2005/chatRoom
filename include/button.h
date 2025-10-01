#pragma once
#include <raylib.h>
#include <functional>
#include <string>

class Button {
	private:
		std::function<void()> onClick;
		std::function<void()> onHold;
		std::function<void()> onRelease;
		Rectangle dimensions;
		float round = 0.f;
		bool autoscale = false;
		char* text = nullptr;
	public:
		Button(){}
		Button(Rectangle, char*, float round=0.0f);
		void SetClick(std::function<void()>);
		void SetHold(std::function<void()>);
		void SetRelease(std::function<void()>);

		void Render();
		void Update();
};
