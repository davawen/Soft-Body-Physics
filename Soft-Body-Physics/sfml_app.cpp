#include <SFML/Graphics.hpp>
#include <vector>

float sqr(float value)
{
    return value * value;
}

float distance(float x1, float y1, float x2, float y2)
{
    return sqrtf(sqr(x1 - x2) + sqr(y1 - y2));
}

float distance(sf::Vector2f v)
{
    return sqrtf(v.x*v.x + v.y*v.y);
}

float distance(sf::Vector2f v1, sf::Vector2f v2)
{
    return sqrtf(sqr(v1.x - v2.x) + sqr(v1.y - v2.y));
}

class Point
{
    private:
    sf::CircleShape shape;

    public:
    sf::Vector2f pos;
    sf::Vector2f vel;
    sf::Vector2f acc;

    Point() : Point(0.f, 0.f) {}
    Point(float x, float y)
    {
        this->pos = { x, y };
        this->vel = { 0, 0 };
        this->acc = { 0, 0 };

        this->shape = sf::CircleShape(10.f);
        this->shape.setOrigin(10.f, 10.f);
    }

    void update(float dt)
    {
		//this->acc.y += 9.81; //Gravity

        this->vel -= this->vel * dt * 5.f;
        this->vel += this->acc;

        this->pos += this->vel * dt;

        this->shape.setPosition(this->pos);

        this->acc = { 0, 0 };
    }

    void draw(sf::RenderWindow &window)
    {
        window.draw(this->shape);
    }
};

class Spring
{
    private:
    float baseHeight;
    float stiffness = 0.5f;

    sf::VertexArray shape = sf::VertexArray(sf::PrimitiveType::LineStrip, 2);

    public:
    Point *p1;
    Point *p2;

    Spring(Point *p1, Point *p2)
    {
        this->baseHeight = distance(p1->pos, p2->pos);

        this->p1 = p1;
        this->p2 = p2;

        this->shape[0].color = sf::Color::Red;
        this->shape[1].color = sf::Color::Red;
    }

    void update(float dt)
    {
        sf::Vector2f relativePosition = this->p1->pos - this->p2->pos; // from p2 to p1

        float length = distance(relativePosition);

        sf::Vector2f force = dt * (length-this->baseHeight) * this->stiffness * relativePosition;

        this->p1->acc += -force;
        this->p2->acc += force;

        this->shape[0].position = this->p1->pos;
        this->shape[1].position = this->p2->pos;
    }

    void draw(sf::RenderWindow &window)
    {
        window.draw(this->shape);
    }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 800), "SFML works!");

	Point p[6] =
	{
		Point(400.f, 400.f),
		Point(600.f, 400.f),
		Point(400.f, 600.f),
		Point(600.f, 600.f),
		Point(400.f, 800.f),
		Point(600.f, 800.f)
	};

    Spring springs[12] =
    {
        Spring(&p[0], &p[1]),
        Spring(&p[0], &p[2]),
        Spring(&p[0], &p[3]),
        Spring(&p[1], &p[3]),
        Spring(&p[1], &p[2]),
        Spring(&p[2], &p[3]),
		Spring(&p[2], &p[3]),
        Spring(&p[2], &p[4]),
        Spring(&p[2], &p[5]),
        Spring(&p[3], &p[5]),
        Spring(&p[3], &p[4]),
        Spring(&p[4], &p[5])
    };

    sf::Clock deltatimeClock;

    float dt = 1.f/60.f;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

		p[0].pos = sf::Vector2f(sf::Mouse::getPosition(window));

        for(int i = 0; i < 12; i++)
        {
            springs[i].update(dt);
            springs[i].draw(window);
        }

		for(int i = 0; i < 6; i++)
		{
			p[i].update(dt);
			p[i].draw(window);
		}


        window.display();
        window.clear();

        dt = deltatimeClock.restart().asSeconds();
    }

    return 0;
}