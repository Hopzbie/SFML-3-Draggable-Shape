// libarary
#include <SFML/Graphics.hpp>

// class - mouse
class Mouse
{
    public:

    // position
    float x;
    float y;

    // states
    bool isPressed;
    bool onPress;
    bool onRelease;
    bool isDragging = false;

    // function - update
    void Update(sf::RenderWindow& window)
    {
        // use mouse namespace
        using namespace sf::Mouse;

        // get mouse position relative to window
        auto position = sf::Vector2f(getPosition(window));
        x = position.x;
        y = position.y;

        // reset states
        onPress   = false;
        onRelease = false;

        // when left button is pressed
        if (isButtonPressed(Button::Left))
        {
            // when state isn't set yet
            if (not isPressed)
                onPress = true;

            // set state
            isPressed = true;
        }
        else
        {
            // when state isn't reset yet
            if (isPressed)
                onRelease = true;

            // reset state
            isPressed = false;
        }
    }
};

// class - shape
class Shape
{
    public:

    // size
    float w;
    float h;

    // color
    uint8_t r = 255;
    uint8_t g = 255;
    uint8_t b = 255;
    uint8_t a = 255;

    // position
    float x = 0.f;
    float y = 0.f;

    // offset
    float offset_x;
    float offset_y;

    // state
    bool isDragged = false;

    // constructor
    Shape(float w, float h) : w(w), h(h) {}

    // function - check if position is inside shape
    bool CheckInside(float x, float y)
    {
        if (x > this->x and x < this->x + w and
            y > this->y and y < this->y + h)
            return true;

        return false;
    }

    // function - update (mouse drag)
    void UpdateDrag(Mouse& mouse)
    {
        // on mouse press inside
        if (not mouse.isDragging and mouse.onPress and CheckInside(mouse.x, mouse.y))
        {
            // set offset
            offset_x = mouse.x - x;
            offset_y = mouse.y - y;

            // set states
            isDragged        = true;
            mouse.isDragging = true;
        }

        // on mouse release
        else if (mouse.onRelease)
        {
            // reset states
            isDragged        = false;
            mouse.isDragging = false;
        }

        // when shape is dragged
        else if (isDragged)
        {
            // set position
            x = mouse.x - offset_x;
            y = mouse.y - offset_y;
        }
    }

    // function - draw on window
    void Draw(sf::RenderWindow& window, sf::RectangleShape& rectangle)
    {
        rectangle.setSize({w, h});
        rectangle.setFillColor(sf::Color(r, g, b, a));
        rectangle.setPosition({x, y});
        window.draw(rectangle);
    }
};

// main program
int main()
{
    // create mouse
    Mouse mouse;

    // create shape
    Shape shape_1(100.f, 100.f);
    Shape shape_2(150.f, 150.f);
    shape_1.r = 0;
    shape_1.g = 170;
    shape_1.b = 255;

    // create rectangle shape
    sf::RectangleShape rectangle;

    // create window
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Title");

    // window loop
    while (window.isOpen())
    {
        // handle events
        while (auto event = window.pollEvent())
        {
            // on close button release
            if (event->is<sf::Event::Closed>())
            {
                // close window
                window.close();
            }

            // when window is resized
            else if (auto resized = event->getIf<sf::Event::Resized>())
            {
                // update view
                window.setView(sf::View(sf::FloatRect({0.f, 0.f}, sf::Vector2f(resized->size))));
            }
        }

        // update
        mouse.Update(window);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
        {
            shape_2.UpdateDrag(mouse);
            shape_1.UpdateDrag(mouse);
        }
        else
        {
            shape_1.UpdateDrag(mouse);
            shape_2.UpdateDrag(mouse);
        }

        // fill background color
        window.clear(sf::Color(64, 64, 64));

        // draw shape
        shape_2.Draw(window, rectangle);
        shape_1.Draw(window, rectangle);

        // update display
        window.display();
    }

    // program end successfully
    return 0;
}