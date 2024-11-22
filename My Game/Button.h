#include "Renderer.h"
#include "Math.h"


/*
class Button {
private:
    Vector2 m_Position;  // Top-left corner of the button
    Vector2 m_Size;      // Width and height of the button
    std::string m_Text;  // Label for the button
    XMFLOAT4 m_Color;    // Button color

public:
    Button(const Vector2& position, const Vector2& size, const std::string& text, const XMFLOAT4& color)
        : m_Position(position), m_Size(size), m_Text(text), m_Color(color) {}

    void Render(CRenderer* renderer) const {
        // Draw button background (rectangle)
        renderer->DrawRectangle(m_Position, m_Size, m_Color);

        // Draw button text
        Vector2 textPosition = m_Position + Vector2(10.0f, 10.0f); // Offset text slightly
        renderer->DrawScreenText(m_Text.c_str(), textPosition);
    }

    bool IsClicked(const Vector2& mousePosition) const {
        return (mousePosition.x >= m_Position.x &&
            mousePosition.x <= m_Position.x + m_Size.x &&
            mousePosition.y >= m_Position.y &&
            mousePosition.y <= m_Position.y + m_Size.y);
    }
};
*/

