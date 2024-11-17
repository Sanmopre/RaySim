#pragma once
#include "raylib.h"

namespace ray_sim
{
    struct SimModel
    {
    public:
        SimModel() = default;
        ~SimModel();

      [[nodiscard]] Model& getModel();
        void loadModelFromMesh(const Mesh& mesh);
        void setColor(const Color& color);
        [[nodiscard]] const Color& getColor() const;
    private:
        Model model;
        Color color;
    };

    inline SimModel::~SimModel()
    {
        UnloadModel(model);
    }

    inline Model & SimModel::getModel()
    {
        return model;
    }

    inline void SimModel::loadModelFromMesh(const Mesh &mesh)
    {
        model = LoadModelFromMesh(mesh);
    }

    inline void SimModel::setColor(const Color &color)
    {
        this->color = color;
    }

    inline const Color & SimModel::getColor() const
    {
        return color;
    }
}
