#pragma once

#include "raylib.h"
#include "types.h"
#include "box2d/math_functions.h"

//std
#include <string>
#include <vector>

struct TextureData
{
    b2Vec2 relativePosition = b2Vec2{0,0};
    Texture2D texture = Texture2D();
};

class TextureComponent
{
public:
    TextureComponent();
    ~TextureComponent();

    void addTexture(const std::string& path, b2Vec2 relativePosition, b2Vec2 size);
    [[nodiscard]] const std::vector<TextureData>& getTexturesData() const;

private:

    std::vector<TextureData> dataVector_;
};

inline TextureComponent::TextureComponent()
= default;

inline TextureComponent::~TextureComponent()
{
    for (const auto& textureData : dataVector_)
    {
        UnloadTexture(textureData.texture);
    }
}

inline void TextureComponent::addTexture(const std::string &path, b2Vec2 relativePosition, b2Vec2 size)
{
    TextureData textureData;
    textureData.relativePosition = relativePosition;
    textureData.texture = LoadTexture(path.c_str());
    textureData.texture.height = size.y;
    textureData.texture.width = size.x;
    dataVector_.push_back(textureData);
}

inline const std::vector<TextureData> & TextureComponent::getTexturesData() const
{
    return dataVector_;
}
