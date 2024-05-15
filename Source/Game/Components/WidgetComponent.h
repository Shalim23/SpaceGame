#pragma once
#include <memory>
#include "../Types/Widget.h"

enum class WidgetLayer
{
    HUD,
    EFFECTS,
    MENU,
    COUNT
};

class WidgetComponent
{
public:
    void setLayer(const WidgetLayer layer) { layer_ = layer; }
    WidgetLayer getLayer() const { return layer_; }
    Widget& addWidget() { return *widgets_.emplace_back(std::make_unique<Widget>()).get(); }
    std::vector<std::unique_ptr<Widget>>& updateWidgets() { return widgets_; }
    std::vector<const RenderData*> gatherRenderData()
    {
        std::vector<const RenderData*> data;
        for (auto& widget : widgets_)
        {
            widget->gatherRenderData(data);
        }
        return data;
    }

private:
    WidgetLayer layer_{WidgetLayer::COUNT};
    std::vector<std::unique_ptr<Widget>> widgets_;
};