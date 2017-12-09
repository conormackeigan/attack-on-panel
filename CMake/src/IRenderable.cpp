#include "IRenderable.h"
#include "RenderSystem.h"

void IRenderable::SetDrawOrder(int draw_order)
{
	drawOrder = draw_order;

	RenderSystem::Instance().SortRenderables();
}

int IRenderable::GetDrawOrder() const
{
	return drawOrder;
}

// Operators (for draw drawOrder sorting)
bool operator<(const IRenderable& lhs, const IRenderable &rhs) { return lhs.GetDrawOrder() < rhs.GetDrawOrder(); }
bool operator>(const IRenderable& lhs, const IRenderable &rhs) { return lhs.GetDrawOrder() > rhs.GetDrawOrder(); }