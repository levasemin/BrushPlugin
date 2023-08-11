#include "Paint.hpp"

ToolPaint::ToolPaint() : booba::Tool(),
    interpolator_(Interpolator::CATMULL_ROM),
    points_({}),
    drawing_object_(min_width_),
    drawing_object_empty_(min_width_, booba::Color(0, 0, 0, 0)),
    drawing_object_frame_(min_width_),
    color_()
    {
        widget_creator_   = dynamic_cast<booba::WidgetCreator *>(booba::getWidgetCreator(this));
        
        width_scroll_bar_ = dynamic_cast<booba::ScrollBar *>(widget_creator_->createWidget(booba::WidgetCreator::Type::SCROLLBAR, 150, 30, 10, 10));
        width_editor_     = dynamic_cast<booba::Editor *>   (widget_creator_->createWidget(booba::WidgetCreator::Type::EDITOR,    50,  30, 50, 50));

        width_scroll_bar_->setMinValue(min_width_);
        width_scroll_bar_->setMaxValue(max_width_);
        width_scroll_bar_->setCommand(dynamic_cast<booba::Command<float> *>(new booba::SimpleCommand<ToolPaint, float>(this, &ToolPaint::set_width)));

        width_editor_->setCommand(dynamic_cast<booba::Command<std::string> *>(new booba::SimpleCommand<ToolPaint, std::string>(this, &ToolPaint::set_width)));
    }

ToolPaint::~ToolPaint()
{
    delete width_scroll_bar_->getCommand();
    delete width_editor_->getCommand();
}

void ToolPaint::set_width(std::string string)
{    
    if (string.size() == 0)
    {
        width_scroll_bar_->setValue(0);
        return;
    }

    if ('0' > string[string.size() - 1] || string[string.size() - 1] > '9')
    {
        string.resize(string.size() - 1);
        width_editor_->setText(string);
        
        return;
    }
    
    float width = string.size() > 0 ? static_cast<float>(std::stoi(string)) : 0;
    width = width <= max_width_ ? width : max_width_;
    
    width_scroll_bar_->setValue(width);

    drawing_object_      .setRadius(width == 0 ? min_width_ : static_cast<int>(width / 2));
    drawing_object_frame_.setRadius(width == 0 ? min_width_ : static_cast<int>(width / 2));
    drawing_object_empty_.setRadius(width == 0 ? min_width_ : static_cast<int>(width / 2));
}

void ToolPaint::set_width(float width)
{    
    int width_ = int(width);
    
    if (width_ != 0)
    {
        width_editor_->setText(std::to_string(width_));
    }
    
    drawing_object_.setRadius(width_ == 0 ? min_width_ : width_ / 2);
    drawing_object_frame_.setRadius(width_ == 0 ? min_width_ : width_ / 2);
    drawing_object_empty_.setRadius(width == 0 ? min_width_ : static_cast<int>(width / 2));
}

void ToolPaint::paint(booba::Image *image)
{   
    int x = points_.back().first  - drawing_object_.getW() / 2;
    int y = points_.back().second - drawing_object_.getH() / 2; 

    image->setPicture(drawing_object_, x, y, 0, 0, max_width_, max_width_, true);

    if (points_.size() == 4)
    {
        for (float t = 0; t <= 1.f; t += 0.1f)
        {
            std::pair<int, int> new_point = interpolator_(t, points_[0], points_[1], points_[2], points_[3]);
            
            int x = new_point.first  - drawing_object_.getW() / 2;
            int y = new_point.second - drawing_object_.getH() / 2;

            image->setPicture(drawing_object_, x, y, 0, 0, max_width_, max_width_, true);
        }
    }
}

void ToolPaint::apply(booba::Image* image, booba::Image *hidden_layer, const booba::Event* event)
{    
    color_ = booba::APPCONTEXT->fgColor;
    drawing_object_.setColor(color_);
    drawing_object_frame_.setColor(color_);
    
    switch (event->type)
    {
        case booba::Event::MousePressed:
        {
            if (event->Oleg.mpedata.button == booba::Event::MouseButton::Left)
            {
                clicked_ = true;

                std::pair<size_t, size_t> new_point(event->Oleg.mpedata.x, event->Oleg.mpedata.y);

                points_.push_back(new_point);
                paint(image);
                points_.pop_back();
            }

            break;
        }

        case booba::Event::MouseReleased:
        {
            if (event->Oleg.mpedata.button == booba::Event::MouseButton::Left)
            {
                clicked_ = false;
            }

            break;
        }

        case booba::Event::MouseMoved:
        {
            std::pair<int64_t, int64_t> new_point(static_cast<int64_t>(event->Oleg.motion.x), static_cast<int64_t>(event->Oleg.motion.y));

            if (clicked_)
            {
                if (points_.size() > 0)
                {
                    if (abs(points_.back().first - new_point.first) > 10 || abs(points_.back().second - new_point.second) > 10)
                    {
                        points_.clear();
                    }
                }

                points_.push_back(new_point);
                
                if (points_.size() > 4)
                {
                    points_.pop_front();
                }

                paint(image);
            }

            static int x = 0;
            static int y = 0;

            hidden_layer->setPicture(drawing_object_empty_, x, y, 0, 0, max_width_, max_width_, false);

            x = new_point.first  - drawing_object_frame_.getW() / 2;
            y = new_point.second - drawing_object_frame_.getH() / 2;
            
            hidden_layer->setPicture(drawing_object_frame_, x, y, 0, 0, max_width_, max_width_, false);

            break;
        }
        
        case booba::Event::NoEvent:
        
        default:
            break;
    }
}

const char* ToolPaint::getTexture()
{
    return icon_path_.c_str();;
} 

std::pair<int, int> ToolPaint::getShape()
{
    return {200, 200};
}

extern "C" booba::Tool *init_module()
{
    return new ToolPaint();
}