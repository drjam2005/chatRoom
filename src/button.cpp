#include <button.h>

Button::Button(Rectangle dimensions, char* text, float round){
	this->dimensions = dimensions;
	this->text = text;
	this->round = round;
}

void Button::SetClick(std::function<void()> func){
	this->onClick = func;
}
void Button::SetHold(std::function<void()> func){
	this->onHold = func;
}
void Button::SetRelease(std::function<void()> func){
	this->onRelease = func;
}

void Button::Render() {
    bool isInside = CheckCollisionPointRec(GetMousePosition(), dimensions);
	Color clr = WHITE;
	if(isInside)
		clr = GRAY;
	if(isInside && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
		clr = DARKGRAY;

    Rectangle verticalPart = {
        dimensions.x + round,
        dimensions.y,
        dimensions.width - (2 * round),
        dimensions.height
    };

    Rectangle horizontalPart = {
        dimensions.x,
        dimensions.y + round,
        dimensions.width,
        dimensions.height - (2 * round),
    };

    DrawCircle(dimensions.x + round,               dimensions.y + round,               round, clr);
    DrawCircle(dimensions.x + dimensions.width - round, dimensions.y + round,               round, clr);
    DrawCircle(dimensions.x + round,               dimensions.y + dimensions.height - round, round, clr);
    DrawCircle(dimensions.x + dimensions.width - round, dimensions.y + dimensions.height - round, round, clr);

    DrawRectangle(verticalPart.x, verticalPart.y, verticalPart.width, verticalPart.height, clr);
    DrawRectangle(horizontalPart.x, horizontalPart.y, horizontalPart.width, horizontalPart.height, clr);

    DrawText(this->text, dimensions.x + 10, dimensions.y + 10, 10, BLACK);
}

void Button::Update(){
	bool isInside = CheckCollisionPointRec(GetMousePosition(), dimensions);
	if(isInside){
		if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
			if(onClick)
				onClick();
		}
		if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)){
			if(onHold)
				onHold();
		}
		if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)){
			if(onRelease)
				onRelease();
		}
	}
}
