#include "Events.h"
#include "glfw3.h"
#include "Editor.h"

glm::vec2 Crack::Event::GetMousePosition()
{
    double xCursor, yCursor;
    glfwGetCursorPos(Editor::Get().GetWindow(), &xCursor, &yCursor);
    return glm::vec2((float)xCursor, (float)yCursor);
}
