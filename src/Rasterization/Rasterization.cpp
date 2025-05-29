#include "base/Painter.hpp"
#include <Rasterization/Rasterization.hpp>

Rasterization::Rasterization()
:painter(Painter::getInstance())
{

}


void Rasterization::Renderer(float time)
{
    painter.Clear(Color{ 255, 255, 255, 255 });
    

}

void Rasterization::DrawLine(Vec2 P0,Vec2 P1,Color color)
{
    //斜率
    auto a = (P1.y - P0.y) / (P1.x - P0.x);
    auto b = P0.y - a * P0.x;
    for (auto x = P0.x; x < P1.x; x++)
    {   
        auto y = a * x + b;
        painter.PutPixel(x, y, color);
    }

}
