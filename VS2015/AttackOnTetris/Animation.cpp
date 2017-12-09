#include "Animation.h"

// Calculate source rectangles and store in drawOrder
Animation::Animation(std::vector<int> clips, const Vector2i& sheetDimensions, const Vector2i& clipSize, float speed, bool loop) : speed(speed), loop(loop)
{
	srcRects.reserve(clips.size());

	for (std::vector<int>::iterator it = clips.begin(); it != clips.end(); ++it)
	{
		// Note: Default sprite origin is top-left &
		// texture grid is traversed in left-right rows starting from the top
		int rectX = (*it % sheetDimensions.x) * clipSize.x;
		int rectY = (*it / sheetDimensions.x) * clipSize.y;

		srcRects.push_back({ rectX, rectY, clipSize.x, clipSize.y });
	}
}

Animation::Animation(std::vector<SDL_Rect> srcRects, float speed, bool loop) : srcRects(srcRects), speed(speed), loop(loop)
{
}
